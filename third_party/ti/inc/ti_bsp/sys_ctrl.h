// Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// SPDX-FileCopyrightText: 2012 Texas Instruments Incorporated
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __SYS_CTRL_H__
#define __SYS_CTRL_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "ti_bsp/hw/hw_types.h"

//*****************************************************************************
//
// General constants
//
//*****************************************************************************

#define SYS_CTRL_TIMEOUT           0x0000ffff // Time out value for polling

#define SYS_CTRL_32MHZ               32000000
#define SYS_CTRL_16MHZ               16000000
#define SYS_CTRL_8MHZ                 8000000
#define SYS_CTRL_4MHZ                 4000000
#define SYS_CTRL_2MHZ                 2000000
#define SYS_CTRL_1MHZ                 1000000
#define SYS_CTRL_500KHZ                500000
#define SYS_CTRL_250KHZ                250000
#define SYS_CTRL_62500HZ                62500


//*****************************************************************************
//
// The following are values that can be passed to the
// SysCtrlPeripheralReset(), SysCtrlPeripheralEnable(),
// SysCtrlPeripheralDisable(), SysCtrlPeripheralSleepEnable(),
// SysCtrlPeripheralSleepDisable(), SysCtrlPeripheralDeepSleepEnable() and
// SysCtrlPeripheralDeepSleepDisable()  APIs as the ui32Peripheral parameter.
//
//*****************************************************************************
#define SYS_CTRL_PERIPH_GPT0       0x00000000  // General Purpose Timer 0
#define SYS_CTRL_PERIPH_GPT1       0x00000001  // General Purpose Timer 1
#define SYS_CTRL_PERIPH_GPT2       0x00000002  // General Purpose Timer 2
#define SYS_CTRL_PERIPH_GPT3       0x00000003  // General Purpose Timer 3
#define SYS_CTRL_PERIPH_SSI0       0x00000100  // SSI 0
#define SYS_CTRL_PERIPH_SSI1       0x00000101  // SSI 1
#define SYS_CTRL_PERIPH_UART0      0x00000200  // UART 0
#define SYS_CTRL_PERIPH_UART1      0x00000201  // UART 1
#define SYS_CTRL_PERIPH_I2C        0x00000300  // I2C0
#define SYS_CTRL_PERIPH_PKA        0x00000400  // PKA
#define SYS_CTRL_PERIPH_AES        0x00000401  // AES
#define SYS_CTRL_PERIPH_RFC        0x00000500  // RF Core


//*****************************************************************************
//
// The following are values that can be passed to the SysCtrlClockSet() API as
// the ui32Config parameter.
//
//*****************************************************************************
#define SYS_CTRL_SYSDIV_32MHZ    0x00000000  // Sys_div for sysclk 32MHz 
#define SYS_CTRL_SYSDIV_16MHZ    0x00000001  // Sys_div for sysclk 16MHz
#define SYS_CTRL_SYSDIV_8MHZ     0x00000002  // Sys_div for sysclk  8MHz
#define SYS_CTRL_SYSDIV_4MHZ     0x00000003  // Sys_div for sysclk  4MHz
#define SYS_CTRL_SYSDIV_2MHZ     0x00000004  // Sys_div for sysclk  2MHz
#define SYS_CTRL_SYSDIV_1MHZ     0x00000005  // Sys_div for sysclk  1MHz
#define SYS_CTRL_SYSDIV_500KHZ   0x00000006  // Sys_div for sysclk  0.50MHz
#define SYS_CTRL_SYSDIV_250KHZ   0x00000007  // Sys_div for sysclk  0.25MHz


//*****************************************************************************
//
// The following are values that can be passed to the SysCtrlPowerModeSet()
// API as the ui32PowerMode parameter.
//
//*****************************************************************************
#define SYS_CTRL_PM_NOACTION     0x00000000  // No action
#define SYS_CTRL_PM_1            0x00000001  // Power Mode 1
#define SYS_CTRL_PM_2            0x00000002  // Power Mode 2
#define SYS_CTRL_PM_3            0x00000003  // Power Mode 3


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void SysCtrlClockSet(bool bOsc32k, bool bInternalOsc,
                            uint32_t ui32SysDiv);
extern uint32_t SysCtrlClockGet(void);
extern void SysCtrlIOClockSet(uint32_t ui32IODiv);
extern uint32_t SysCtrlIOClockGet(void);
extern void SysCtrlDelay(uint32_t ui32Count);
extern void SysCtrlReset(void);
extern void SysCtrlSleep(void);
extern void SysCtrlDeepSleep(void);
extern bool SysCtrlPeripheralPresent(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralReset(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralEnable(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralDisable(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralSleepEnable(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralSleepDisable(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralDeepSleepEnable(uint32_t ui32Peripheral);
extern void SysCtrlPeripheralDeepSleepDisable(uint32_t ui32Peripheral);
extern void SysCtrlPowerModeSet(uint32_t ui32PowerMode);
extern uint32_t SysCtrlPowerModeGet(void);
extern void SysCtrlClockLossDetectEnable(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SYS_CTRL_H__
