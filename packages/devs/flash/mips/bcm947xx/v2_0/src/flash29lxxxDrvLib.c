#include <pkgconf/system.h>
#include <pkgconf/hal.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cyg/io/flashDrvLib.h>

#include <cyg/hal/osl.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#define FLASH_16BIT 1

#if defined(CYGPKG_HAL_MIPS_MSBFIRST)
#define ADDR_XOR_3 3
#else
#define ADDR_XOR_3 0
#endif

#define xor_val ADDR_XOR_3

#define FLASH_ADDR(dev, addr) \
    ((volatile cyg_uint8 *) (((int)(dev) + (int)(addr)) ^ xor_val))

#define FLASH_WRITE(dev, addr, value) \
    (*FLASH_ADDR(dev, addr) = (value))

#define FLASH_READ(dev, addr) \
    (*FLASH_ADDR(dev, addr))

extern struct flash_drv_funcs_s flash29lxxx;
//extern struct flash_drv_funcs_s flash29l320;
//extern struct flash_drv_funcs_s flash29l640;
//extern struct flash_drv_funcs_s flash29l800;

LOCAL void
flashReadReset(void)
{
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xf0);
}

LOCAL void
flashAutoSelect(FLASH_TYPES *dev, FLASH_VENDORS *vendor)
{
    flashReadReset();   
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xaa);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0x555, 0x55);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0x90);

    *vendor = FLASH_READ(FLASH_BASE_ADDRESS, 0);
    *dev = FLASH_READ(FLASH_BASE_ADDRESS, 2);

    if (flashVerbose)
        printf("flashAutoSelect(): dev = 0x%x, vendor = 0x%x\n",
               (int)*dev, (int)*vendor);
    flashReadReset();   

    if (((*dev != FLASH_2L160) &&
         (*dev != FLASH_2L320) &&
         (*dev != FLASH_2L640) &&
         (*dev != FLASH_2L800) &&
         (*dev != FLASH_29GL128)) ||
        ((*vendor != AMD) && (*vendor != ALLIANCE) && *vendor != MXIC)) {
        *vendor = *dev = 0xFF;
    }
}

LOCAL int
flashEraseDevices(volatile unsigned char *sectorBasePtr)
{
    int             i;
    unsigned int    tmp;

    if (flashVerbose) {
        printf("Erasing Sector @ 0x%08x\n",(unsigned int)sectorBasePtr);
    }
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xaa);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0x555, 0x55);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0x80);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xaa);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0x555, 0x55);
    FLASH_WRITE(sectorBasePtr, 0x0, 0x30);

    for (i = 0; i < FLASH_ERASE_TIMEOUT_COUNT; i++) {
    	   cyg_thread_delay((cyg_tick_count_t)FLASH_ERASE_TIMEOUT_TICKS);

        tmp = FLASH_READ(sectorBasePtr, 0x0);

        if ((tmp & 0x80) == 0x80) {
            if (flashVerbose > 1)
                printf("flashEraseDevices(): all devices erased\n");
            return (OK);
        }
    }

    if ((tmp & 0x20) == 0x20) {
        printf("flashEraseDevices(): addr 0x%08x erase failed\n",
           (int)sectorBasePtr);
    } else {
        printf("flashEraseDevices(): addr 0x%08x erase timed out\n",
           (int)sectorBasePtr);
    }

    flashReadReset();
    return (ERROR);
}

LOCAL int
flashEraseSector(int sectorNum)
{
    unsigned char   *sectorBasePtr =
	(unsigned char *)FLASH_SECTOR_ADDRESS(sectorNum);

    if (sectorNum < 0 || sectorNum >= flashSectorCount) {
        printf("flashEraseSector(): Sector %d invalid\n", sectorNum);
        return (ERROR);
    }

    if (flashEraseDevices(sectorBasePtr) == ERROR) {
        printf("flashEraseSector(): erase devices failed sector=%d\n",
               sectorNum);
        return (ERROR);
    }

    /* Boot Sectored devices need multiple erases in sector 0 Sector 0 is
     * broken up into 16k, 8k, 8k, 32k sub sectors. Each one must have an
     * erase issued in that sub sector to erase all of logical sector 0. */

    if (sectorNum == 0) {
        if (flashEraseDevices(sectorBasePtr + 0x4000) == ERROR) {
            printf("flashEraseSector(): erase devices failed sector=0.1\n");
            return (ERROR);
        }

        if (flashEraseDevices(sectorBasePtr + 0x6000) == ERROR) {
            printf("flashEraseSector(): erase devices failed sector=0.2\n");
            return (ERROR);
        }

        if (flashEraseDevices(sectorBasePtr + 0x8000) == ERROR) {
            printf("flashEraseSector(): erase devices failed sector=0.3\n");
            return (ERROR);
        }
    }

    if (flashVerbose)
        printf("flashEraseSector(): Sector %d erased\n", sectorNum);

    return (OK);
}

LOCAL int
flashRead(int sectorNum, char *buff, unsigned int offset, unsigned int count)
{
    if (sectorNum < 0 || sectorNum >= flashSectorCount) {
        printf("flashRead(): Illegal sector %d\n", sectorNum);
        return (ERROR);
    }

    bcopy((char *)(FLASH_SECTOR_ADDRESS(sectorNum) + offset), buff, count);

    return (0);
}


LOCAL int
#ifdef FLASH_16BIT
flashProgramDevices(volatile unsigned short *addr, unsigned short val)
#else
flashProgramDevices(volatile unsigned char *addr, unsigned char val)
#endif
{
    int             polls;
    unsigned int    tmp;

    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xaa);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0x555, 0x55);
    FLASH_WRITE(FLASH_BASE_ADDRESS, 0xaaa, 0xa0);
    /* FLASH_WRITE(addr, 0x0, val);*/
    *addr = val;

    for (polls = 0; polls < FLASH_PROGRAM_TIMEOUT_POLLS; polls++) {
        tmp = *addr;

#ifdef FLASH_16BIT
        if ((tmp & 0x8080) == (val & 0x8080)) {
#else
        if ((tmp & 0x80) == (val & 0x80)) {
#endif
            if (flashVerbose > 2)
                printf("flashProgramDevices(): devices programmed\n");
            return (OK);
        }
    }

#ifdef FLASH_16BIT
    if ((tmp & 0x2020) != 0) {
#else
    if ((tmp & 0x20) != 0) {
#endif
	/* 
	 * We've already waited so long that chances are nil that the
	 * 0x80 bits will change again.  Don't bother re-checking them.
	 */
		printf("flashProgramDevices(): Address 0x%08x program failed\n",
		       (int)addr);
    } else {
        printf("flashProgramDevices(): timed out\n");
    }

    flashReadReset();
    return (ERROR);
}

LOCAL int
flashWrite(int sectorNum, char *buff, unsigned int offset, unsigned int count)
{
#ifdef FLASH_16BIT
    unsigned short *curBuffPtr, *flashBuffPtr;
#else
    unsigned char   *curBuffPtr, *flashBuffPtr;
#endif
    int             i;

#ifdef FLASH_16BIT
    curBuffPtr = (unsigned short *)buff;
    flashBuffPtr = (unsigned short *)(FLASH_SECTOR_ADDRESS(sectorNum) + offset);

    for (i = 0; i < (count+1)/2; i++) {
#else
    curBuffPtr = (unsigned char *)buff;
    flashBuffPtr = (unsigned char *)(FLASH_SECTOR_ADDRESS(sectorNum) + offset);

    for (i = 0; i < count; i++) {
#endif
        if (flashProgramDevices(flashBuffPtr, *curBuffPtr) == ERROR) {
            printf("flashWrite(): Failed: Sector %d, address 0x%x\n",
               sectorNum, (int)flashBuffPtr);
            return (ERROR);
        }

        flashBuffPtr++;
        curBuffPtr++;
    }

    return (0);
}

struct flash_drv_funcs_s flash29lxxx = {
    FLASH_2L160, AMD,
    flashAutoSelect,
    flashEraseSector,
    flashRead,
    flashWrite
};
/** Ramgopal Remove Start **/
struct flash_drv_funcs_s flash29l320 = {
    FLASH_2L320, AMD,
    flashAutoSelect,
    flashEraseSector,
    flashRead,
    flashWrite
};

struct flash_drv_funcs_s flash29l640 = {
    FLASH_2L640, AMD,
    flashAutoSelect,
    flashEraseSector,
    flashRead,
    flashWrite
};

struct flash_drv_funcs_s flash29l800 = {
    FLASH_2L800, MXIC,
    flashAutoSelect,
    flashEraseSector,
    flashRead,
    flashWrite
};
/** Ramgopal Remove End **/
