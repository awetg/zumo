/*******************************************************************************
* File Name: Timer_L3_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_L3.h"

static Timer_L3_backupStruct Timer_L3_backup;


/*******************************************************************************
* Function Name: Timer_L3_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_L3_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_L3_SaveConfig(void) 
{
    #if (!Timer_L3_UsingFixedFunction)
        Timer_L3_backup.TimerUdb = Timer_L3_ReadCounter();
        Timer_L3_backup.InterruptMaskValue = Timer_L3_STATUS_MASK;
        #if (Timer_L3_UsingHWCaptureCounter)
            Timer_L3_backup.TimerCaptureCounter = Timer_L3_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_L3_UDB_CONTROL_REG_REMOVED)
            Timer_L3_backup.TimerControlRegister = Timer_L3_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_L3_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_L3_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_L3_RestoreConfig(void) 
{   
    #if (!Timer_L3_UsingFixedFunction)

        Timer_L3_WriteCounter(Timer_L3_backup.TimerUdb);
        Timer_L3_STATUS_MASK =Timer_L3_backup.InterruptMaskValue;
        #if (Timer_L3_UsingHWCaptureCounter)
            Timer_L3_SetCaptureCount(Timer_L3_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_L3_UDB_CONTROL_REG_REMOVED)
            Timer_L3_WriteControlRegister(Timer_L3_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_L3_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_L3_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_L3_Sleep(void) 
{
    #if(!Timer_L3_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_L3_CTRL_ENABLE == (Timer_L3_CONTROL & Timer_L3_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_L3_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_L3_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_L3_Stop();
    Timer_L3_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_L3_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_L3_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_L3_Wakeup(void) 
{
    Timer_L3_RestoreConfig();
    #if(!Timer_L3_UDB_CONTROL_REG_REMOVED)
        if(Timer_L3_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_L3_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
