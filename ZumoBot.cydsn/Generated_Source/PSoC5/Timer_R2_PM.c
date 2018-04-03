/*******************************************************************************
* File Name: Timer_R2_PM.c
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

#include "Timer_R2.h"

static Timer_R2_backupStruct Timer_R2_backup;


/*******************************************************************************
* Function Name: Timer_R2_SaveConfig
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
*  Timer_R2_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_R2_SaveConfig(void) 
{
    #if (!Timer_R2_UsingFixedFunction)
        Timer_R2_backup.TimerUdb = Timer_R2_ReadCounter();
        Timer_R2_backup.InterruptMaskValue = Timer_R2_STATUS_MASK;
        #if (Timer_R2_UsingHWCaptureCounter)
            Timer_R2_backup.TimerCaptureCounter = Timer_R2_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_R2_UDB_CONTROL_REG_REMOVED)
            Timer_R2_backup.TimerControlRegister = Timer_R2_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_R2_RestoreConfig
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
*  Timer_R2_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_R2_RestoreConfig(void) 
{   
    #if (!Timer_R2_UsingFixedFunction)

        Timer_R2_WriteCounter(Timer_R2_backup.TimerUdb);
        Timer_R2_STATUS_MASK =Timer_R2_backup.InterruptMaskValue;
        #if (Timer_R2_UsingHWCaptureCounter)
            Timer_R2_SetCaptureCount(Timer_R2_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_R2_UDB_CONTROL_REG_REMOVED)
            Timer_R2_WriteControlRegister(Timer_R2_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_R2_Sleep
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
*  Timer_R2_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_R2_Sleep(void) 
{
    #if(!Timer_R2_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_R2_CTRL_ENABLE == (Timer_R2_CONTROL & Timer_R2_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_R2_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_R2_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_R2_Stop();
    Timer_R2_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_R2_Wakeup
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
*  Timer_R2_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_R2_Wakeup(void) 
{
    Timer_R2_RestoreConfig();
    #if(!Timer_R2_UDB_CONTROL_REG_REMOVED)
        if(Timer_R2_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_R2_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
