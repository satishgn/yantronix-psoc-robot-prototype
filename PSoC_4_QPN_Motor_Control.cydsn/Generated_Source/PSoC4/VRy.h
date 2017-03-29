/*******************************************************************************
* File Name: VRy.h  
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

#if !defined(CY_PINS_VRy_H) /* Pins VRy_H */
#define CY_PINS_VRy_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VRy_aliases.h"


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
} VRy_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   VRy_Read(void);
void    VRy_Write(uint8 value);
uint8   VRy_ReadDataReg(void);
#if defined(VRy__PC) || (CY_PSOC4_4200L) 
    void    VRy_SetDriveMode(uint8 mode);
#endif
void    VRy_SetInterruptMode(uint16 position, uint16 mode);
uint8   VRy_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void VRy_Sleep(void); 
void VRy_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(VRy__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define VRy_DRIVE_MODE_BITS        (3)
    #define VRy_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - VRy_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the VRy_SetDriveMode() function.
         *  @{
         */
        #define VRy_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define VRy_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define VRy_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define VRy_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define VRy_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define VRy_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define VRy_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define VRy_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define VRy_MASK               VRy__MASK
#define VRy_SHIFT              VRy__SHIFT
#define VRy_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VRy_SetInterruptMode() function.
     *  @{
     */
        #define VRy_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define VRy_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define VRy_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define VRy_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(VRy__SIO)
    #define VRy_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(VRy__PC) && (CY_PSOC4_4200L)
    #define VRy_USBIO_ENABLE               ((uint32)0x80000000u)
    #define VRy_USBIO_DISABLE              ((uint32)(~VRy_USBIO_ENABLE))
    #define VRy_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define VRy_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define VRy_USBIO_ENTER_SLEEP          ((uint32)((1u << VRy_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << VRy_USBIO_SUSPEND_DEL_SHIFT)))
    #define VRy_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << VRy_USBIO_SUSPEND_SHIFT)))
    #define VRy_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << VRy_USBIO_SUSPEND_DEL_SHIFT)))
    #define VRy_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(VRy__PC)
    /* Port Configuration */
    #define VRy_PC                 (* (reg32 *) VRy__PC)
#endif
/* Pin State */
#define VRy_PS                     (* (reg32 *) VRy__PS)
/* Data Register */
#define VRy_DR                     (* (reg32 *) VRy__DR)
/* Input Buffer Disable Override */
#define VRy_INP_DIS                (* (reg32 *) VRy__PC2)

/* Interrupt configuration Registers */
#define VRy_INTCFG                 (* (reg32 *) VRy__INTCFG)
#define VRy_INTSTAT                (* (reg32 *) VRy__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define VRy_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(VRy__SIO)
    #define VRy_SIO_REG            (* (reg32 *) VRy__SIO)
#endif /* (VRy__SIO_CFG) */

/* USBIO registers */
#if !defined(VRy__PC) && (CY_PSOC4_4200L)
    #define VRy_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define VRy_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define VRy_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define VRy_DRIVE_MODE_SHIFT       (0x00u)
#define VRy_DRIVE_MODE_MASK        (0x07u << VRy_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins VRy_H */


/* [] END OF FILE */
