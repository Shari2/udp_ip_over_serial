// Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// SPDX-FileCopyrightText: 2012 Texas Instruments Incorporated
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __AES_H__
#define __AES_H__

#include "ti_bsp/hw/hw_types.h"
#include "ti_bsp/hw/hw_aes.h"
#include "ti_bsp/interrupt.h"
#include "ti_bsp/hw/hw_ints.h"

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

//*****************************************************************************
//
//  General constants
//
//*****************************************************************************

// AES and SHA256 module return codes
#define AES_SUCCESS                   0
#define SHA256_SUCCESS                0
#define AES_KEYSTORE_READ_ERROR       1
#define AES_KEYSTORE_WRITE_ERROR      2
#define AES_DMA_BUS_ERROR             3
#define CCM_AUTHENTICATION_FAILED     4
#define SHA2_ERROR                    5
#define SHA256_INVALID_PARAM          6
#define SHA256_TEST_ERROR             7
#define AES_ECB_TEST_ERROR            8
#define AES_NULL_ERROR                9
#define SHA256_NULL_ERROR             9    
#define AES_CCM_TEST_ERROR            10

// Key store module defines
#define STATE_BLENGTH   16      // Number of bytes in State 
#define KEY_BLENGTH     16      // Number of bytes in Key 
#define KEY_EXP_LENGTH  176     // Nb * (Nr+1) * 4 
#define KEY_STORE_SIZE_BITS  0x03UL
#define KEY_STORE_SIZE_NA    0x00UL
#define KEY_STORE_SIZE_128   0x01UL
#define KEY_STORE_SIZE_192   0x02UL
#define KEY_STORE_SIZE_256   0x03UL

// AES module defines
#define AES_BUSY    0x08
#define ENCRYPT     0x00
#define DECRYPT     0x01

// Defines for setting the mode of the AES operation
#define ECB         0x1FFFFFE0
#define CCM         0x00040000

// Macro for setting the mode of the AES operation 
#define AES_SETMODE_ECB do { HWREG(AES_AES_CTRL) &= ~ECB; } while (0)
#define AES_SETMODE(mode) do { HWREG(AES_AES_CTRL) &= ~mode; HW_REG(AES_AES_CTRL) |= mode} while (0)

// Macro for MIN  
#define MIN(n,m)   (((n) < (m)) ? (n) : (m))

//ASM NOP in ccs and IAR
#ifdef ccs
#define ASM_NOP    asm(" NOP")
#elif defined rvmdk
#define ASM_NOP   __nop()
#else
#define ASM_NOP    asm("NOP")
#endif

//*****************************************************************************
//
//For 128 bit key all 8 Key Area locations from 0 to 8 are valid
//However for 192 bit and 256 bit keys, only even Key Areas 0, 2, 4, 6 
//are valid. This is passes as a parameter to AesECBStart()
//
//*****************************************************************************
enum 
{
    KEY_AREA_0,        
    KEY_AREA_1,          
    KEY_AREA_2,       
    KEY_AREA_3,       
    KEY_AREA_4,
    KEY_AREA_5,       
    KEY_AREA_6,  
    KEY_AREA_7 
};

// Current AES operation
enum
{
    AES_NONE,        
    AES_KEYL0AD,          
    AES_ECB,       
    AES_CCM,       
    AES_SHA256,
    AES_RNG 
};

// Variable that holds the current AES operation
extern volatile uint8_t g_ui8CurrentAESOp;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
// AES and Keystore functions
extern uint8_t AESLoadKey(uint8_t *pui8Key, uint8_t ui8KeyLocation);
extern uint8_t AESECBStart(uint8_t *pui8MsgIn, 
                           uint8_t *pui8MsgOut, 
                           uint8_t ui8KeyLocation,
                           uint8_t ui8Encrypt, 
                           uint8_t ui8IntEnable);
extern uint8_t AESECBCheckResult(void);
extern uint8_t AESECBGetResult(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // __AES_H__
