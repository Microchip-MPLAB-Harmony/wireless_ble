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
#include "configuration.h"
#include "osal/osal_freertos.h"
#include "peripheral/nvm/plib_nvm.h"
#include "peripheral/dmac/plib_dmac.h"
#include "mba_error_defs.h"
#include "mw_dfu.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define MW_DFU_FW_EFLASH_0_START_ADDR          0x01000000UL         // Start address of the first embedded flash memory for firmware.
#define MW_DFU_FW_EFLASH_1_START_ADDR_1M       0x01080000UL         // Start address of the second embedded flash memory (1MB) for firmware.
#define MW_DFU_FW_EFLASH_1_START_ADDR_2M       0x01100000UL         // Start address of the second embedded flash memory (2MB) for firmware.
#define MW_DFU_FW_PAGE_SIZE                    0x1000U              // The size of a flash memory page.
#define MW_DFU_FW_ALIGN_SIZE                   0x20U                // The size of alignment used in firmware.

#define MW_DFU_META_CHR_OFFSET_V3_272B         0x18U                // Offset for coherence in metadata version 3 (272-bit width flash).
#define MW_DFU_META_SEQ_OFFSET_V3_272B         0x3CU                // Offset for the sequence number in metadata version 3 (272-bit width flash).
#define MW_DFU_META_SRC_OFFSET_V3_272B         0x4CU                // Offset for source address in metadata version 3 (272-bit width flash).
#define MW_DFU_META_LEN_OFFSET_V3_272B         0x54U                // Offset for length in metadata version 3 (272-bit width flash).
#define MW_DFU_CRC_OFFSET_V3_272B              0x40U                // Offset for CRC in metadata version 3 (272-bit width flash).

#define MW_DFU_FW_OFFSET                       0x200U               // Offset for the firmware.

#define MISCDIAG (*((volatile unsigned int *) (0x44000440) ))       // A volatile pointer to a memory-mapped miscellaneous diagnostics register.
#define MW_DFU_DEVSEL_PFM_1M                   0x00000004           // To indicate 1M embedded flash memory in DEVSEL.

#define MW_DFU_IDENT_SIZE                      0x20U                // Size of the DFU identifier.

#define MW_DFU_HDR_COHERENCE                   0x4D434850UL         // Coherence check value for the DFU header.

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
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Function pointer type for starting a firmware operation. */
typedef uint16_t (*MW_DFU_FwOpStart)(void);

/* Function pointer type for updating firmware with a data chunk. */
typedef uint16_t (*MW_DFU_FwOpUpdate)(uint16_t length, uint8_t *p_content);

/* Function pointer type for validating the firmware operation. */
typedef uint16_t (*MW_DFU_FwOpValiate)(uint16_t cfmValue);

/* Function pointer type for activating the firmware. */
typedef uint16_t (*MW_DFU_FwOpActivate)();

/* Function pointer type for reading firmware data from a given offset. */
typedef uint16_t (*MW_DFU_FwOpRead)(uint32_t offset, uint16_t length, uint8_t *p_content);

/* Structure containing function pointers to firmware operation handlers.*/
typedef struct MW_DFU_FwOperation_T
{
    MW_DFU_FwOpStart        start;                  // Pointer to the start operation function.
    MW_DFU_FwOpUpdate       update;                 // Pointer to the update operation function.
    MW_DFU_FwOpValiate      validate;               // Pointer to the validate operation function.
    MW_DFU_FwOpActivate     activate;               // Pointer to the activate operation function.
    MW_DFU_FwOpRead         read;                   // Pointer to the read operation function.
} MW_DFU_FwOperation_T;



// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static uint16_t mw_dfu_FwImageStartInt(void);
static uint16_t mw_dfu_FwImageUpdateInt(uint16_t length, uint8_t *p_content);
static uint16_t mw_dfu_FwImageValidateInt(uint16_t fwImageCrc);
static uint16_t mw_dfu_FwImageActivateInt(void);
static uint16_t mw_dfu_FwImageReadInt(uint32_t offset, uint16_t length, uint8_t *p_content);

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static uint8_t s_dfuState;                              // Current state of the DFU process.
static uint32_t s_dfuSizeInfo;                          // Holds size information for the DFU process. 
static uint32_t s_dfuAddr;                              // Current address pointer for the DFU process.
static uint32_t *sp_dfuIdent;                           // Pointer to an identifier used in the DFU process.

static uint32_t s_dfuStartAddr;
static uint32_t s_dfuMaxImgSize;

static const MW_DFU_FwOperation_T *sp_dfuFwOp;          // Pointer to the firmware operation structure.

/* Firmware operation structure for internal flash. */
static const MW_DFU_FwOperation_T s_dfuFwOpInt = 
{
    mw_dfu_FwImageStartInt,
    mw_dfu_FwImageUpdateInt,
    mw_dfu_FwImageValidateInt,
    mw_dfu_FwImageActivateInt,
    mw_dfu_FwImageReadInt
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initializes the firmware image update process.
 *
 * @retval MBA_RES_SUCCESS on success.
 */
static uint16_t mw_dfu_FwImageStartInt(void)
{
    s_dfuAddr = s_dfuStartAddr;
    s_dfuState = MW_DFU_STATE_FW_START;

    return MBA_RES_SUCCESS;
}

/**
 * @brief Updates the firmware image with the given content.
 *
 * @param[in] length The    length of the content to be written.
 * @param[in] p_content     Pointer to the content to be written.
 * 
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_INVALID_PARA if parameters are invalid.
 * @retval MBA_RES_FAIL if the write operation fails.
 * @retval MBA_RES_OOM if memory allocation fails.
 */
static uint16_t mw_dfu_FwImageUpdateInt(uint16_t length, uint8_t *p_content)
{
    uint32_t * p_data;
    uint32_t addr;

    /* Check if image content length and offset are legal */
    if ((s_dfuAddr + length > s_dfuStartAddr + s_dfuSizeInfo) || (length > MW_DFU_MAX_BLOCK_LEN)
        || (length & (MW_DFU_FW_ALIGN_SIZE-1U)) || (length == 0U))
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

    /* if need to check and revise sequnce number or src address */
    if (s_dfuAddr <= s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B)
    {
        
        if (*(uint32_t *)(p_content + MW_DFU_META_CHR_OFFSET_V3_272B) != MW_DFU_HDR_COHERENCE)
        {
            return MBA_RES_INVALID_PARA;
        }


        /* revise sequence number */
        if (s_dfuAddr <= s_dfuStartAddr + MW_DFU_META_SEQ_OFFSET_V3_272B && s_dfuAddr + length > s_dfuStartAddr + MW_DFU_META_SEQ_OFFSET_V3_272B)
        {
            *(uint32_t *)(p_content + s_dfuStartAddr + MW_DFU_META_SEQ_OFFSET_V3_272B - s_dfuAddr) = 0xFFFFFFFFU;
        }

        /* revise src address */
        if (s_dfuAddr <= s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B && s_dfuAddr + length > s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B)
        {
            if((MISCDIAG & 0x0000C000) == 0x0000C000)
            {
                *(uint32_t *)(p_content + s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B - s_dfuAddr) = MW_DFU_FW_EFLASH_0_START_ADDR + MW_DFU_FW_OFFSET;
            }
            else
            {
                *(uint32_t *)(p_content + s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B - s_dfuAddr) = s_dfuStartAddr + MW_DFU_FW_OFFSET;
            }
        }
    }
    
    p_data = (uint32_t *)p_content;
    addr = s_dfuAddr; 

    /* backup first 16 bytes */
    if (s_dfuAddr == s_dfuStartAddr)
    {
        if (sp_dfuIdent == NULL)
        {
            sp_dfuIdent = OSAL_Malloc(MW_DFU_IDENT_SIZE);
            if (sp_dfuIdent == NULL)
            {
                return MBA_RES_OOM;
            }
        }
        
        (void)memcpy(sp_dfuIdent, p_data, MW_DFU_IDENT_SIZE);

        addr += MW_DFU_IDENT_SIZE;
        p_data += MW_DFU_IDENT_SIZE/sizeof(*p_data);
    }
    
    for (;addr < s_dfuAddr + length; addr += MW_DFU_FW_ALIGN_SIZE)
    {
        if(!NVM_QuadDoubleWordWrite(p_data, addr))
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

        p_data += MW_DFU_FW_ALIGN_SIZE/sizeof(uint32_t);
    }

    s_dfuAddr = addr;

    s_dfuState = MW_DFU_STATE_FW_UPDATE;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Validates the firmware image against the provided CRC.
 *
 * This function checks if the firmware image is valid by comparing its calculated CRC
 * with the provided CRC value.
 *
 * @param[in] fwImageCrc    The expected CRC value of the firmware image.
 * 
 * @retval MBA_RES_SUCCESS if the CRC matches, MBA_RES_FAIL otherwise.
 */
static uint16_t mw_dfu_FwImageValidateInt(uint16_t fwImageCrc)
{
    DMAC_CRC_SETUP  crcSetup;
    uint32_t src = 0x00000000;
    uint32_t fwLen = 0, result;

    result = mw_dfu_FwImageReadInt(MW_DFU_META_LEN_OFFSET_V3_272B, 4, (uint8_t *)&fwLen);

    fwLen += MW_DFU_FW_OFFSET;

    if (fwLen > s_dfuMaxImgSize || result != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }

    crcSetup.polynomial_type = DMAC_CRC_TYPE_16;
    crcSetup.crc_mode = DMAC_CRC_MODE_DEFAULT;
    crcSetup.seed = 0xFFFF;

    crcSetup.seed = DMAC_CRCCalculate((void *)(s_dfuStartAddr + MW_DFU_CRC_OFFSET_V3_272B), MW_DFU_META_SRC_OFFSET_V3_272B - MW_DFU_CRC_OFFSET_V3_272B, crcSetup);
    crcSetup.seed = DMAC_CRCCalculate(&src, 4, crcSetup);  /* Set FW_IMG_SRC_ADR field as 0x0000000. */
    crcSetup.seed = DMAC_CRCCalculate((void *)(s_dfuStartAddr + MW_DFU_META_SRC_OFFSET_V3_272B + 4U), fwLen - MW_DFU_META_SRC_OFFSET_V3_272B - 4U, crcSetup); 
    
    if (crcSetup.seed != fwImageCrc)
    {
        return MBA_RES_FAIL;
    }
    
    return MBA_RES_SUCCESS;
}


/**
 * @brief Activates the firmware image.
 *
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_FAIL if the write operation fails.
 */
static uint16_t mw_dfu_FwImageActivateInt(void)
{
    if(!NVM_QuadDoubleWordWrite(sp_dfuIdent, s_dfuStartAddr))
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

    s_dfuState = MW_DFU_STATE_CONFIG;

    return MBA_RES_SUCCESS;
}

/**
 * @brief Reads a block of the firmware image from the internal flash memory.
 *
 * @param[in] offset    The offset from the start of the firmware image to begin reading.
 * @param[in] length    The length of the content to be read.
 * @param[in] p_content Pointer to the buffer where the read content will be stored.
 * 
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_INVALID_PARA if parameters are invalid.
 * @retval MBA_RES_FAIL if the read operation fails.
 */
static uint16_t mw_dfu_FwImageReadInt(uint32_t offset, uint16_t length, uint8_t *p_content)
{
    //Check read range 
    if ((offset + length > s_dfuMaxImgSize) || (length > MW_DFU_MAX_BLOCK_LEN))
    {
        return MBA_RES_INVALID_PARA;
    }

    while(NVM_IsBusy()==true)
    {
    }

    if (!NVM_Read((uint32_t *)p_content, length, s_dfuStartAddr + offset))
    {
        return MBA_RES_FAIL;
    }
    
    return MBA_RES_SUCCESS;
}

/**
 * @brief Configures the device information for the DFU process.
 *
 * @param[in] p_dfuInfo            Pointer to the DFU image information structure. See @ref MW_DFU_Info_T for details.
 *
 * @retval MBA_RES_SUCCESS         Configuration successful.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters due to:\n
 *                                  - Image size exceeding the maximum allowed.
 *                                  - Image size not being 32-byte aligned.
 *                                  - Invalid flash type (refer to @ref MW_DFU_FLASH_TYPE).
*/
uint16_t MW_DFU_Config(MW_DFU_Info_T * p_dfuInfo)
{
    if ((p_dfuInfo->fwImageSize == 0U) || (p_dfuInfo->fwImageSize & (MW_DFU_FW_ALIGN_SIZE-1U)))
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_dfuInfo->fwFlashType == MW_DFU_FLASH_INTERNAL)
    {
        if (MW_DFU_DEVSEL_PFM_1M & DSU_DID_DEVSEL(DSU_REGS->DSU_DID))
        {
            s_dfuStartAddr = MW_DFU_FW_EFLASH_1_START_ADDR_1M;
            s_dfuMaxImgSize = MW_DFU_MAX_SIZE_FW_IMAGE_INT_1M;
        }
        else
        {
            s_dfuStartAddr = MW_DFU_FW_EFLASH_1_START_ADDR_2M;
            s_dfuMaxImgSize = MW_DFU_MAX_SIZE_FW_IMAGE_INT_2M;
        }

        if (p_dfuInfo->fwImageSize > s_dfuMaxImgSize)
        {
            return MBA_RES_INVALID_PARA;
        }
    
        sp_dfuFwOp = &s_dfuFwOpInt;
    }
    else
    {
        return MBA_RES_INVALID_PARA;
    }

    s_dfuSizeInfo = p_dfuInfo->fwImageSize;

    s_dfuState = MW_DFU_STATE_CONFIG;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Starts or restarts the firmware image update procedure.
 * @note  Calling this API resets the state machine and parameters of the DFU module.
 *
 * @retval MBA_RES_SUCCESS         Update procedure started or restarted successfully.
 * @retval MBA_RES_BAD_STATE       API cannot be executed in the current DFU process state.
 */
uint16_t MW_DFU_FwImageStart(void)
{
    if (s_dfuState == MW_DFU_STATE_IDLE)
    {
        return MBA_RES_BAD_STATE;
    }

    return sp_dfuFwOp->start();
}


/**
 * @brief Updates a fragment of the firmware image to flash. 
 * @note  This API should be called multiple times to update all fragments of the firmware image.
 *
 * @param[in] length               Length of the image fragment in bytes. Must be 32-byte aligned for internal flash. 
 *                                  For external flash, it must be 256-byte aligned unless it is the last fragment.
 * @param[in] p_content            Pointer to the image fragment.
 *
 * @retval MBA_RES_SUCCESS         Firmware image fragment updated successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters due to:\n
 *                                  - Length exceeding the image size or @ref MW_DFU_MAX_BLOCK_LEN.
 *                                  - Length not meeting the alignment requirements.
 * @retval MBA_RES_BAD_STATE       API cannot be executed in the current DFU process state.
 * @retval MBA_RES_OOM             Insufficient memory available.
 * @retval MBA_RES_FAIL            Flash update failed.
 */
uint16_t MW_DFU_FwImageUpdate(uint16_t length, uint8_t *p_content)
{
    if ((s_dfuState != MW_DFU_STATE_FW_START) && (s_dfuState != MW_DFU_STATE_FW_UPDATE))
    {
        return MBA_RES_BAD_STATE;
    }

    return sp_dfuFwOp->update(length, p_content);
}

/**
 * @brief Validates the updated MCU image using CRC for internal flash.
 *
 * @param[in] fwCfmValue           CRC value for internal flash.
 *
 * @retval MBA_RES_SUCCESS         Validation successful.
 * @retval MBA_RES_FAIL            Validation failed.
 */
uint16_t MW_DFU_FwImageValidate(uint16_t fwCfmValue)
{
    return sp_dfuFwOp->validate(fwCfmValue);
}

/**
 * @brief Activates the new firmware after a system reboot.
 *
 * @note  The system state after activation should be verified to ensure proper operation.
 *
 * @retval MBA_RES_SUCCESS         Activation successful.
 * @retval MBA_RES_BAD_STATE       API cannot be executed in the current DFU process state.
 * @retval MBA_RES_OOM             Insufficient memory available.
 * @retval MBA_RES_FAIL            Flash access failed.
 */
uint16_t MW_DFU_FwImageActivate(void)
{
    if (s_dfuState != MW_DFU_STATE_FW_UPDATE)
    {
        return MBA_RES_BAD_STATE;
    }

    return sp_dfuFwOp->activate();
}

/**
 * @brief Reads a fragment of the firmware image from flash.
 * @note  The first 16 bytes read from flash are invalid before the new firmware image is activated.
 *
 *
 * @param[in] offset               Flash offset to start reading the image fragment.
 * @param[in] length               Length of the image fragment in bytes.
 * @param[in] p_content            Pointer to the buffer for the image fragment.
 *
 * @retval MBA_RES_SUCCESS         Firmware image fragment read successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters due to:\n
 *                                  - The sum of offset and length exceeding @ref MW_DFU_MAX_SIZE_FW_IMAGE.
 *                                  - Length exceeding @ref MW_DFU_MAX_BLOCK_LEN.
 * @retval MBA_RES_FAIL            Flash access failed.
 */
uint16_t MW_DFU_FwImageRead(uint32_t offset, uint16_t length, uint8_t *p_content)
{
    if (s_dfuState < MW_DFU_STATE_FW_START)
    {
        return MBA_RES_BAD_STATE;
    }

    return sp_dfuFwOp->read(offset, length, p_content);
}
