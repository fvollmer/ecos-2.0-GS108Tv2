//==========================================================================
//
//      common/except.cxx
//
//      Exception handling implementation
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    nickg
// Contributors: nickg, jlarmour
// Date:         1999-02-16
// Purpose:      Exception handling implementation
// Description:  This file contains the code that registers and delivers
//               exceptions.
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <stdio.h>
#include <fcntl.h>
#include <network.h>
#include <pkgconf/kernel.h>

#include <cyg/kernel/ktypes.h>         // base kernel types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
#include <cyg/kernel/instrmnt.h>       // instrumentation

#include <cyg/kernel/except.hxx>       // our header

#include <cyg/hal/hal_arch.h>          // architecture definitions
#include <cyg/hal/hal_intr.h>          // vector definitions

#include <cyg/kernel/thread.hxx>       // thread interface

#include <cyg/kernel/thread.inl>       // thread inlines

#ifdef CYGPKG_KERNEL_EXCEPTIONS

externC void sysReboot(void);
static int already_called;

/* define CRASH_LOG_FILE as 1 to enable writing crash dump to file */
#define CRASH_LOG_FILE 1
externC void osapiFsSync(void);
externC int tprintf( FILE *stream, const char *format, ... );

#define EXTRA_BACKTRACE_DETAIL

#define FRAME_DEPTH   256
#define LINE_LEN        4
#define ISPRINT(c) ((' '<=(c))&&((c)<= '~'))
void dumpMemory(FILE *fp, unsigned long *addr, int size)
{
  int i, j, k, m;
  unsigned long *searchAddr;
  unsigned long value;
  unsigned long temp = (unsigned long)addr;
  char lBuff[80], chr;

  if ((size <= 0) || (size > FRAME_DEPTH)){
    size = FRAME_DEPTH;
  }

  value = 0xFFFFFFFC;
  j = LINE_LEN>>1;
  while (j) {
    value <<= 1;
    value &= 0xFFFFFFFE;
    j >>= 1;
  }

  temp &= value;
  searchAddr = (unsigned long *)temp;

  for(i=0;i<((size+LINE_LEN-1)/LINE_LEN);i++) 
  {
    tprintf(fp, "%08lX:  ", (unsigned long)searchAddr);
    m = 0;
    for(j=0;j<LINE_LEN;j++) 
    {
      if (j == (LINE_LEN/2)) 
      {
        tprintf(fp, " ");
        lBuff[m++] = ' ';        
      }
      value = *searchAddr;
      if (((unsigned long)searchAddr >= (unsigned long)addr) &&
          ((unsigned long)searchAddr < ((unsigned long)addr+(size*4))))
      {
        tprintf(fp, "%08lX ", value);
        for(k=0;k<4;k++) 
        {
          chr = (char)((value>>((3-k)*8)) & 0x000000FF);
          lBuff[m++] = ISPRINT(chr)?chr:'.';        
        }
      }
      else
      {
        tprintf(fp, "         ");
        for(k=0;k<4;k++) 
        {
          lBuff[m++] = '.';        
        }
      }
      searchAddr++;
    }
    lBuff[m] = '\0';
    tprintf(fp, "   %s\n", lBuff);
  }
  tprintf(fp, "\n\n");
}

/*************************************************************************************/
externC volatile CYG_ADDRESS    vectorNop[256];       /* used for HAL Translation table */
static void print_backtrace_mips(cyg_code exception_number,CYG_ADDRWORD exception_info)
{
  unsigned long trace_depth, cur_depth;
  unsigned long *cur_sp, *ra;
  HAL_SavedRegisters *ss;
  unsigned int cur_frame_size, cur_ra_offset, cur_epilog_frame_size;
  volatile unsigned long *code_search_point;
  unsigned long cur_inst, r1, r2, r3, immed, op, dest_reg;
  unsigned long *cur_frame_addr;
  int i, num_inst_scanned;

  if (already_called == 1)
  {
    return;
  }
  
  FILE  *fp = NULL;
#if CRASH_LOG_FILE
  if ((fp = fopen("except.txt","w")) == NULL)
  {
    printf("Cannot open exception file.\n\n");
  }
#endif
  
  already_called =1;

  trace_depth = 0x10UL; /*trace as far as we can*/

  ss = (HAL_SavedRegisters *)exception_info;
  cur_sp = (unsigned long *) ((unsigned long) (ss->d[29]));
  ra = (unsigned long *) ((unsigned long) (ss->d[31]));
  tprintf(fp, "Stack pointer before signal: 0x%08lX\n", (unsigned long)cur_sp);
  tprintf(fp, "Offending instruction at address 0x%08lX\n", (unsigned long)ss->pc);
  tprintf(fp, "tried to access address 0x%08lX\n", ss->badvr);
  tprintf(fp, "CPU's exception-cause code: 0x%08lX\n", ss->cause);

  code_search_point = (unsigned long *)((unsigned long)ss->pc);
  if ((code_search_point == NULL) || 
      ((code_search_point < (unsigned long *)(0x80041000)) &&
       (code_search_point > (unsigned long *)(0x81000000))))
  {
    /* printf("\n EPC is NULL POINTER and changing to RA.\n"); */
    code_search_point = (unsigned long *) ra;
  }

  tprintf(fp, "\nHAL Translation table:\n");
  dumpMemory(fp, (unsigned long *)vectorNop, 64);
  tprintf(fp, "\nInterrupt vector table:\n");
  dumpMemory(fp, (unsigned long *)hal_interrupt_handlers, 64);
  tprintf(fp, "\nRaw stack frame dump:\n");
  dumpMemory(fp, (unsigned long *)cur_sp-(FRAME_DEPTH/4), FRAME_DEPTH/4);
  tprintf(fp, "\n");
  dumpMemory(fp, (unsigned long *)cur_sp, FRAME_DEPTH);

  cur_depth = 0;
  while (trace_depth > cur_depth)
  {    
    tprintf(fp, "---------------------- Stack Depth %lu ------------------------\n", cur_depth);
    /* Figure out from the code how large the stack frame is, and 
       where the return-address register R31 is stored */
    cur_frame_size = 0;
    cur_ra_offset = 0;
    num_inst_scanned = 0; /* Protection against hanging for a long time if we 
			     go off into the weeds */
    cur_epilog_frame_size = 0;
    while (((cur_frame_size == 0) || (cur_ra_offset == 0)) && 
           (num_inst_scanned < 100000))
    {
      if (code_search_point == (unsigned long *)0)
      {
        tprintf(fp, "\n NULL POINTER is encountred return.\n");
        goto print_backtrace_mips_exit;
      }
      cur_inst = *code_search_point;
      num_inst_scanned++;
      op = (cur_inst & 0xFC000000UL)>>26;
      r1 = (cur_inst & 0x03E00000UL)>>21;
      r2 = (cur_inst & 0x001F0000UL)>>16;
      r3 = (cur_inst & 0x0000F800UL)>>11;
      immed = cur_inst & 0x0000FFFFUL;
      /* Don't analyze instructions that don't alter or store
	 registers, to avoid false positives */
      if ((op == 1) || (op == 4) || (op == 2) || (op == 3) ||  
	  (op == 5) || (cur_inst == 0) /* noop */ ||
	  ((cur_inst & 0xF81FFFFF) == 8) /* jr */ || 
	  ((cur_inst & 0xFC00003F) == 0xC) /* syscall */ || 
          ((cur_inst & 0xF800FFF8) == 0x0018) /* mult/div */ )
      {
	code_search_point--;
	continue;
      }
      /* Which register stores the result of this instruction? */
      if (op == 0)
      {
	dest_reg = r3;
      } 
      else if (op == 0x2B)
      { /* sw */
	dest_reg = 0;
      }
      else
      {
	dest_reg = r2;
      }
      if ((op == 0x2B) && (r1 == 29))
      {
	/* sw {r2}, {immed}(sp) */
        if (r2 == 31) 
        { /* ra */
          if ((immed & 0x8000UL) != 0)
          {
            tprintf(fp, "At code addr 0x%08lX found code 0x%08lX that stores\n", 
		     (unsigned long)code_search_point, cur_inst);
            tprintf(fp, "RA at negative offset to SP. Giving up.\n");
            goto print_backtrace_mips_exit;
          }
	  cur_ra_offset = immed;
	}
      }
      else if ((cur_inst & 0xFBFF0000UL) == 0x23BD0000UL) 
      {
	/* addi/addiu sp,sp,{immed} */
        if ((immed & 0x8000UL) == 0) 
        {
	  /* Positive offset - this is either the epilog code at the end of 
	     the previous function, or this function has some nonlinear 
	     control flow with the pop and return in the middle. For the 
	     latter case, we'll only see one epilog, of the same size as 
	     the prolog. */	  
          if (cur_epilog_frame_size == 0)
          {
	    cur_epilog_frame_size = immed;
          }
          else
          {
            tprintf(fp, "At code addr 0x%08lX found function epilog code 0x%08lX\n", 
		   (unsigned long)code_search_point, cur_inst);
            tprintf(fp, "but had not yet found prolog code. Giving up.\n");
            tprintf(fp, "This might mean I hit the top of stack.\n");
            goto print_backtrace_mips_exit;
          }
        }
        else
        {
	  cur_frame_size = (((~immed) & 0xFFFFUL)+1); /* -immed */
	  if ((cur_epilog_frame_size != 0) && 
              (cur_epilog_frame_size != cur_frame_size))
          {
            tprintf(fp, "Found prolog at code addr 0x%08lX, but had seen epilog of different frame size %d on the way. Giving up.\n", (unsigned long)code_search_point, cur_epilog_frame_size);
            goto print_backtrace_mips_exit;
          }
        }
      }
      else if ((dest_reg == 29) && (cur_frame_size == 0))
      {
        tprintf(fp, "At code addr 0x%08lX the code 0x%08lX alters SP, \n", 
		      (unsigned long)code_search_point, cur_inst);
        tprintf(fp, "but had not yet found frame size. Giving up.\n");
        goto print_backtrace_mips_exit;
      } 
      code_search_point--;
    } /* End while looking for ra_offset and frame_size */

    tprintf(fp, "Current frame at 0x%08lX, size %d bytes, retaddr offset %d bytes\n",
	     (unsigned long)cur_sp, cur_frame_size, cur_ra_offset);
       code_search_point = (unsigned long *)(*(cur_sp + (cur_ra_offset / 4)));
    tprintf(fp, "Return address: 0x%08lX\n\n", (unsigned long)code_search_point); 

#ifdef EXTRA_BACKTRACE_DETAIL
    tprintf(fp, "Stack frame dump:\n");
    dumpMemory(fp, (unsigned long *)cur_sp, cur_frame_size/4);
#endif
    if (((((unsigned long)code_search_point) & 0xF0000000UL) < 0x80000000UL) || 
        ((((unsigned long)code_search_point) & 0xF0000000UL) > 0x80400000UL) || 
        (code_search_point == 0))
    {
      tprintf(fp, "\nThat return address doesn't look like userspace code-space. continue anyway.\n");
      break;
    } 
    cur_sp += (cur_frame_size / 4);
    cur_depth++;
    tprintf(fp, "\n");
  } /* end while (trace_depth > cur_depth) */

print_backtrace_mips_exit:
#if CRASH_LOG_FILE
  fflush(fp);
  fclose(fp);
  //osapiFsSync();
#endif
  return;
}

// -------------------------------------------------------------------------
// Null exception handler. This is used to capture exceptions that are
// not caught by user supplied handlers.

void
cyg_null_exception_handler(
    CYG_ADDRWORD        data,                   // user supplied data
    cyg_code            exception_number,       // exception being raised
    CYG_ADDRWORD        exception_info          // any exception specific info
    )
{
    HAL_SavedRegisters *savedreg;
    CYG_HAL_MIPS_REG sp, ra;
    void *sh;
    
    CYG_REPORT_FUNCTION();
    CYG_REPORT_FUNCARG3("data=%08x, exception=%d, info=%08x", data,
                        exception_number, exception_info);
    
    savedreg = (HAL_SavedRegisters *)exception_info;

    sp=(CYG_HAL_MIPS_REG)(savedreg->d[29]);
    ra = (CYG_HAL_MIPS_REG)(savedreg->d[31]);
 
    diag_printf("\n**Exception %d: SP=%08X, RA=%08X, EPC=%08X, Cause=%08X VAddr=%08X\n",
                exception_number, sp, ra, savedreg->pc,
                savedreg->cause, savedreg->badvr);

    print_backtrace_mips(exception_number,exception_info);
    
    sysReboot();
    
    CYG_TRACE1( 1, "Uncaught exception: %d", exception_number);
    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Exception Controller constructor.

Cyg_Exception_Control::Cyg_Exception_Control()
{
    CYG_REPORT_FUNCTION();
#ifdef CYGSEM_KERNEL_EXCEPTIONS_DECODE

    for( int i = 0; i < CYGNUM_HAL_EXCEPTION_COUNT ; i++ )
        exception_handler[i] = cyg_null_exception_handler,
            exception_data[i] = 0;
#else

    exception_handler = cyg_null_exception_handler;    
    exception_data = 0;
    
#endif
    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Exception registation. Stores the handler function and data to be used
// for handling the given exception number. Where exceptions are not decoded
// only a single handler may be registered for all exceptions. This function
// also returns the old values of the exception handler and data to allow
// chaining to be implemented.

void
Cyg_Exception_Control::register_exception(
    cyg_code                exception_number,       // exception number
    cyg_exception_handler   handler,                // handler function
    CYG_ADDRWORD            data,                   // data argument
    cyg_exception_handler   **old_handler,          // handler function
    CYG_ADDRWORD            *old_data               // data argument
    )
{
    CYG_REPORT_FUNCTION();
    CYG_REPORT_FUNCARG5("exception=%d, handler func=%08x, data=%08x, "
                        "space for old handler=%08x,space for old data=%08x",
                        exception_number, handler, data, old_handler,
                        old_data);

    CYG_ASSERT( exception_number <= CYGNUM_HAL_EXCEPTION_MAX,
                "Out of range exception number");
    CYG_ASSERT( exception_number >= CYGNUM_HAL_EXCEPTION_MIN,
                "Out of range exception number");


    // Should we complain if there is already a registered
    // handler, or should we just replace is silently?
    
#ifdef CYGSEM_KERNEL_EXCEPTIONS_DECODE

    if( old_handler != NULL )
        *old_handler = exception_handler[exception_number -
                                        CYGNUM_HAL_EXCEPTION_MIN];
    if( old_data != NULL )
        *old_data = exception_data[exception_number - 
                                  CYGNUM_HAL_EXCEPTION_MIN];
    exception_handler[exception_number - CYGNUM_HAL_EXCEPTION_MIN] = handler;
    exception_data[exception_number - CYGNUM_HAL_EXCEPTION_MIN] = data;
    
#else
    
    if( old_handler != NULL )
        *old_handler = exception_handler;
    if( old_data != NULL )
        *old_data = exception_data;
    exception_handler = handler;
    exception_data = data;
    
#endif
    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Exception deregistation. Revert the handler for the exception number
// to the default.

void
Cyg_Exception_Control::deregister_exception(
    cyg_code                exception_number        // exception number
    )
{
    CYG_REPORT_FUNCTION();
    CYG_REPORT_FUNCARG1("exception number=%d", exception_number);

    CYG_ASSERT( exception_number <= CYGNUM_HAL_EXCEPTION_MAX,
                "Out of range exception number");
    CYG_ASSERT( exception_number >= CYGNUM_HAL_EXCEPTION_MIN,
                "Out of range exception number");

#ifdef CYGSEM_KERNEL_EXCEPTIONS_DECODE

    exception_handler[exception_number - CYGNUM_HAL_EXCEPTION_MIN] = 
        cyg_null_exception_handler;
    exception_data[exception_number - CYGNUM_HAL_EXCEPTION_MIN] = 0;
    
#else
    
    exception_handler = cyg_null_exception_handler;
    exception_data = 0;
    
#endif

    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Exception delivery. Call the appropriate exception handler.

void
Cyg_Exception_Control::deliver_exception(
    cyg_code            exception_number,       // exception being raised
    CYG_ADDRWORD        exception_info          // exception specific info
    )
{
    CYG_REPORT_FUNCTION();
    CYG_REPORT_FUNCARG2("exception number=%d, exception info=%08x",
                        exception_number, exception_info);

    cyg_exception_handler *handler = NULL;
    CYG_ADDRWORD data = 0;

    CYG_ASSERT( exception_number <= CYGNUM_HAL_EXCEPTION_MAX,
                "Out of range exception number");
    CYG_ASSERT( exception_number >= CYGNUM_HAL_EXCEPTION_MIN,
                "Out of range exception number");
    
#ifdef CYGSEM_KERNEL_EXCEPTIONS_DECODE

    handler = exception_handler[exception_number - CYGNUM_HAL_EXCEPTION_MIN];
    data = exception_data[exception_number - CYGNUM_HAL_EXCEPTION_MIN];
    
#else
    
    handler = exception_handler;
    data = exception_data;
    
#endif

    // The handler will always be a callable function: either the user's
    // registered function or the null handler. So it is always safe to
    // just go ahead and call it.
    
    handler( data, exception_number, exception_info );

    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Exception delivery function called from the HAL as a result of a
// hardware exception being raised.

externC void
cyg_hal_deliver_exception( CYG_WORD code, CYG_ADDRWORD data )
{
    CYG_REPORT_FUNCTION();
    Cyg_Thread::self()->deliver_exception( (cyg_code)code, data );
    CYG_REPORT_RETURN();
}

// -------------------------------------------------------------------------
// Where exceptions are global, there is a single static instance of the
// exception control object. Define it here.

#ifdef CYGSEM_KERNEL_EXCEPTIONS_GLOBAL

Cyg_Exception_Control Cyg_Thread::exception_control 
                                              CYG_INIT_PRIORITY(INTERRUPTS);

#endif

// -------------------------------------------------------------------------

#endif // ifdef CYGPKG_KERNEL_EXCEPTIONS

// -------------------------------------------------------------------------
// EOF common/except.cxx
