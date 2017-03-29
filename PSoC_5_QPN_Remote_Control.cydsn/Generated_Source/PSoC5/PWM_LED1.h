/*******************************************************************************
* File Name: PWM_LED1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PWM_LED1_H) /* Pins PWM_LED1_H */
#define CY_PINS_PWM_LED1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PWM_LED1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PWM_LED1__PORT == 15 && ((PWM_LED1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PWM_LED1_Write(uint8 value);
void    PWM_LED1_SetDriveMode(uint8 mode);
uint8   PWM_LED1_ReadDataReg(void);
uint8   PWM_LED1_Read(void);
void    PWM_LED1_SetInterruptMode(uint16 position, uint16 mode);
uint8   PWM_LED1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PWM_LED1_SetDriveMode() function.
     *  @{
     */
        #define PWM_LED1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PWM_LED1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PWM_LED1_DM_RES_UP          PIN_DM_RES_UP
        #define PWM_LED1_DM_RES_DWN         PIN_DM_RES_DWN
        #define PWM_LED1_DM_OD_LO           PIN_DM_OD_LO
        #define PWM_LED1_DM_OD_HI           PIN_DM_OD_HI
        #define PWM_LED1_DM_STRONG          PIN_DM_STRONG
        #define PWM_LED1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PWM_LED1_MASK               PWM_LED1__MASK
#define PWM_LED1_SHIFT              PWM_LED1__SHIFT
#define PWM_LED1_WIDTH              1u

/* Interrupt constants */
#if defined(PWM_LED1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PWM_LED1_SetInterruptMode() function.
     *  @{
     */
        #define PWM_LED1_INTR_NONE      (uint16)(0x0000u)
        #define PWM_LED1_INTR_RISING    (uint16)(0x0001u)
        #define PWM_LED1_INTR_FALLING   (uint16)(0x0002u)
        #define PWM_LED1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PWM_LED1_INTR_MASK      (0x01u) 
#endif /* (PWM_LED1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PWM_LED1_PS                     (* (reg8 *) PWM_LED1__PS)
/* Data Register */
#define PWM_LED1_DR                     (* (reg8 *) PWM_LED1__DR)
/* Port Number */
#define PWM_LED1_PRT_NUM                (* (reg8 *) PWM_LED1__PRT) 
/* Connect to Analog Globals */                                                  
#define PWM_LED1_AG                     (* (reg8 *) PWM_LED1__AG)                       
/* Analog MUX bux enable */
#define PWM_LED1_AMUX                   (* (reg8 *) PWM_LED1__AMUX) 
/* Bidirectional Enable */                                                        
#define PWM_LED1_BIE                    (* (reg8 *) PWM_LED1__BIE)
/* Bit-mask for Aliased Register Access */
#define PWM_LED1_BIT_MASK               (* (reg8 *) PWM_LED1__BIT_MASK)
/* Bypass Enable */
#define PWM_LED1_BYP                    (* (reg8 *) PWM_LED1__BYP)
/* Port wide control signals */                                                   
#define PWM_LED1_CTL                    (* (reg8 *) PWM_LED1__CTL)
/* Drive Modes */
#define PWM_LED1_DM0                    (* (reg8 *) PWM_LED1__DM0) 
#define PWM_LED1_DM1                    (* (reg8 *) PWM_LED1__DM1)
#define PWM_LED1_DM2                    (* (reg8 *) PWM_LED1__DM2) 
/* Input Buffer Disable Override */
#define PWM_LED1_INP_DIS                (* (reg8 *) PWM_LED1__INP_DIS)
/* LCD Common or Segment Drive */
#define PWM_LED1_LCD_COM_SEG            (* (reg8 *) PWM_LED1__LCD_COM_SEG)
/* Enable Segment LCD */
#define PWM_LED1_LCD_EN                 (* (reg8 *) PWM_LED1__LCD_EN)
/* Slew Rate Control */
#define PWM_LED1_SLW                    (* (reg8 *) PWM_LED1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PWM_LED1_PRTDSI__CAPS_SEL       (* (reg8 *) PWM_LED1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PWM_LED1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PWM_LED1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PWM_LED1_PRTDSI__OE_SEL0        (* (reg8 *) PWM_LED1__PRTDSI__OE_SEL0) 
#define PWM_LED1_PRTDSI__OE_SEL1        (* (reg8 *) PWM_LED1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PWM_LED1_PRTDSI__OUT_SEL0       (* (reg8 *) PWM_LED1__PRTDSI__OUT_SEL0) 
#define PWM_LED1_PRTDSI__OUT_SEL1       (* (reg8 *) PWM_LED1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PWM_LED1_PRTDSI__SYNC_OUT       (* (reg8 *) PWM_LED1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PWM_LED1__SIO_CFG)
    #define PWM_LED1_SIO_HYST_EN        (* (reg8 *) PWM_LED1__SIO_HYST_EN)
    #define PWM_LED1_SIO_REG_HIFREQ     (* (reg8 *) PWM_LED1__SIO_REG_HIFREQ)
    #define PWM_LED1_SIO_CFG            (* (reg8 *) PWM_LED1__SIO_CFG)
    #define PWM_LED1_SIO_DIFF           (* (reg8 *) PWM_LED1__SIO_DIFF)
#endif /* (PWM_LED1__SIO_CFG) */

/* Interrupt Registers */
#if defined(PWM_LED1__INTSTAT)
    #define PWM_LED1_INTSTAT            (* (reg8 *) PWM_LED1__INTSTAT)
    #define PWM_LED1_SNAP               (* (reg8 *) PWM_LED1__SNAP)
    
	#define PWM_LED1_0_INTTYPE_REG 		(* (reg8 *) PWM_LED1__0__INTTYPE)
#endif /* (PWM_LED1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PWM_LED1_H */


/* [] END OF FILE */
