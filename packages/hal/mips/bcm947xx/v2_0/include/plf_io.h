#ifndef CYGONCE_PLF_IO_H
#define CYGONCE_PLF_IO_H
//=============================================================================
//
//      plf_io.h
//
//      Platform specific IO support
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    
// Contributors: 
// Date:         
// Purpose:      BCM947xx platform IO support
// Description: 
// Usage:        #include <cyg/hal/plf_io.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================
#include <pkgconf/hal.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>

//-----------------------------------------------------------------------------

__externC void cyg_hal_plf_pci_init(void);

// Initialization of the PCI bus.
#define HAL_PCI_INIT      cyg_hal_plf_pci_init


// Map PCI device resources starting from these addresses in PCI space.
#define HAL_PCI_ALLOC_BASE_MEMORY 0x08000000
#define HAL_PCI_ALLOC_BASE_IO     0x00000000

// This is where the PCI spaces are mapped in the CPU's address space.
#define HAL_PCI_PHYSICAL_MEMORY_BASE   0xA0000000
#define HAL_PCI_PHYSICAL_IO_BASE       0xA0000000


__externC cyg_uint8 cyg_hal_plf_pci_cfg_read_byte(int busNo, int devFnNo, int regOffset);
__externC cyg_uint16 cyg_hal_plf_pci_cfg_read_word(int busNo, int devFnNo, int regOffset);
__externC cyg_uint32 cyg_hal_plf_pci_cfg_read_dword(int busNo, int devFnNo, int regOffset);

__externC void cyg_hal_plf_pci_cfg_write_byte(int busNo, int devFnNo, int regOffset, cyg_uint8 data);
__externC void cyg_hal_plf_pci_cfg_write_word(int busNo, int devFnNo, int regOffset, cyg_uint16 data);
__externC void cyg_hal_plf_pci_cfg_write_dword(int busNo, int devFnNo, int regOffset, cyg_uint32 data);

// Read a value from the PCI configuration space of the appropriate
// size at an address composed from the bus, devfn and offset.
#define HAL_PCI_CFG_READ_UINT8( __bus, __devfn, __offset, __val )  \
        __val = cyg_hal_plf_pci_cfg_read_byte(__bus, __devfn, __offset)
		
		
    
#define HAL_PCI_CFG_READ_UINT16( __bus, __devfn, __offset, __val ) \
	__val = cyg_hal_plf_pci_cfg_read_word(__bus, __devfn, __offset)

#define HAL_PCI_CFG_READ_UINT32( __bus, __devfn, __offset, __val ) \
	__val = cyg_hal_plf_pci_cfg_read_dword(__bus, __devfn, __offset)

// Write a value to the PCI configuration space of the appropriate
// size at an address composed from the bus, devfn and offset.
#define HAL_PCI_CFG_WRITE_UINT8( __bus, __devfn, __offset, __val )  \
        cyg_hal_plf_pci_cfg_write_byte(__bus, __devfn, __offset, __val)

#define HAL_PCI_CFG_WRITE_UINT16( __bus, __devfn, __offset, __val ) \
	cyg_hal_plf_pci_cfg_write_word(__bus, __devfn, __offset, __val)
	   
#define HAL_PCI_CFG_WRITE_UINT32( __bus, __devfn, __offset, __val ) \
	cyg_hal_plf_pci_cfg_write_dword(__bus, __devfn, __offset, __val)



// Translate the PCI interrupt requested by the device (INTA#, INTB#)
// to the associated CPU interrupt (i.e., HAL vector).
#define HAL_PCI_TRANSLATE_INTERRUPT( __bus, __devfn, __vec, __valid) \
    CYG_MACRO_START                                                           \
    cyg_uint8 __req;                                                          \
    HAL_PCI_CFG_READ_UINT8(__bus, __devfn, CYG_PCI_CFG_INT_PIN, __req);       \
	printf("plf_io: intr req is for %d for device %d\n", __req, CYG_PCI_DEV_GET_DEV(__devfn));			\
    if (0 != __req) {                                                         \
        CYG_ADDRWORD __translation[4] = {                                     \
            CYGNUM_HAL_INTERRUPT_PCI_INTB, /* INTB# */                        \
            0,                                                                \
            CYGNUM_HAL_INTERRUPT_PCI_INTA, /* INTA# */                        \
			0,                                                                \
            };                                                                \
                                                                              \
        __vec = __translation[(((__req)+CYG_PCI_DEV_GET_DEV(__devfn))&3)]; \
        __valid = true;                                                       \
    } else {                                                                  \
        /* Device will not generate interrupt requests. */                    \
        __valid = false;                                                      \
    }                                                                         \
    CYG_MACRO_END


// end of plf_io.h
#endif // CYGONCE_PLF_IO_H
