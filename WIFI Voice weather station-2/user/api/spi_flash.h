/**
  ******************************************************************************
  * @file    stm32_eval_spi_flash.h
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains all the functions prototypes for the stm32_eval_spi_flash
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM32_EVAL_SPI_FLASH
  * @{
  */  

/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Types
  * @{
  */ 
/**
  * @}
  */
  
/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Constants
  * @{
  */
/**
  * @brief  M25P SPI Flash supported commands
  */  
#define sFLASH_CMD_WRITE          0x02  /*!< 页编程Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< 写状态寄存器Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< 写使能 Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< 读数据Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< 读状态寄存器1 Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< 读ID Read identification */
#define sFLASH_CMD_SE             0x20  /*!< 扇区擦除指令 Sector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /*!< 整体擦除 Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< 写入正在进行的标志 Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xA5  /*无意义字节  */
#define sFLASH_SPI_PAGESIZE       0x100 /*页大小  256字节*/

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P64_ID          0x202017
#define sFLASH_W25Q64_ID          0xEF4015

#define sFLASH_SPI 		SPI2
/**
  * @}
  */ 
  
/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Macros
  * @{
  */
/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)  
/**
  * @}
  */ 
  


/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Functions
  * @{
  */
/**
  * @brief  High layer functions
  */
void sFLASH_DeInit(void);
void sFLASH_Init(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBulk(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);
void SPI2_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32_EVAL_SPI_FLASH_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
