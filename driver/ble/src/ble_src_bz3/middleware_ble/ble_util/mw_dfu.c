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

#ifdef MW_DFU_EXTERNAL_FLASH_ENABLE
#include "driver/sst26/drv_sst26.h"
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define MW_DFU_FW_EFLASH_0_START_ADDR          0x01000000UL         // Start address of the first embedded flash memory for firmware.
#define MW_DFU_FW_EFLASH_1_START_ADDR          0x01040000UL         // Start address of the second embedded flash memory for firmware.
#define MW_DFU_FW_PAGE_SIZE                    0x1000U              // The size of a firmware page.
#define MW_DFU_FW_QUAD_WORD_SIZE               0x10U                // The size of a quad word used in firmware.

#define MW_DFU_META_SEQ_OFFSET_V1              0x00U                // Offset for the sequence number in metadata version 1.
#define MW_DFU_META_CHR_OFFSET_V1              0x06U                // Offset for characteristic in metadata version 1.
#define MW_DFU_META_SRC_OFFSET_V1              0x14U                // Offset for source address in metadata version 1.
#define MW_DFU_META_LEN_OFFSET_V1              0x1CU                // Offset for length in metadata version 1.
#define MW_DFU_CRC_OFFSET_V1                   0x10U                // Offset for CRC in metadata version 1.

#define MW_DFU_META_CHR_OFFSET_V3              0x08U                // Offset for characteristic in metadata version 3.
#define MW_DFU_META_SEQ_OFFSET_V3              0x1CU                // Offset for the sequence number in metadata version 3.
#define MW_DFU_META_SRC_OFFSET_V3              0x2CU                // Offset for source address in metadata version 3.
#define MW_DFU_META_LEN_OFFSET_V3              0x34U                // Offset for length in metadata version 3.
#define MW_DFU_CRC_OFFSET_V3                   0x20U                // Offset for CRC in metadata version 3.

#define MW_DFU_FW_OFFSET                       0x200U               // Offset for the firmware.

#define MISCDIAG (*((volatile unsigned int *) (0x44000440) ))       // A volatile pointer to a memory-mapped miscellaneous diagnostics register.

#define MW_DFU_IDENT_SIZE                      0x10U                // Size of the DFU identifier.

#define MW_DFU_FW_EXT_FLASH_START_ADDR         0x00000000UL         // Start address of the external flash memory for firmware.
#define MW_DFU_FW_EXT_FLASH_SLOT_SIZE          0x80000UL            // Slot size of the external flash memory for firmware.
#define MW_DFU_FW_EXT_SECTOR_SIZE              0x1000U              // Sector size of the external flash memory for firmware.

#define MW_DFU_EXT_FLASH_ID                    0x004326BFUL         // Identifier for the external flash memory.

#define MW_DFU_HDR_COHERENCE                   0x4D434850UL         // Coherence check value for the DFU header.
#define MW_DFU_EXT_HDR_COHERENCE               0x5048434DUL         // Coherence check value for the external DFU header.

/* Enumeration of the different states in the DFU (Device Firmware Update) process. */
typedef enum MW_DFU_State_T
{
    MW_DFU_STATE_IDLE=0X00U,                                        // DFU process is idle. */
    MW_DFU_STATE_CONFIG,                                            // DFU process is in configuration state. */
    MW_DFU_STATE_FW_START,                                          // DFU process has started firmware update. */
    MW_DFU_STATE_FW_UPDATE                                          // DFU process is updating firmware. */
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


/* Structure representing the header of the external flash used in firmware update. */
typedef struct MW_DFU_ExtFlashHdr_T
{
    uint32_t                coherence;              // Coherence check value.
    uint8_t                 mdRev;                  // Metadata revision.
    uint8_t                 plDecMthd;              // Payload decode method.
    uint8_t                 plDecKey;               // Payload decode key.
    uint8_t                 reserved;               // Reserved for future use.
    uint32_t                seqNum;                 // Sequence number.
    uint16_t                crc16;                  // CRC16 checksum for header validation.
    uint16_t                checksum;               // Checksum for additional validation.
} MW_DFU_ExtFlashHdr_T;


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

#ifdef MW_DFU_EXTERNAL_FLASH_ENABLE
static uint16_t mw_dfu_FwImageStartExt(void);
static uint16_t mw_dfu_FwImageUpdateExt(uint16_t length, uint8_t *p_content);
static uint16_t mw_dfu_FwImageValidateExt(uint16_t fwImageChksum);
static uint16_t mw_dfu_FwImageActivateExt(void);
static uint16_t mw_dfu_FwImageReadExt(uint32_t offset, uint16_t length, uint8_t *p_content);
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static uint8_t          s_dfuState;                                 // Current state of the DFU process.
static uint32_t         s_dfuSizeInfo;                              // Holds size information for the DFU process. 
static uint32_t         s_dfuAddr;                                  // Current address pointer for the DFU process.
static uint32_t         *sp_dfuIdent;                               // Pointer to an identifier used in the DFU process.
static bool             s_dfuIsV1Fw;                                // Flag to indicate if the firmware is version 1.

static const MW_DFU_FwOperation_T *sp_dfuFwOp;                      // Pointer to the firmware operation structure.

/* Firmware operation structure for internal flash. */
static const MW_DFU_FwOperation_T s_dfuFwOpInt = 
{
    mw_dfu_FwImageStartInt,
    mw_dfu_FwImageUpdateInt,
    mw_dfu_FwImageValidateInt,
    mw_dfu_FwImageActivateInt,
    mw_dfu_FwImageReadInt
};

#ifdef MW_DFU_EXTERNAL_FLASH_ENABLE
static uint32_t s_dfuStartAddr;                                     // Start address for the DFU process in external flash, only used when external flash is enabled.

/* Firmware operation structure for external flash. */
static const MW_DFU_FwOperation_T s_dfuFwOpExt = 
{
    mw_dfu_FwImageStartExt,
    mw_dfu_FwImageUpdateExt,
    mw_dfu_FwImageValidateExt,
    mw_dfu_FwImageActivateExt,
    mw_dfu_FwImageReadExt
};
#endif


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
    s_dfuAddr = MW_DFU_FW_EFLASH_1_START_ADDR;
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
    uint8_t srcOffset, seqOffset;

    /* Check if image content length and offset are legal */
    if ((s_dfuAddr + length > MW_DFU_FW_EFLASH_1_START_ADDR + s_dfuSizeInfo) || (length > MW_DFU_MAX_BLOCK_LEN)
        || (length & (MW_DFU_FW_QUAD_WORD_SIZE-1U)) || (length == 0U))
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
    if (s_dfuAddr <= MW_DFU_FW_EFLASH_1_START_ADDR + MW_DFU_META_SRC_OFFSET_V3)
    {
        
        if (*(uint32_t *)(p_content + MW_DFU_META_CHR_OFFSET_V1) == MW_DFU_HDR_COHERENCE)
        {
            seqOffset = MW_DFU_META_SEQ_OFFSET_V1;
            srcOffset = MW_DFU_META_SRC_OFFSET_V1;
            s_dfuIsV1Fw = true;
        }
        else if (*(uint32_t *)(p_content + MW_DFU_META_CHR_OFFSET_V3) == MW_DFU_HDR_COHERENCE)
        {
            seqOffset = MW_DFU_META_SEQ_OFFSET_V3;
            srcOffset = MW_DFU_META_SRC_OFFSET_V3;

            s_dfuIsV1Fw = false;
        }
        else
        {
            return MBA_RES_INVALID_PARA;
        }


        /* revise sequence number */
        if (s_dfuAddr <= MW_DFU_FW_EFLASH_1_START_ADDR + seqOffset && s_dfuAddr + length > MW_DFU_FW_EFLASH_1_START_ADDR + seqOffset)
        {
            uint32_t seq;

            if (s_dfuIsV1Fw == true)
            {
                while(NVM_IsBusy()==true)
                {
                }
                
                if (!NVM_Read(&seq, 4, MW_DFU_FW_EFLASH_0_START_ADDR + MW_DFU_META_SEQ_OFFSET_V1))
                {
                    return MBA_RES_FAIL;
                }

                *(uint32_t *)(p_content + MW_DFU_FW_EFLASH_1_START_ADDR + MW_DFU_META_SEQ_OFFSET_V1 - s_dfuAddr) = --seq;
            }
            else
            {
                seq = 0xFFFFFFFFU;
            }
            
            *(uint32_t *)(p_content + MW_DFU_FW_EFLASH_1_START_ADDR + seqOffset - s_dfuAddr) = seq;
        }

        /* revise src address */
        if (s_dfuAddr <= MW_DFU_FW_EFLASH_1_START_ADDR + srcOffset && s_dfuAddr + length > MW_DFU_FW_EFLASH_1_START_ADDR + srcOffset)
        {
            if((MISCDIAG & 0x0000C000) == 0x0000C000)
            {
                *(uint32_t *)(p_content + MW_DFU_FW_EFLASH_1_START_ADDR + srcOffset - s_dfuAddr) = MW_DFU_FW_EFLASH_0_START_ADDR + MW_DFU_FW_OFFSET;
            }
            else
            {
                *(uint32_t *)(p_content + MW_DFU_FW_EFLASH_1_START_ADDR + srcOffset - s_dfuAddr) = MW_DFU_FW_EFLASH_1_START_ADDR + MW_DFU_FW_OFFSET;
            }
        }
    }
    
    p_data = (uint32_t *)p_content;
    addr = s_dfuAddr; 

    /* backup first 16 bytes */
    if (s_dfuAddr == MW_DFU_FW_EFLASH_1_START_ADDR)
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
    uint8_t srcOffset, crcOffset;
    uint32_t fwLen = 0, result;

    if (s_dfuIsV1Fw == true)
    {
        crcOffset = MW_DFU_CRC_OFFSET_V1;
        srcOffset = MW_DFU_META_SRC_OFFSET_V1;
        result = mw_dfu_FwImageReadInt(MW_DFU_META_LEN_OFFSET_V1, 4, (uint8_t *)&fwLen);
    }
    else
    {
        crcOffset = MW_DFU_CRC_OFFSET_V3;
        srcOffset = MW_DFU_META_SRC_OFFSET_V3;
        result = mw_dfu_FwImageReadInt(MW_DFU_META_LEN_OFFSET_V3, 4, (uint8_t *)&fwLen);
    }

    fwLen += MW_DFU_FW_OFFSET;

    if (fwLen > MW_DFU_MAX_SIZE_FW_IMAGE_INT || result != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }

    crcSetup.polynomial_type = DMAC_CRC_TYPE_16;
    crcSetup.crc_mode = DMAC_CRC_MODE_DEFAULT;
    crcSetup.seed = 0xFFFF;

    crcSetup.seed = DMAC_CRCCalculate((void *)(MW_DFU_FW_EFLASH_1_START_ADDR + crcOffset), srcOffset - crcOffset, crcSetup);
    crcSetup.seed = DMAC_CRCCalculate(&src, 4, crcSetup);  /* Set FW_IMG_SRC_ADR field as 0x0000000. */
    crcSetup.seed = DMAC_CRCCalculate((void *)(MW_DFU_FW_EFLASH_1_START_ADDR + srcOffset + 4U), fwLen - srcOffset - 4U, crcSetup); 
    
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
    if(!NVM_QuadWordWrite(sp_dfuIdent, MW_DFU_FW_EFLASH_1_START_ADDR))
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
    if ((offset + length > MW_DFU_MAX_SIZE_FW_IMAGE_INT) || (length > MW_DFU_MAX_BLOCK_LEN))
    {
        return MBA_RES_INVALID_PARA;
    }

    while(NVM_IsBusy()==true)
    {
    }

    if (!NVM_Read((uint32_t *)p_content, length, MW_DFU_FW_EFLASH_1_START_ADDR + offset))
    {
        return MBA_RES_FAIL;
    }
    
    return MBA_RES_SUCCESS;
}

#ifdef MW_DFU_EXTERNAL_FLASH_ENABLE
/**
 * @brief Validates the header of the external flash.
 *
 * @param[in] p_hdr Pointer to the external flash header structure.
 * 
 * @retval true if the header is valid, false otherwise.
 */
static bool mw_dfu_validExtHdr(MW_DFU_ExtFlashHdr_T * p_hdr)
{
    uint8_t *p_data;
    uint16_t chksum, idx;
    uint32_t seq;

    if (p_hdr->coherence != MW_DFU_EXT_HDR_COHERENCE 
        || p_hdr->mdRev != 0x01U || p_hdr->plDecMthd > 0x01U || p_hdr->seqNum == 0U)
    {
        return false;
    }

    p_data = (uint8_t *)p_hdr;
    seq = p_hdr->seqNum;
    p_hdr->seqNum = 0;
    chksum = 0;
    for (idx = 0; idx < 14U; idx++)
    {
        chksum += p_data[idx];
    }

    p_hdr->seqNum = seq;

    if (chksum != p_hdr->checksum)
    {
        return false;
    }

    return true;
}


/**
 * @brief Initializes the firmware image update process for external flash.
 *
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_FAIL if the operation fails.
 */
static uint16_t mw_dfu_FwImageStartExt(void)
{
    DRV_HANDLE hdl;
    MW_DFU_ExtFlashHdr_T hdr;
    uint32_t seq;

    
    hdl = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
    if (hdl == DRV_HANDLE_INVALID)
    {
        return MBA_RES_FAIL;
    }

    if (!DRV_SST26_Read(hdl, &hdr, sizeof(hdr), MW_DFU_FW_EXT_FLASH_START_ADDR))
    {
        DRV_SST26_Close(hdl);
        return MBA_RES_FAIL;
    }

    while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
    {
    }

    s_dfuAddr = MW_DFU_FW_EXT_FLASH_START_ADDR;

    if (mw_dfu_validExtHdr(&hdr) && hdr.seqNum != 0xFFFFFFFFUL)
    {
        seq = hdr.seqNum;
        
        if (!DRV_SST26_Read(hdl, &hdr, sizeof(hdr), MW_DFU_FW_EXT_FLASH_START_ADDR + MW_DFU_FW_EXT_FLASH_SLOT_SIZE))
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_FAIL;
        }

        while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
        {
        }

        if (!mw_dfu_validExtHdr(&hdr) || seq < hdr.seqNum)
        {
            s_dfuAddr = MW_DFU_FW_EXT_FLASH_START_ADDR + MW_DFU_FW_EXT_FLASH_SLOT_SIZE;
        }
    }

    DRV_SST26_Close(hdl);

    s_dfuState = MW_DFU_STATE_FW_START;
    s_dfuStartAddr = s_dfuAddr;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Updates the firmware image with the given content for external flash.
 *
 * @param[in] length        The length of the content to be written.
 * @param[in] p_content     Pointer to the content to be written.
 * 
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_INVALID_PARA if parameters are invalid,
 * @retval MBA_RES_FAIL if the write operation fails
 * @retval MBA_RES_OOM if memory allocation fails.
 */
static uint16_t mw_dfu_FwImageUpdateExt(uint16_t length, uint8_t *p_content)
{
    uint32_t * p_data;
    uint32_t addr;
    DRV_HANDLE hdl;
    
    /* check if image content length and offset are legal */
    if ((s_dfuAddr + length > s_dfuStartAddr + s_dfuSizeInfo) || (length > MW_DFU_MAX_BLOCK_LEN)
        || ((s_dfuAddr + length != s_dfuStartAddr + s_dfuSizeInfo) && (length & (DRV_SST26_PAGE_SIZE-1)))
        || (length == 0U))
    {
        return MBA_RES_INVALID_PARA;
    }

    hdl = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
    if (hdl == DRV_HANDLE_INVALID)
    {
        return MBA_RES_FAIL;
    }

    /* if start from page boundary or write cross page, erase new page */
    if ((s_dfuAddr & (MW_DFU_FW_EXT_SECTOR_SIZE - 1U)) == 0U
    || (s_dfuAddr & ~(MW_DFU_FW_EXT_SECTOR_SIZE - 1U)) < ((s_dfuAddr + length - 1U)  & ~(MW_DFU_FW_EXT_SECTOR_SIZE - 1U)))
    {
        if (!DRV_SST26_SectorErase(hdl, (s_dfuAddr + length - 1U) & ~(MW_DFU_FW_EXT_SECTOR_SIZE - 1U)))
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_FAIL;
        }

        while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
        {
        }
    }

    
    p_data = (uint32_t *)p_content;
    addr = s_dfuAddr; 

    /* for activate later, we need to change first 16 bytes to 0xFF */
    if (s_dfuAddr == s_dfuStartAddr)
    {
        uint8_t *p_buf;
        bool ret;
        
        p_buf = OSAL_Malloc(DRV_SST26_PAGE_SIZE);
        if (p_buf == NULL)
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_OOM;
        }
        (void)memset(p_buf, 0xFF, MW_DFU_IDENT_SIZE);
        (void)memcpy(p_buf + MW_DFU_IDENT_SIZE, (uint8_t *)p_data + MW_DFU_IDENT_SIZE, DRV_SST26_PAGE_SIZE - MW_DFU_IDENT_SIZE);

        ret = DRV_SST26_PageWrite(hdl, p_buf, addr);
        OSAL_Free(p_buf);

        if(!ret)
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_FAIL;
        }

        while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
        {
        }

        addr += DRV_SST26_PAGE_SIZE;
        p_data += DRV_SST26_PAGE_SIZE/sizeof(*p_data);


        if (sp_dfuIdent == NULL)
        {
            sp_dfuIdent = OSAL_Malloc(MW_DFU_IDENT_SIZE);
            if (sp_dfuIdent == NULL)
            {
                DRV_SST26_Close(hdl);
                return MBA_RES_OOM;
            }
        }
        
        (void)memcpy((void *)sp_dfuIdent, (void *)p_content, MW_DFU_IDENT_SIZE);
    }

    
    for (;addr < s_dfuAddr + length; addr += DRV_SST26_PAGE_SIZE)
    {
        if(!DRV_SST26_PageWrite(hdl, p_data, addr))
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_FAIL;
        }

        while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
        {
        }

        p_data += DRV_SST26_PAGE_SIZE/sizeof(*p_data);
    }

    s_dfuAddr = addr;

    s_dfuState = MW_DFU_STATE_FW_UPDATE;

    DRV_SST26_Close(hdl);

    return MBA_RES_SUCCESS;
}


/**
 * @brief Validates the firmware image against the provided checksum for external flash.
 *
 * @param[in] fwImageChksum The expected checksum value of the firmware image.
 * 
 * @retval MBA_RES_SUCCESS if the checksum matches.
 * @retval MBA_RES_FAIL otherwise.
 */
static uint16_t mw_dfu_FwImageValidateExt(uint16_t fwImageChksum)
{

    uint16_t chksum;
    uint32_t offset, idx;
    uint8_t buf[16];


    chksum = 0;
    for (idx = 0; idx < MW_DFU_IDENT_SIZE; idx++)
    {
        chksum += *((uint8_t *)sp_dfuIdent + idx);
    }


    for (offset = MW_DFU_IDENT_SIZE; offset < s_dfuSizeInfo; offset += sizeof(buf))
    {
        mw_dfu_FwImageReadExt(offset, sizeof(buf), buf);

        for (idx = 0; idx < sizeof(buf); idx++)
        {
            chksum += buf[idx];
        }
    }

    chksum = 0xFFFFU - chksum + 1U;

    if (chksum != fwImageChksum)
    {
        return MBA_RES_FAIL;
    }
    
    return MBA_RES_SUCCESS;
}

/**
 * @brief Activates the firmware image for external flash.
 *
 * @retval MBA_RES_SUCCESS on success.
 * @retval MBA_RES_FAIL if the operation fails.
 * @retval MBA_RES_OOM if memory allocation fails.
 */
static uint16_t mw_dfu_FwImageActivateExt(void)
{
    DRV_HANDLE hdl;
    uint8_t *p_ident;
    uint16_t status;

    p_ident = OSAL_Malloc(DRV_SST26_PAGE_SIZE);

    if (p_ident == NULL)
    {
        return MBA_RES_OOM;
    }

    status = MBA_RES_FAIL;
    hdl = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
    if (hdl != DRV_HANDLE_INVALID)
    {
        if (DRV_SST26_Read(hdl, p_ident, DRV_SST26_PAGE_SIZE, s_dfuStartAddr)==true)
        {
            while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
            {
            }

            (void)memcpy((void *)p_ident, (void *)sp_dfuIdent, MW_DFU_IDENT_SIZE);
        
            if(DRV_SST26_PageWrite(hdl, p_ident, s_dfuStartAddr)==true)
            {
                while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
                {
                }

                OSAL_Free(sp_dfuIdent);
                sp_dfuIdent = NULL;
                
                s_dfuState = MW_DFU_STATE_CONFIG;

                status = MBA_RES_SUCCESS;
            }
        }
        DRV_SST26_Close(hdl);
    }

    OSAL_Free(p_ident);

    return status;
}


/**
 * @brief Read firmware image from external memory.
 *
 * This function reads a block of the firmware image from the external memory
 * starting at the specified offset for the given length.
 *
 * @param[in] offset        The offset in the external memory where the read operation should begin.
 * @param[in] length        The number of bytes to read from the external memory.
 * @param[in] p_content     Pointer to the buffer where the read data should be stored.
 *
 * @retval MBA_RES_SUCCESS on successful read operation.
 * @retval MBA_RES_INVALID_PARA if the read range is outside the allowed limits.
 * @retval MBA_RES_FAIL if the read operation fails or if the driver handle is invalid.
 */
static uint16_t mw_dfu_FwImageReadExt(uint32_t offset, uint16_t length, uint8_t *p_content)
{
    DRV_HANDLE hdl;
    
    //check read range 
    if ((offset + length > MW_DFU_MAX_SIZE_FW_IMAGE_EXT) || (length > MW_DFU_MAX_BLOCK_LEN))
    {
        return MBA_RES_INVALID_PARA;
    }

    hdl = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
    if (hdl == DRV_HANDLE_INVALID)
    {
        return MBA_RES_FAIL;
    }

    if (!DRV_SST26_Read(hdl, (void*)p_content, length, s_dfuStartAddr + offset))
    {
        DRV_SST26_Close(hdl);
        return MBA_RES_FAIL;
    }

    while(DRV_SST26_TransferStatusGet(hdl) == DRV_SST26_TRANSFER_BUSY)
    {
    }

    DRV_SST26_Close(hdl);
    
    return MBA_RES_SUCCESS;
}
#endif


/**
 * @brief Configures the device information for the DFU process.
 *
 * @param[in] p_dfuInfo            Pointer to the DFU image information structure. See @ref MW_DFU_Info_T for details.
 *
 * @retval MBA_RES_SUCCESS         Configuration successful.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters due to:\n
 *                                  - Image size exceeding the maximum allowed.
 *                                  - Image size not being 16-byte aligned.
 *                                  - Invalid flash type (refer to @ref MW_DFU_FLASH_TYPE).
 * @retval MBA_RES_FAIL            External flash access failed.
 */
uint16_t MW_DFU_Config(MW_DFU_Info_T * p_dfuInfo)
{
    if ((p_dfuInfo->fwImageSize == 0U) || (p_dfuInfo->fwImageSize & (MW_DFU_FW_QUAD_WORD_SIZE-1U)))
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_dfuInfo->fwFlashType == MW_DFU_FLASH_INTERNAL)
    {
        if (p_dfuInfo->fwImageSize > MW_DFU_MAX_SIZE_FW_IMAGE_INT)
        {
            return MBA_RES_INVALID_PARA;
        }
    
        sp_dfuFwOp = &s_dfuFwOpInt;
    }
    #ifdef MW_DFU_EXTERNAL_FLASH_ENABLE
    else if (p_dfuInfo->fwFlashType == MW_DFU_FLASH_EXTERNAL)
    {
        uint32_t jedecId;
        DRV_HANDLE hdl;

        if (p_dfuInfo->fwImageSize > MW_DFU_MAX_SIZE_FW_IMAGE_EXT)
        {
            return MBA_RES_INVALID_PARA;
        }


        hdl = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
        if (hdl == DRV_HANDLE_INVALID)
        {
            return MBA_RES_FAIL;
        }
        
        
        if (!DRV_SST26_ReadJedecId(hdl, &jedecId))
        {
            DRV_SST26_Close(hdl);
            return MBA_RES_FAIL;
        }

        DRV_SST26_Close(hdl);

        if ((jedecId & 0xFFFFFFUL) != MW_DFU_EXT_FLASH_ID)
        {
           return MBA_RES_FAIL;
        }

        sp_dfuFwOp = &s_dfuFwOpExt;
    }
    #endif
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
 * @retval MBA_RES_FAIL            External flash access failed.
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
 * @param[in] length               Length of the image fragment in bytes. Must be 16-byte aligned for internal flash. 
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
 * @brief Validates the updated MCU image using CRC for internal flash or checksum for external flash.
 *
 * @param[in] fwCfmValue           CRC value for internal flash or checksum value for external flash.
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
