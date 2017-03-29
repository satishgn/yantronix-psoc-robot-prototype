/*******************************************************************************
* File Name: VRx.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_VRx_H) /* Pins VRx_H */
#define CY_PINS_VRx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VRx_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} VRx_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   VRx_Read(void);
void    VRx_Write(uint8 value);
uint8   VRx_ReadDataReg(void);
#if defined(VRx__PC) || (CY_PSOC4_4200L) 
    void    VRx_SetDriveMode(uint8 mode);
#endif
void    VRx_SetInterruptMode(uint16 position, uint16 mode);
uint8   VRx_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void VRx_Sleep(void); 
void VRx_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(VRx__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define VRx_DRIVE_MODE_BITS        (3)
    #define VRx_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - VRx_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the VRx_SetDriveMode() function.
         *  @{
         */
        #define VRx_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define VRx_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define VRx_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define VRx_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define VRx_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define VRx_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define VRx_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define VRx_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define VRx_MASK               VRx__MASK
#define VRx_SHIFT              VRx__SHIFT
#define VRx_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VRx_SetInterruptMode() function.
     *  @{
     */
        #define VRx_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define VRx_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define VRx_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define VRx_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(VRx__SIO)
    #define VRx_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(VRx__PC) && (CY_PSOC4_4200L)
    #define VRx_USBIO_ENABLE               ((uint32)0x80000000u)
    #define VRx_USBIO_DISABLE              ((uint32)(~VRx_USBIO_ENABLE))
    #define VRx_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define VRx_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define VRx_USBIO_ENTER_SLEEP          ((uint32)((1u << VRx_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << VRx_USBIO_SUSPEND_DEL_SHIFT)))
    #define VRx_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << VRx_USBIO_SUSPEND_SHIFT)))
    #define VRx_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << VRx_USBIO_SUSPEND_DEL_SHIFT)))
    #define VRx_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(VRx__PC)
    /* Port Configuration */
    #define VRx_PC                 (* (reg32 *) VRx__PC)
#endif
/* Pin State */
#define VRx_PS                     (* (reg32 *) VRx__PS)
/* Data Register */
#define VRx_DR                     (* (reg32 *) VRx__DR)
/* Input Buffer Disable Override */
#define VRx_INP_DIS                (* (reg32 *) VRx__PC2)

/* Interrupt configuration Registers */
#define VRx_INTCFG                 (* (reg32 *) VRx__INTCFG)
#define VRx_INTSTAT                (* (reg32 *) VRx__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define VRx_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(VRx__SIO)
    #define VRx_SIO_REG            (* (reg32 *) VRx__SIO)
#endif /* (VRx__SIO_CFG) */

/* USBIO registers */
#if !defined(VRx__PC) && (CY_PSOC4_4200L)
    #define VRx_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define VRx_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define VRx_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define VRx_DRIVE_MODE_SHIFT       (0x00u)
#define VRx_DRIVE_MODE_MASK        (0x07u << VRx_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins VRx_H */


/* [] END OF FILE */
