/*******************************************************************************
* File Name: ultra_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_ultra_isr_H)
#define CY_ISR_ultra_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ultra_isr_Start(void);
void ultra_isr_StartEx(cyisraddress address);
void ultra_isr_Stop(void);

CY_ISR_PROTO(ultra_isr_Interrupt);

void ultra_isr_SetVector(cyisraddress address);
cyisraddress ultra_isr_GetVector(void);

void ultra_isr_SetPriority(uint8 priority);
uint8 ultra_isr_GetPriority(void);

void ultra_isr_Enable(void);
uint8 ultra_isr_GetState(void);
void ultra_isr_Disable(void);

void ultra_isr_SetPending(void);
void ultra_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ultra_isr ISR. */
#define ultra_isr_INTC_VECTOR            ((reg32 *) ultra_isr__INTC_VECT)

/* Address of the ultra_isr ISR priority. */
#define ultra_isr_INTC_PRIOR             ((reg8 *) ultra_isr__INTC_PRIOR_REG)

/* Priority of the ultra_isr interrupt. */
#define ultra_isr_INTC_PRIOR_NUMBER      ultra_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ultra_isr interrupt. */
#define ultra_isr_INTC_SET_EN            ((reg32 *) ultra_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ultra_isr interrupt. */
#define ultra_isr_INTC_CLR_EN            ((reg32 *) ultra_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ultra_isr interrupt state to pending. */
#define ultra_isr_INTC_SET_PD            ((reg32 *) ultra_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ultra_isr interrupt. */
#define ultra_isr_INTC_CLR_PD            ((reg32 *) ultra_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_ultra_isr_H */


/* [] END OF FILE */
