/*******************************************************************************
* File Name: ADC_Battery_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_Battery.h"


/***************************************
* Local data allocation
***************************************/

static ADC_Battery_BACKUP_STRUCT  ADC_Battery_backup =
{
    ADC_Battery_DISABLED
};


/*******************************************************************************
* Function Name: ADC_Battery_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Battery_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Battery_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Battery_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Battery_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_Battery_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Battery_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_Battery_Sleep(void)
{
    if((ADC_Battery_PWRMGR_SAR_REG  & ADC_Battery_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_Battery_SAR_CSR0_REG & ADC_Battery_SAR_SOF_START_CONV) != 0u)
        {
            ADC_Battery_backup.enableState = ADC_Battery_ENABLED | ADC_Battery_STARTED;
        }
        else
        {
            ADC_Battery_backup.enableState = ADC_Battery_ENABLED;
        }
        ADC_Battery_Stop();
    }
    else
    {
        ADC_Battery_backup.enableState = ADC_Battery_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_Battery_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_Battery_Sleep() was called. If the component was enabled before the
*  ADC_Battery_Sleep() function was called, the
*  ADC_Battery_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Battery_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_Battery_Wakeup(void)
{
    if(ADC_Battery_backup.enableState != ADC_Battery_DISABLED)
    {
        ADC_Battery_Enable();
        #if(ADC_Battery_DEFAULT_CONV_MODE != ADC_Battery__HARDWARE_TRIGGER)
            if((ADC_Battery_backup.enableState & ADC_Battery_STARTED) != 0u)
            {
                ADC_Battery_StartConvert();
            }
        #endif /* End ADC_Battery_DEFAULT_CONV_MODE != ADC_Battery__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
