// Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// SPDX-FileCopyrightText: 2012 Texas Instruments Incorporated
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __UART_H__
#define __UART_H__

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
// Values that can be passed to UARTIntEnable, UARTIntDisable, and UARTIntClear
// as the ui32IntFlags parameter, and returned from UARTIntStatus.
//
//*****************************************************************************
#define UART_INT_9BIT           0x1000      // 9-bit address match interrupt
#define UART_INT_OE             0x400       // Overrun Error Interrupt Mask
#define UART_INT_BE             0x200       // Break Error Interrupt Mask
#define UART_INT_PE             0x100       // Parity Error Interrupt Mask
#define UART_INT_FE             0x080       // Framing Error Interrupt Mask
#define UART_INT_RT             0x040       // Receive Timeout Interrupt Mask
#define UART_INT_TX             0x020       // Transmit Interrupt Mask
#define UART_INT_RX             0x010       // Receive Interrupt Mask
#define UART_INT_CTS            0x002       // CTS Modem Interrupt Mask (only
                                            // UART1)

//*****************************************************************************
//
// Values that can be passed to UARTConfigSetExpClk as the ui32Config parameter
// and returned by UARTConfigGetExpClk in the pui32Config parameter.
// Additionally, the UART_CONFIG_PAR_* subset can be passed to
// UARTParityModeSet as the ui32Parity parameter, and are returned by
// UARTParityModeGet.
//
//*****************************************************************************
#define UART_CONFIG_WLEN_MASK   0x00000060  // Mask for extracting word length
#define UART_CONFIG_WLEN_8      0x00000060  // 8 bit data
#define UART_CONFIG_WLEN_7      0x00000040  // 7 bit data
#define UART_CONFIG_WLEN_6      0x00000020  // 6 bit data
#define UART_CONFIG_WLEN_5      0x00000000  // 5 bit data
#define UART_CONFIG_STOP_MASK   0x00000008  // Mask for extracting stop bits
#define UART_CONFIG_STOP_ONE    0x00000000  // One stop bit
#define UART_CONFIG_STOP_TWO    0x00000008  // Two stop bits
#define UART_CONFIG_PAR_MASK    0x00000086  // Mask for extracting parity
#define UART_CONFIG_PAR_NONE    0x00000000  // No parity
#define UART_CONFIG_PAR_EVEN    0x00000006  // Even parity
#define UART_CONFIG_PAR_ODD     0x00000002  // Odd parity
#define UART_CONFIG_PAR_ONE     0x00000082  // Parity bit is one
#define UART_CONFIG_PAR_ZERO    0x00000086  // Parity bit is zero

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ui32TxLevel parameter and
// returned by UARTFIFOLevelGet in the pui32TxLevel.
//
//*****************************************************************************
#define UART_FIFO_TX1_8         0x00000000  // Transmit interrupt at 1/8 Full
#define UART_FIFO_TX2_8         0x00000001  // Transmit interrupt at 1/4 Full
#define UART_FIFO_TX4_8         0x00000002  // Transmit interrupt at 1/2 Full
#define UART_FIFO_TX6_8         0x00000003  // Transmit interrupt at 3/4 Full
#define UART_FIFO_TX7_8         0x00000004  // Transmit interrupt at 7/8 Full

//*****************************************************************************
//
// Values that can be passed to UARTFIFOLevelSet as the ui32RxLevel parameter and
// returned by UARTFIFOLevelGet in the pui32RxLevel.
//
//*****************************************************************************
#define UART_FIFO_RX1_8         0x00000000  // Receive interrupt at 1/8 Full
#define UART_FIFO_RX2_8         0x00000008  // Receive interrupt at 1/4 Full
#define UART_FIFO_RX4_8         0x00000010  // Receive interrupt at 1/2 Full
#define UART_FIFO_RX6_8         0x00000018  // Receive interrupt at 3/4 Full
#define UART_FIFO_RX7_8         0x00000020  // Receive interrupt at 7/8 Full

//*****************************************************************************
//
// Values that can be passed to UARTDMAEnable() and UARTDMADisable().
//
//*****************************************************************************
#define UART_DMA_ERR_RXSTOP     0x00000004  // Stop DMA receive if UART error
#define UART_DMA_TX             0x00000002  // Enable DMA for transmit
#define UART_DMA_RX             0x00000001  // Enable DMA for receive

//*****************************************************************************
//
// Values returned from UARTRxErrorGet().
//
//*****************************************************************************
#define UART_RXERROR_OVERRUN    0x00000008
#define UART_RXERROR_BREAK      0x00000004
#define UART_RXERROR_PARITY     0x00000002
#define UART_RXERROR_FRAMING    0x00000001

//*****************************************************************************
//
// Values that can be passed to UARTTxIntModeSet() or returned from
// UARTTxIntModeGet().
//
//*****************************************************************************
#define UART_TXINT_MODE_FIFO    0x00000000
#define UART_TXINT_MODE_EOT     0x00000010

//*****************************************************************************
//
// Values that can be passed to UARTClockSourceSet() or returned from
// UARTClockSourceGet().
//
//*****************************************************************************
#define UART_CLOCK_SYSTEM       0x00000000
#define UART_CLOCK_PIOSC        0x00000001

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void UARTParityModeSet(uint32_t ui32Base, uint32_t ui32Parity);
extern uint32_t UARTParityModeGet(uint32_t ui32Base);
extern void UARTFIFOLevelSet(uint32_t ui32Base, uint32_t ui32TxLevel,
                             uint32_t ui32RxLevel);
extern void UARTFIFOLevelGet(uint32_t ui32Base, uint32_t *pui32TxLevel,
                             uint32_t *pui32RxLevel);
extern void UARTConfigSetExpClk(uint32_t ui32Base, uint32_t ui32UARTClk,
                                uint32_t ui32Baud, uint32_t ui32Config);
extern void UARTConfigGetExpClk(uint32_t ui32Base, uint32_t ui32UARTClk,
                                uint32_t *pui32Baud,
                                uint32_t *pui32Config);
extern void UARTEnable(uint32_t ui32Base);
extern void UARTDisable(uint32_t ui32Base);
extern void UARTFIFOEnable(uint32_t ui32Base);
extern void UARTFIFODisable(uint32_t ui32Base);
extern void UARTEnableSIR(uint32_t ui32Base, bool bLowPower);
extern void UARTDisableSIR(uint32_t ui32Base);
extern bool UARTCharsAvail(uint32_t ui32Base);
extern bool UARTSpaceAvail(uint32_t ui32Base);
extern int32_t UARTCharGetNonBlocking(uint32_t ui32Base);
extern int32_t UARTCharGet(uint32_t ui32Base);
extern bool UARTCharPutNonBlocking(uint32_t ui32Base,
                                       uint8_t ui8Data);
extern void UARTCharPut(uint32_t ui32Base, uint8_t ui8Data);
extern void UARTBreakCtl(uint32_t ui32Base, bool bBreakState);
extern bool UARTBusy(uint32_t ui32Base);
extern void UARTIntRegister(uint32_t ui32Base, void (*pfnHandler)(void));
extern void UARTIntUnregister(uint32_t ui32Base);
extern void UARTIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void UARTIntDisable(uint32_t ui32Base, uint32_t ui32IntFlags);
extern uint32_t UARTIntStatus(uint32_t ui32Base, bool bMasked);
extern void UARTIntClear(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void UARTDMAEnable(uint32_t ui32Base, uint32_t ui32DMAFlags);
extern void UARTDMADisable(uint32_t ui32Base, uint32_t ui32DMAFlags);
extern uint32_t UARTRxErrorGet(uint32_t ui32Base);
extern void UARTRxErrorClear(uint32_t ui32Base);
extern void UARTTxIntModeSet(uint32_t ui32Base, uint32_t ui32Mode);
extern uint32_t UARTTxIntModeGet(uint32_t ui32Base);
extern void UARTClockSourceSet(uint32_t ui32Base, uint32_t ui32Source);
extern uint32_t UARTClockSourceGet(uint32_t ui32Base);
extern void UART9BitEnable(uint32_t ui32Base);
extern void UART9BitDisable(uint32_t ui32Base);
extern void UART9BitAddrSet(uint32_t ui32Base, uint8_t ui8Addr,
                            uint8_t ui8Mask);
extern void UART9BitAddrSend(uint32_t ui32Base, uint8_t ui8Addr);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __UART_H__
