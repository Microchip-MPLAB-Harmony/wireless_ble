/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  Middleware Device Firmware Udpate Source File

  Company:
    Microchip Technology Inc.

  File Name:
    mw_dfu.c

  Summary:
    This source file implements the middleware functions necessary for handling
    device firmware updates (DFU).

  Description:
    The functions within this file provide the application user with the tools
    required to manage firmware updates. These include routines for checking
    update integrity, initiating the update process, and applying firmware patches.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "peripheral/nvm/plib_nvm.h"
#include "mba_error_defs.h"
#include "mw_dfu.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define MW_DFU_FW_START_ADDR                   0x01080000UL         // Start address of the firmware in the device's flash memory.
#define MW_DFU_FW_PAGE_SIZE                    0x1000U              // The size of a page in the device's flash memory.
#define MW_DFU_FW_QUAD_WORD_SIZE               0x10U                // The size of a quad word in the device's flash memory.

/* Enumeration of the different states in the DFU (Device Firmware Update) process. */
typedef enum MW_DFU_State_T
{
    MW_DFU_STATE_IDLE=0X00U,                                        // DFU process is idle.
    MW_DFU_STATE_CONFIG,                                            // DFU process is in configuration state.
    MW_DFU_STATE_FW_START,                                          // DFU process has started firmware update.
    MW_DFU_STATE_FW_UPDATE                                          // DFU process is updating firmware.
}MW_DFU_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static uint8_t          s_dfuState;                                 // Current state of the DFU process.
static MW_DFU_Info_T    s_dfuSizeInfo;                              // Structure holding size information for the DFU process.
static uint32_t         s_dfuAddr;                                  // Current address pointer for the DFU process.
static uint32_t         *sp_dfuIdent;                               // Pointer to an identifier used in the DFU process.


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Configures the device information for the Device Firmware Update (DFU) process.
 *
 * @param[in] p_dfuInfo            Pointer to the structure containing the DFU image size information. 
                                    Refer to @ref MW_DFU_Info_T for the structure's detailed information.
 *
 *
 * @retval MBA_RES_SUCCESS         Configuration successful.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The size exceeds the maximum allowable size, or the size is not aligned to a 16-byte boundary.
 */
uint16_t MW_DFU_Config(MW_DFU_Info_T * p_dfuInfo)
{
    if ((p_dfuInfo->fwImageSize > MW_DFU_MAX_SIZE_FW_IMAGE) || (p_dfuInfo->fwImageSize == 0U)
        || ((p_dfuInfo->fwImageSize & (MW_DFU_FW_QUAD_WORD_SIZE-1U)) != 0U))
    {
        return MBA_RES_INVALID_PARA;
    }

    s_dfuSizeInfo = *p_dfuInfo;

    s_dfuState = MW_DFU_STATE_CONFIG;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Starts or restarts the firmware image update procedure.
 * @note  Calling this API resets the state machine and parameters of the DFU module.
 *
 *
 * @retval MBA_RES_SUCCESS         Firmware image update procedure started or restarted successfully.
 * @retval MBA_RES_BAD_STATE       The API cannot be executed in the current state of the DFU process.
 */
uint16_t MW_DFU_FwImageStart(void)
{
    if (s_dfuState == (uint8_t)MW_DFU_STATE_IDLE)
    {
        return MBA_RES_BAD_STATE;
    }

    s_dfuAddr = MW_DFU_FW_START_ADDR;
    s_dfuState = MW_DFU_STATE_FW_START;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Updates a fragment of the firmware image in flash memory.
 * @note  This API should be called multiple times to update all fragments of the firmware image.
 *
 * @param[in] length               The length of the image fragment to update, in bytes. Must be aligned to 16 bytes.
 * @param[in] p_content            Pointer to the image fragment data.
 *
 *
 * @retval MBA_RES_SUCCESS         Fragment of firmware image updated successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The length exceeds the image size, the maximum block length, 
 *                                  or is not 16-byte aligned.
 * @retval MBA_RES_BAD_STATE       The API cannot be executed in the current state of the DFU process.
 * @retval MBA_RES_OOM             Out of memory error.
 * @retval MBA_RES_FAIL            Failed to update the fragment to flash memory.
 */
uint16_t MW_DFU_FwImageUpdate(uint16_t length, uint8_t *p_content)
{
    uint32_t * p_data;
    uint32_t addr;

    if ((s_dfuState != (uint8_t)MW_DFU_STATE_FW_START) && (s_dfuState != (uint8_t)MW_DFU_STATE_FW_UPDATE))
    {
        return MBA_RES_BAD_STATE;
    }

    /* Check if image content length and offset are legal */
    if ((s_dfuAddr + length > MW_DFU_FW_START_ADDR + s_dfuSizeInfo.fwImageSize) || (length > MW_DFU_MAX_BLOCK_LEN)
        || ((length & (MW_DFU_FW_QUAD_WORD_SIZE-1U)) != 0U) || (length == 0U))
    {
        return MBA_RES_INVALID_PARA;
    }

    /* make sure nvm is not busy now */
    while(NVM_IsBusy()==true)
    {
        
    }

    /* if start from page boundary or write cross page, erase new page */
    if ((s_dfuAddr & (MW_DFU_FW_PAGE_SIZE - 1U)) == 0U 
    || (s_dfuAddr & ~(MW_DFU_FW_PAGE_SIZE - 1U)) < ((s_dfuAddr + length - 1U)  & ~(MW_DFU_FW_PAGE_SIZE - 1U)))
    {
        if (!NVM_PageErase((s_dfuAddr + length - 1U) & ~(MW_DFU_FW_PAGE_SIZE - 1U)))
        {
            return MBA_RES_FAIL;
        }

        while(NVM_IsBusy()==true)
        {

        }
        
        if (NVM_ErrorGet() != NVM_ERROR_NONE)
        {
            return MBA_RES_FAIL;
        }
    }

    
    p_data = (uint32_t *)p_content;
    addr = s_dfuAddr; 

    /* backup first 16 bytes */
    if (s_dfuAddr == MW_DFU_FW_START_ADDR)
    {
        if (sp_dfuIdent == NULL)
        {
            sp_dfuIdent = OSAL_Malloc(16);
            if (sp_dfuIdent == NULL)
            {
                return MBA_RES_OOM;
            }
        }
        
        (void)memcpy(sp_dfuIdent, p_data, 16);

        addr += MW_DFU_FW_QUAD_WORD_SIZE;
        p_data += 4;
    }
    
    for (;addr < s_dfuAddr + length; addr += MW_DFU_FW_QUAD_WORD_SIZE)
    {
        if(!NVM_QuadWordWrite(p_data, addr))
        {
            return MBA_RES_FAIL;
        }

        while(NVM_IsBusy()==true)
        {

        }

        if (NVM_ErrorGet() != NVM_ERROR_NONE)
        {
            return MBA_RES_FAIL;
        }

        p_data += 4;
    }

    s_dfuAddr = addr;

    s_dfuState = MW_DFU_STATE_FW_UPDATE;

    /* update complete, write first 16 bytes */
    if (s_dfuAddr == MW_DFU_FW_START_ADDR + s_dfuSizeInfo.fwImageSize)
    {
        if(!NVM_QuadWordWrite(sp_dfuIdent, MW_DFU_FW_START_ADDR))
        {
            return MBA_RES_FAIL;
        }

        while(NVM_IsBusy()==true)
        {
        }

        if (NVM_ErrorGet() != NVM_ERROR_NONE)
        {
            return MBA_RES_FAIL;
        }

        OSAL_Free(sp_dfuIdent);
        sp_dfuIdent = NULL;
    }

    return MBA_RES_SUCCESS;
}


/**
 * @brief Reads a fragment of the firmware image from flash memory.
 * @note  The first 16 bytes of the image fragment read from flash are invalid until the full firmware image is updated.
 *
 *
 * @param[in] offset               The offset in flash memory from where to start reading the image fragment.
 * @param[in] length               The length of the image fragment to read, in bytes.
 * @param[in] p_content            Pointer to the buffer where the image fragment will be stored.
 *
 *
 * @retval MBA_RES_SUCCESS         Fragment of firmware image read successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The sum of offset and length exceeds the maximum firmware image size, 
 *                                  or the length exceeds the maximum block length.
 */
uint16_t MW_DFU_FwImageRead(uint32_t offset, uint16_t length, uint8_t *p_content)
{
    //Check read range 
    if ((offset + length > MW_DFU_MAX_SIZE_FW_IMAGE) || (length > MW_DFU_MAX_BLOCK_LEN))
    {
        return MBA_RES_INVALID_PARA;
    }

    while(NVM_IsBusy()==true)
    {
    }

    if (!NVM_Read((uint32_t *)p_content, length, MW_DFU_FW_START_ADDR + offset))
    {
        return MBA_RES_FAIL;
    }
    
    return MBA_RES_SUCCESS;
}
