/*******************************************************************************
* File Name: VRx.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "VRx.h"

static VRx_BACKUP_STRUCT  VRx_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: VRx_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet VRx_SUT.c usage_VRx_Sleep_Wakeup
*******************************************************************************/
void VRx_Sleep(void)
{
    #if defined(VRx__PC)
        VRx_backup.pcState = VRx_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            VRx_backup.usbState = VRx_CR1_REG;
            VRx_USB_POWER_REG |= VRx_USBIO_ENTER_SLEEP;
            VRx_CR1_REG &= VRx_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(VRx__SIO)
        VRx_backup.sioState = VRx_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        VRx_SIO_REG &= (uint32)(~VRx_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: VRx_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to VRx_Sleep() for an example usage.
*******************************************************************************/
void VRx_Wakeup(void)
{
    #if defined(VRx__PC)
        VRx_PC = VRx_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            VRx_USB_POWER_REG &= VRx_USBIO_EXIT_SLEEP_PH1;
            VRx_CR1_REG = VRx_backup.usbState;
            VRx_USB_POWER_REG &= VRx_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(VRx__SIO)
        VRx_SIO_REG = VRx_backup.sioState;
    #endif
}


/* [] END OF FILE */
