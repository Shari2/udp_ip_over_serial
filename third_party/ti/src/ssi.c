// Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// SPDX-FileCopyrightText: 2012 Texas Instruments Incorporated
//
// SPDX-License-Identifier: BSD-3-Clause

//*****************************************************************************
//
//! \addtogroup ssi_api
//! @{
//
//*****************************************************************************

#include "ti_bsp/hw/hw_ints.h"
#include "ti_bsp/hw/hw_memmap.h"
#include "ti_bsp/hw/hw_ssi.h"
#include "ti_bsp/debug.h"
#include "ti_bsp/interrupt.h"
#include "ti_bsp/ssi.h"

//*****************************************************************************
//
//! Configures the synchronous serial interface
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32SSIClk is the rate of the clock supplied to the SSI module.
//! \param ui32Protocol specifies the data transfer protocol.
//! \param ui32Mode specifies the mode of operation.
//! \param ui32BitRate specifies the clock rate.
//! \param ui32DataWidth specifies number of bits transferred per frame.
//!
//! This function configures the synchronous serial interface.  It sets
//! the SSI protocol, mode of operation, bit rate, and data width.
//!
//! The \e ui32Protocol parameter defines the data frame format.  The
//! \e ui32Protocol parameter can be one of the following values:
//! \b SSI_FRF_MOTO_MODE_0, \b SSI_FRF_MOTO_MODE_1, \b SSI_FRF_MOTO_MODE_2,
//! \b SSI_FRF_MOTO_MODE_3, \b SSI_FRF_TI, or \b SSI_FRF_NMW.  The Motorola
//! frame formats imply the following polarity and phase configurations:
//!
//! <pre>
//! Polarity Phase       Mode
//!   0       0   SSI_FRF_MOTO_MODE_0
//!   0       1   SSI_FRF_MOTO_MODE_1
//!   1       0   SSI_FRF_MOTO_MODE_2
//!   1       1   SSI_FRF_MOTO_MODE_3
//! </pre>
//!
//! The \e ui32Mode parameter defines the operating mode of the SSI module.  The
//! SSI module can operate as a master or slave; if a slave, the SSI can be
//! configured to disable output on its serial output line.  The \e ui32Mode
//! parameter can be one of the following values: \b SSI_MODE_MASTER,
//! \b SSI_MODE_SLAVE, or \b SSI_MODE_SLAVE_OD.
//!
//! The \e ui32BitRate parameter defines the bit rate for the SSI.  This bit rate
//! must satisfy the following clock ratio criteria:
//!
//! - FSSI >= 2 * bit rate (master mode)
//! - FSSI >= 12 * bit rate (slave modes)
//!
//! where FSSI is the frequency of the clock supplied to the SSI module.
//!
//! The \e ui32DataWidth parameter defines the width of the data transfers, and
//! can be a value between 4 and 16, inclusive.
//!
//! The peripheral clock is set in the System Control module.  The frequency of
//! the system clock is the value returned by SysCtrlClockGet() or
//! SysCtrlIOClockGet() depending on the chosen clock source as set by
//! SSIClockSourceSet(), or it can be explicitly hard coded if it is constant
//! and known (to save the code/execution overhead of a call to
//! SysCtrlClockGet() or SysCtrlIOClockGet()).
//!
//! \return None
//
//*****************************************************************************
void
SSIConfigSetExpClk(uint32_t ui32Base, uint32_t ui32SSIClk,
                   uint32_t ui32Protocol, uint32_t ui32Mode,
                   uint32_t ui32BitRate, uint32_t ui32DataWidth)
{
    uint32_t ui32MaxBitRate;
    uint32_t ui32RegVal;
    uint32_t ui32PreDiv;
    uint32_t ui32SCR;
    uint32_t ui32SPH_SPO;

    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));
    ASSERT((ui32Protocol == SSI_FRF_MOTO_MODE_0) ||
           (ui32Protocol == SSI_FRF_MOTO_MODE_1) ||
           (ui32Protocol == SSI_FRF_MOTO_MODE_2) ||
           (ui32Protocol == SSI_FRF_MOTO_MODE_3) ||
           (ui32Protocol == SSI_FRF_TI) ||
           (ui32Protocol == SSI_FRF_NMW));
    ASSERT((ui32Mode == SSI_MODE_MASTER) ||
           (ui32Mode == SSI_MODE_SLAVE) ||
           (ui32Mode == SSI_MODE_SLAVE_OD));
    ASSERT(((ui32Mode == SSI_MODE_MASTER) && (ui32BitRate <= (ui32SSIClk / 2))) ||
           ((ui32Mode != SSI_MODE_MASTER) && (ui32BitRate <= (ui32SSIClk / 12))));
    ASSERT((ui32SSIClk / ui32BitRate) <= (254 * 256));
    ASSERT((ui32DataWidth >= 4) && (ui32DataWidth <= 16));

    //
    // Set the mode.
    //
    ui32RegVal = (ui32Mode == SSI_MODE_SLAVE_OD) ? SSI_CR1_SOD : 0;
    ui32RegVal |= (ui32Mode == SSI_MODE_MASTER) ? 0 : SSI_CR1_MS;
    HWREG(ui32Base + SSI_O_CR1) = ui32RegVal;

    //
    // Set the clock predivider.
    //
    ui32MaxBitRate = ui32SSIClk / ui32BitRate;
    ui32PreDiv = 0;
    do
    {
        ui32PreDiv += 2;
        ui32SCR = (ui32MaxBitRate / ui32PreDiv) - 1;
    }
    while(ui32SCR > 255);
    HWREG(ui32Base + SSI_O_CPSR) = ui32PreDiv;

    //
    // Set protocol and clock rate.
    //
    ui32SPH_SPO = (ui32Protocol & 3) << 6;
    ui32Protocol &= SSI_CR0_FRF_M;
    ui32RegVal = (ui32SCR << 8) | ui32SPH_SPO | ui32Protocol | (ui32DataWidth - 1);
    HWREG(ui32Base + SSI_O_CR0) = ui32RegVal;
}

//*****************************************************************************
//
//! Enables the synchronous serial interface
//!
//! \param ui32Base specifies the SSI module base address.
//!
//! This function enables operation of the synchronous serial interface.  The
//! synchronous serial interface must be configured before it is enabled.
//!
//! \return None
//
//*****************************************************************************
void
SSIEnable(uint32_t ui32Base)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Read-modify-write the enable bit.
    //
    HWREG(ui32Base + SSI_O_CR1) |= SSI_CR1_SSE;
}

//*****************************************************************************
//
//! Disables the synchronous serial interface
//!
//! \param ui32Base specifies the SSI module base address.
//!
//! This function disables operation of the synchronous serial interface.
//!
//! \return None
//
//*****************************************************************************
void
SSIDisable(uint32_t ui32Base)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Read-modify-write the enable bit.
    //
    HWREG(ui32Base + SSI_O_CR1) &= ~(SSI_CR1_SSE);
}

//*****************************************************************************
//
//! Registers an interrupt handler for the synchronous serial interface
//!
//! \param ui32Base specifies the SSI module base address.
//! \param pfnHandler is a pointer to the function to be called when the
//! synchronous serial interface interrupt occurs.
//!
//! This sets the handler to be called when an SSI interrupt
//! occurs.  This will enable the global interrupt in the interrupt controller;
//! specific SSI interrupts must be enabled via SSIIntEnable().  If necessary,
//! it is the interrupt handler's responsibility to clear the interrupt source
//! via SSIIntClear().
//!
//! \sa See IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None
//
//*****************************************************************************
void
SSIIntRegister(uint32_t ui32Base, void (*pfnHandler)(void))
{
    uint32_t ui32Int;

    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Determine the interrupt number based on the SSI port.
    //
    ui32Int = (ui32Base == SSI0_BASE) ? INT_SSI0 : INT_SSI1;

    //
    // Register the interrupt handler, returning an error if an error occurs.
    //
    IntRegister(ui32Int, pfnHandler);

    //
    // Enable the synchronous serial interface interrupt.
    //
    IntEnable(ui32Int);
}

//*****************************************************************************
//
//! Unregisters an interrupt handler for the synchronous serial interface
//!
//! \param ui32Base specifies the SSI module base address.
//!
//! This function will clear the handler to be called when a SSI
//! interrupt occurs.  This will also mask off the interrupt in the interrupt
//! controller so that the interrupt handler no longer is called.
//!
//! \sa See IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None
//
//*****************************************************************************
void
SSIIntUnregister(uint32_t ui32Base)
{
    uint32_t ui32Int;

    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Determine the interrupt number based on the SSI port.
    //
    ui32Int = (ui32Base == SSI0_BASE) ? INT_SSI0 : INT_SSI1;

    //
    // Disable the interrupt.
    //
    IntDisable(ui32Int);

    //
    // Unregister the interrupt handler.
    //
    IntUnregister(ui32Int);
}

//*****************************************************************************
//
//! Enables individual SSI interrupt sources
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32IntFlags is a bit mask of the interrupt sources to be enabled.
//!
//! Enables the indicated SSI interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.  The \e ui32IntFlags parameter can be any of the
//! \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, or \b SSI_RXOR values.
//!
//! \return None
//
//*****************************************************************************
void
SSIIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Enable the specified interrupts.
    //
    HWREG(ui32Base + SSI_O_IM) |= ui32IntFlags;
}

//*****************************************************************************
//
//! Disables individual SSI interrupt sources
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32IntFlags is a bit mask of the interrupt sources to be disabled.
//!
//! Disables the indicated SSI interrupt sources.  The \e ui32IntFlags parameter
//! can be any of the \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, or \b SSI_RXOR
//! values.
//!
//! \return None
//
//*****************************************************************************
void
SSIIntDisable(uint32_t ui32Base, uint32_t ui32IntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Disable the specified interrupts.
    //
    HWREG(ui32Base + SSI_O_IM) &= ~(ui32IntFlags);
}

//*****************************************************************************
//
//! Gets the current interrupt status
//!
//! \param ui32Base specifies the SSI module base address.
//! \param bMasked is \b false if the raw interrupt status is required or
//! \b true if the masked interrupt status is required.
//!
//! This function returns the interrupt status for the SSI module.  Either the
//! raw interrupt status or the status of interrupts that are allowed to
//! reflect to the processor can be returned.
//!
//! \return The current interrupt status, enumerated as a bit field of
//! \b SSI_TXFF, \b SSI_RXFF, \b SSI_RXTO, and \b SSI_RXOR.
//
//*****************************************************************************
uint32_t
SSIIntStatus(uint32_t ui32Base, bool bMasked)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Return either the interrupt status or the raw interrupt status as
    // requested.
    //
    if(bMasked)
    {
        return(HWREG(ui32Base + SSI_O_MIS));
    }
    else
    {
        return(HWREG(ui32Base + SSI_O_RIS));
    }
}

//*****************************************************************************
//
//! Clears SSI interrupt sources
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32IntFlags is a bit mask of the interrupt sources to be cleared.
//!
//! The specified SSI interrupt sources are cleared so that they no longer
//! assert.  This function must be called in the interrupt handler to keep the
//! interrupts from being recognized again immediately upon exit.  The
//! \e ui32IntFlags parameter can consist of either or both the \b SSI_RXTO and
//! \b SSI_RXOR values.
//!
//! \note Because there is a write buffer in the Cortex-M3 processor, it may
//! take several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (because the interrupt controller still sees
//! the interrupt source asserted).
//!
//! \return None
//
//*****************************************************************************
void
SSIIntClear(uint32_t ui32Base, uint32_t ui32IntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Clear the requested interrupt sources.
    //
    HWREG(ui32Base + SSI_O_ICR) = ui32IntFlags;
}

//*****************************************************************************
//
//! Puts a data element into the SSI transmit FIFO
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32Data is the data to be transmitted over the SSI interface.
//!
//! This function places the supplied data into the transmit FIFO of the
//! specified SSI module.
//!
//! \note The upper 32 - N bits of the \e ui32Data are discarded by the hardware,
//! where N is the data width as configured by SSIConfigSetExpClk().  For
//! example, if the interface is configured for 8-bit data width, the upper 24
//! bits of \e ui32Data are discarded.
//!
//! \return None
//
//*****************************************************************************
void
SSIDataPut(uint32_t ui32Base, uint32_t ui32Data)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));
    ASSERT((ui32Data & (0xfffffffe << (HWREG(ui32Base + SSI_O_CR0) &
                                       SSI_CR0_DSS_M))) == 0);

    //
    // Wait until there is space.
    //
    while(!(HWREG(ui32Base + SSI_O_SR) & SSI_SR_TNF))
    {
    }

    //
    // Write the data to the SSI.
    //
    HWREG(ui32Base + SSI_O_DR) = ui32Data;
}

//*****************************************************************************
//
//! Puts a data element into the SSI transmit FIFO
//!
//! \param ui32Base specifies the SSI module base address.
//! \param ui32Data is the data to be transmitted over the SSI interface.
//!
//! This function places the supplied data into the transmit FIFO of the
//! specified SSI module.  If there is no space in the FIFO, then this function
//! returns a zero.
//!
//! \note The upper 32 - N bits of the \e ui32Data are discarded by the hardware,
//! where N is the data width as configured by SSIConfigSetExpClk().  For
//! example, if the interface is configured for 8-bit data width, the upper 24
//! bits of \e ui32Data are discarded.
//!
//! \return Returns the number of elements written to the SSI transmit FIFO.
//
//*****************************************************************************
int32_t
SSIDataPutNonBlocking(uint32_t ui32Base, uint32_t ui32Data)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));
    ASSERT((ui32Data & (0xfffffffe << (HWREG(ui32Base + SSI_O_CR0) &
                                       SSI_CR0_DSS_M))) == 0);

    //
    // Check for space to write.
    //
    if(HWREG(ui32Base + SSI_O_SR) & SSI_SR_TNF)
    {
        HWREG(ui32Base + SSI_O_DR) = ui32Data;
        return(1);
    }
    else
    {
        return(0);
    }
}

//*****************************************************************************
//
//! Gets a data element from the SSI receive FIFO
//!
//! \param ui32Base specifies the SSI module base address.
//! \param pui32Data is a pointer to a storage location for data that was
//! received over the SSI interface.
//!
//! This function gets received data from the receive FIFO of the specified
//! SSI module and places that data into the location specified by the
//! \e pui32Data parameter.
//!
//! \note Only the lower N bits of the value written to \e pui32Data contain
//! valid data, where N is the data width as configured by
//! SSIConfigSetExpClk().  For example, if the interface is configured for
//! 8-bit data width, only the lower 8 bits of the value written to \e pui32Data
//! contain valid data.
//!
//! \return None
//
//*****************************************************************************
void
SSIDataGet(uint32_t ui32Base, uint32_t *pui32Data)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Wait until there is data to be read.
    //
    while(!(HWREG(ui32Base + SSI_O_SR) & SSI_SR_RNE))
    {
    }

    //
    // Read data from SSI.
    //
    *pui32Data = HWREG(ui32Base + SSI_O_DR);
}

//*****************************************************************************
//
//! Gets a data element from the SSI receive FIFO
//!
//! \param ui32Base specifies the SSI module base address.
//! \param pui32Data is a pointer to a storage location for data that was
//! received over the SSI interface.
//!
//! This function gets received data from the receive FIFO of the specified SSI
//! module and places that data into the location specified by the \e ui32Data
//! parameter.  If there is no data in the FIFO, then this function returns a
//! zero.
//!
//! \note Only the lower N bits of the value written to \e pui32Data contain
//! valid data, where N is the data width as configured by
//! SSIConfigSetExpClk().  For example, if the interface is configured for
//! 8-bit data width, only the lower 8 bits of the value written to \e pui32Data
//! contain valid data.
//!
//! \return Returns the number of elements read from the SSI receive FIFO.
//
//*****************************************************************************
int32_t
SSIDataGetNonBlocking(uint32_t ui32Base, uint32_t *pui32Data)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Check for data to read.
    //
    if(HWREG(ui32Base + SSI_O_SR) & SSI_SR_RNE)
    {
        *pui32Data = HWREG(ui32Base + SSI_O_DR);
        return(1);
    }
    else
    {
        return(0);
    }
}

//*****************************************************************************
//
//! Enable SSI DMA operation
//!
//! \param ui32Base is the base address of the SSI port.
//! \param ui32DMAFlags is a bit mask of the DMA features to enable.
//!
//! The specified SSI DMA features are enabled.  The SSI can be
//! configured to use DMA for transmit and/or receive data transfers.
//! The \e ui32DMAFlags parameter is the logical OR of any of the following
//! values:
//!
//! - SSI_DMA_RX - enable DMA for receive
//! - SSI_DMA_TX - enable DMA for transmit
//!
//! \note The uDMA controller must also be set up before DMA can be used
//! with the SSI.
//!
//! \return None
//
//*****************************************************************************
void
SSIDMAEnable(uint32_t ui32Base, uint32_t ui32DMAFlags)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Set the requested bits in the SSI DMA control register.
    //
    HWREG(ui32Base + SSI_O_DMACTL) |= ui32DMAFlags;
}

//*****************************************************************************
//
//! Disable SSI DMA operation
//!
//! \param ui32Base is the base address of the SSI port.
//! \param ui32DMAFlags is a bit mask of the DMA features to disable.
//!
//! This function is used to disable SSI DMA features that were enabled
//! by SSIDMAEnable().  The specified SSI DMA features are disabled.  The
//! \e ui32DMAFlags parameter is the logical OR of any of the following values:
//!
//! - SSI_DMA_RX - disable DMA for receive
//! - SSI_DMA_TX - disable DMA for transmit
//!
//! \return None
//
//*****************************************************************************
void
SSIDMADisable(uint32_t ui32Base, uint32_t ui32DMAFlags)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Clear the requested bits in the SSI DMA control register.
    //
    HWREG(ui32Base + SSI_O_DMACTL) &= ~ui32DMAFlags;
}

//*****************************************************************************
//
//! Determines whether the SSI transmitter is busy or not
//!
//! \param ui32Base is the base address of the SSI port.
//!
//! Allows the caller to determine whether all transmitted bytes have cleared
//! the transmitter hardware.  If \b false is returned, then the transmit FIFO
//! is empty and all bits of the last transmitted word have left the hardware
//! shift register.
//!
//! \return Returns \b true if the SSI is transmitting or \b false if all
//! transmissions are complete.
//
//*****************************************************************************
bool
SSIBusy(uint32_t ui32Base)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Determine if the SSI is busy.
    //
    return((HWREG(ui32Base + SSI_O_SR) & SSI_SR_BSY) ? true : false);
}

//*****************************************************************************
//
//! Sets the data clock source for the specified SSI peripheral
//!
//! \param ui32Base is the base address of the SSI port.
//! \param ui32Source is the baud clock source for the SSI.
//!
//! This function allows the baud clock source for the SSI to be selected.
//! The possible clock source are the system clock (\b SSI_CLOCK_SYSTEM) or
//! the precision internal oscillator (\b SSI_CLOCK_PIOSC), i.e. the IO clock
//! in the SysCtrl.
//! If \b SSI_CLOCK_SYSTEM is chosen, the IO clock frequency must thus be
//! queried by SysCtrlClockSet().
//! If \b SSI_CLOCK_PIOSC the SysCtrlIOClockSet() function must be used.
//!
//! Changing the baud clock source will change the data rate generated by the
//! SSI.  Therefore, the data rate should be reconfigured after any change to
//! the SSI clock source.
//!
//! \return None
//
//*****************************************************************************
void
SSIClockSourceSet(uint32_t ui32Base, uint32_t ui32Source)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));
    ASSERT((ui32Source == SSI_CLOCK_SYSTEM) || (ui32Source == SSI_CLOCK_PIOSC));

    //
    // Set the SSI clock source.
    //
    HWREG(ui32Base + SSI_O_CC) = ui32Source;
}

//*****************************************************************************
//
//! Gets the data clock source for the specified SSI peripheral
//!
//! \param ui32Base is the base address of the SSI port.
//!
//! This function returns the data clock source for the specified SSI.  The
//! possible data clock source are the system clock (\b SSI_CLOCK_SYSTEM) or
//! the precision internal oscillator (\b SSI_CLOCK_PIOSC).
//!
//! \return None
//
//*****************************************************************************
uint32_t
SSIClockSourceGet(uint32_t ui32Base)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Base == SSI0_BASE) || (ui32Base == SSI1_BASE));

    //
    // Return the SSI clock source.
    //
    return(HWREG(ui32Base + SSI_O_CC));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
