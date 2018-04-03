/*******************************************************************************
* File Name: Buzzer_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Buzzer_PWM.h"

static Buzzer_PWM_backupStruct Buzzer_PWM_backup;


/*******************************************************************************
* Function Name: Buzzer_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Buzzer_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Buzzer_PWM_SaveConfig(void) 
{

    #if(!Buzzer_PWM_UsingFixedFunction)
        #if(!Buzzer_PWM_PWMModeIsCenterAligned)
            Buzzer_PWM_backup.PWMPeriod = Buzzer_PWM_ReadPeriod();
        #endif /* (!Buzzer_PWM_PWMModeIsCenterAligned) */
        Buzzer_PWM_backup.PWMUdb = Buzzer_PWM_ReadCounter();
        #if (Buzzer_PWM_UseStatus)
            Buzzer_PWM_backup.InterruptMaskValue = Buzzer_PWM_STATUS_MASK;
        #endif /* (Buzzer_PWM_UseStatus) */

        #if(Buzzer_PWM_DeadBandMode == Buzzer_PWM__B_PWM__DBM_256_CLOCKS || \
            Buzzer_PWM_DeadBandMode == Buzzer_PWM__B_PWM__DBM_2_4_CLOCKS)
            Buzzer_PWM_backup.PWMdeadBandValue = Buzzer_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Buzzer_PWM_KillModeMinTime)
             Buzzer_PWM_backup.PWMKillCounterPeriod = Buzzer_PWM_ReadKillTime();
        #endif /* (Buzzer_PWM_KillModeMinTime) */

        #if(Buzzer_PWM_UseControl)
            Buzzer_PWM_backup.PWMControlRegister = Buzzer_PWM_ReadControlRegister();
        #endif /* (Buzzer_PWM_UseControl) */
    #endif  /* (!Buzzer_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Buzzer_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Buzzer_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Buzzer_PWM_RestoreConfig(void) 
{
        #if(!Buzzer_PWM_UsingFixedFunction)
            #if(!Buzzer_PWM_PWMModeIsCenterAligned)
                Buzzer_PWM_WritePeriod(Buzzer_PWM_backup.PWMPeriod);
            #endif /* (!Buzzer_PWM_PWMModeIsCenterAligned) */

            Buzzer_PWM_WriteCounter(Buzzer_PWM_backup.PWMUdb);

            #if (Buzzer_PWM_UseStatus)
                Buzzer_PWM_STATUS_MASK = Buzzer_PWM_backup.InterruptMaskValue;
            #endif /* (Buzzer_PWM_UseStatus) */

            #if(Buzzer_PWM_DeadBandMode == Buzzer_PWM__B_PWM__DBM_256_CLOCKS || \
                Buzzer_PWM_DeadBandMode == Buzzer_PWM__B_PWM__DBM_2_4_CLOCKS)
                Buzzer_PWM_WriteDeadTime(Buzzer_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Buzzer_PWM_KillModeMinTime)
                Buzzer_PWM_WriteKillTime(Buzzer_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Buzzer_PWM_KillModeMinTime) */

            #if(Buzzer_PWM_UseControl)
                Buzzer_PWM_WriteControlRegister(Buzzer_PWM_backup.PWMControlRegister);
            #endif /* (Buzzer_PWM_UseControl) */
        #endif  /* (!Buzzer_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Buzzer_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Buzzer_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Buzzer_PWM_Sleep(void) 
{
    #if(Buzzer_PWM_UseControl)
        if(Buzzer_PWM_CTRL_ENABLE == (Buzzer_PWM_CONTROL & Buzzer_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Buzzer_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Buzzer_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Buzzer_PWM_UseControl) */

    /* Stop component */
    Buzzer_PWM_Stop();

    /* Save registers configuration */
    Buzzer_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Buzzer_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Buzzer_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Buzzer_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Buzzer_PWM_RestoreConfig();

    if(Buzzer_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Buzzer_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
