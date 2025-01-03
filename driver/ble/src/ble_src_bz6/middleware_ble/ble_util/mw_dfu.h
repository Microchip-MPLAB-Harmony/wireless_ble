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
  Middleware Device Firmware Udpate Header File

  Company:
    Microchip Technology Inc.

  File Name:
    mw_dfu.h

  Summary:
    This header file provides the API for the BLE Device Firmware Update functionality.

  Description:
    This header file includes function declarations and necessary definitions for 
    the application user to perform firmware updates.
 *******************************************************************************/
#ifndef MW_DFU_H
#define MW_DFU_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @addtogroup BLE_MW BLE Middleware
 * @{
 */

/**
 * @defgroup MW_DFU Device Firmware Update (DFU)
 * 
 * @brief Defines the API for the Device Firmware Update (DFU).
 * @note This file contains the definitions and function prototypes necessary for
 *       integrating the Device Firmware Update feature into BLE applications.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup MW_DFU_DEFINES Defines
 * @{
 */


/**
 * @defgroup MW_DFU_MAX_IMAGE_SIZE Maximum image size
 * @brief Defines the maximum allowable size for a firmware image during Device Firmware Update (DFU).
 * @{
 */
#define MW_DFU_MAX_SIZE_FW_IMAGE_INT_2M          (1048576)       /**< Maximum size of the firmware image for 2MB internal flash in bytes. */
#define MW_DFU_MAX_SIZE_FW_IMAGE_INT_1M          (524288)        /**< Maximum size of the firmware image for 1MB internal flash in bytes. */
/** @} */


/**
 * @defgroup MW_DFU_MAX_BLOCK_LEN Maximum block length
 * @brief Defines the maximum length of a data block during DFU.
 * @{
 */
#define MW_DFU_MAX_BLOCK_LEN                   (0x400U)            /**< Maximum data block length in bytes. */
/** @} */


/**
 * @defgroup MW_DFU_FLASH_TYPE Flash types
 * @brief Defines the type of flash memory used during DFU.
 * @{
 */
#define MW_DFU_FLASH_INTERNAL                 (0x01U)            /**< Identifier for DFU operations on internal flash memory. */
/** @} */


/** @} */ //MW_DFU_DEFINES


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup MW_DFU_STRUCTS Structures
 * @{
 */

/**
 * @brief Structure for DFU (Device Firmware Update) information. 
 * 
 * This structure is used to store information about the firmware image
 * that is relevant for the DFU process, such as its size and the type
 * of flash memory it is intended for. The image size is related to the
 * maximum image size defined by @ref MW_DFU_MAX_IMAGE_SIZE.
*/
typedef struct MW_DFU_Info_T
{
    uint32_t    fwImageSize;                                      /**< Size of the firmware image in bytes. Must be a multiple of 32 bytes for alignment purposes. 
                                                                    See @ref MW_DFU_MAX_IMAGE_SIZE for the maximum allowable size. */
    uint8_t     fwFlashType;                                      /**< Type of flash memory where the firmware is stored. Defined by numeration @ref MW_DFU_FLASH_TYPE. */
} MW_DFU_Info_T;

/** @} */ //MW_DFU_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup MW_DFU_FUNS Functions
 * @{
 */

/**
 * @brief Configures the device information for the Device Firmware Update (DFU) process.
 *
 * @param[in] p_dfuInfo            Pointer to the structure containing the DFU image size information. 
 *                                  Refer to @ref MW_DFU_Info_T for the structure's detailed information.
 *
 *
 * @retval MBA_RES_SUCCESS         Configuration successful.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The size exceeds the maximum allowable size, or the size is not aligned to a 32-byte boundary
 *									or invalid flash type (refer to @ref MW_DFU_FLASH_TYPE).
 */
uint16_t MW_DFU_Config(MW_DFU_Info_T *p_dfuInfo);


/**
 * @brief Starts or restarts the firmware image update procedure.
 * @note  Calling this API resets the state machine and parameters of the DFU module.
 *
 *
 * @retval MBA_RES_SUCCESS         Firmware image update procedure started or restarted successfully.
 * @retval MBA_RES_BAD_STATE       The API cannot be executed in the current state of the DFU process.
 */
uint16_t MW_DFU_FwImageStart(void);


/**
 * @brief Updates a fragment of the firmware image in flash memory.
 * @note  This API should be called multiple times to update all fragments of the firmware image.
 *
 * @param[in] length               The length of the image fragment to update, in bytes. Must be aligned to 16 bytes for internal flash.
 * @param[in] p_content            Pointer to the image fragment data.
 *
 *
 * @retval MBA_RES_SUCCESS         Fragment of firmware image updated successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The length exceeds the image size, the maximum block length @ref MW_DFU_MAX_BLOCK_LEN, 
 *                                  or does not meet alignment requirements.
 * @retval MBA_RES_BAD_STATE       The API cannot be executed in the current state of the DFU process.
 * @retval MBA_RES_OOM             Out of memory error.
 * @retval MBA_RES_FAIL            Failed to update the fragment to flash memory.
 */
uint16_t MW_DFU_FwImageUpdate(uint16_t length, uint8_t *p_content);


/**
 * @brief Validates the updated MCU image using CRC for internal flash.
 *
 * @param[in] fwCfmValue           CRC value for internal flash.
 *
 * @retval MBA_RES_SUCCESS         Validation successful.
 * @retval MBA_RES_FAIL            Validation failed.
 */
uint16_t MW_DFU_FwImageValidate(uint16_t fwCfmValue);


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
uint16_t MW_DFU_FwImageActivate(void);


/**
 * @brief Reads a fragment of the firmware image from flash memory.
 * @note  The first 16 bytes of the image fragment read from flash are invalid until the full firmware image is activated.
 *
 *
 * @param[in] offset               The offset in flash memory from where to start reading the image fragment.
 * @param[in] length               The length of the image fragment to read, in bytes.
 * @param[in] p_content            Pointer to the buffer where the image fragment will be stored.
 *
 *
 * @retval MBA_RES_SUCCESS         Fragment of firmware image read successfully.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters. The sum of offset and length exceeds the maximum firmware image size @ref MW_DFU_MAX_SIZE_FW_IMAGE, 
 *                                  or the length exceeds the maximum block length @ref MW_DFU_MAX_BLOCK_LEN.
 * @retval MBA_RES_FAIL            Flash access failed.
 */
uint16_t MW_DFU_FwImageRead(uint32_t offset, uint16_t length, uint8_t *p_content);

/** @} */ //MW_DFU_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //MW_DFU_H
