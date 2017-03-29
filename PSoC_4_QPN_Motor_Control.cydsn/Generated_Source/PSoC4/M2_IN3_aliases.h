/*******************************************************************************
* File Name: M2_IN3.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_M2_IN3_ALIASES_H) /* Pins M2_IN3_ALIASES_H */
#define CY_PINS_M2_IN3_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define M2_IN3_0			(M2_IN3__0__PC)
#define M2_IN3_0_PS		(M2_IN3__0__PS)
#define M2_IN3_0_PC		(M2_IN3__0__PC)
#define M2_IN3_0_DR		(M2_IN3__0__DR)
#define M2_IN3_0_SHIFT	(M2_IN3__0__SHIFT)
#define M2_IN3_0_INTR	((uint16)((uint16)0x0003u << (M2_IN3__0__SHIFT*2u)))

#define M2_IN3_INTR_ALL	 ((uint16)(M2_IN3_0_INTR))


#endif /* End Pins M2_IN3_ALIASES_H */


/* [] END OF FILE */
