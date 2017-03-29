/*******************************************************************************
* File Name: VRx.h  
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

#if !defined(CY_PINS_VRx_ALIASES_H) /* Pins VRx_ALIASES_H */
#define CY_PINS_VRx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define VRx_0			(VRx__0__PC)
#define VRx_0_PS		(VRx__0__PS)
#define VRx_0_PC		(VRx__0__PC)
#define VRx_0_DR		(VRx__0__DR)
#define VRx_0_SHIFT	(VRx__0__SHIFT)
#define VRx_0_INTR	((uint16)((uint16)0x0003u << (VRx__0__SHIFT*2u)))

#define VRx_INTR_ALL	 ((uint16)(VRx_0_INTR))


#endif /* End Pins VRx_ALIASES_H */


/* [] END OF FILE */
