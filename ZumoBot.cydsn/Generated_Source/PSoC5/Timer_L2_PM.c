/*******************************************************************************
* File Name: Timer_L2_PM.c
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

#include "Timer_L2.h"

static Timer_L2_backupStruct Timer_L2_backup;


/*******************************************************************************
* Function Name: Timer_L2_SaveConfig
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
*  Timer_L2_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_L2_SaveConfig(void) 
{
    #if (!Timer_L2_UsingFixedFunction)
        Timer_L2_backup.TimerUdb = Timer_L2_ReadCounter();
        Timer_L2_backup.InterruptMaskValue = Timer_L2_STATUS_MASK;
        #if (Timer_L2_UsingHWCaptureCounter)
            Timer_L2_backup.TimerCaptureCounter = Timer_L2_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_L2_UDB_CONTROL_REG_REMOVED)
            Timer_L2_backup.TimerControlRegister = Timer_L2_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_L2_RestoreConfig
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
*  Timer_L2_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_L2_RestoreConfig(void) 
{   
    #if (!Timer_L2_UsingFixedFunction)

        Timer_L2_WriteCounter(Timer_L2_backup.TimerUdb);
        Timer_L2_STATUS_MASK =Timer_L2_backup.InterruptMaskValue;
        #if (Timer_L2_UsingHWCaptureCounter)
            Timer_L2_SetCaptureCount(Timer_L2_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_L2_UDB_CONTROL_REG_REMOVED)
            Timer_L2_WriteControlRegister(Timer_L2_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_L2_Sleep
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
*  Timer_L2_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_L2_Sleep(void) 
{
    #if(!Timer_L2_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_L2_CTRL_ENABLE == (Timer_L2_CONTROL & Timer_L2_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_L2_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_L2_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_L2_Stop();
    Timer_L2_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_L2_Wakeup
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
*  Timer_L2_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_L2_Wakeup(void) 
{
    Timer_L2_RestoreConfig();
    #if(!Timer_L2_UDB_CONTROL_REG_REMOVED)
        if(Timer_L2_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_L2_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
