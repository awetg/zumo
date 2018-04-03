/*******************************************************************************
* File Name: IR_isr.h
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
#if !defined(CY_ISR_IR_isr_H)
#define CY_ISR_IR_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void IR_isr_Start(void);
void IR_isr_StartEx(cyisraddress address);
void IR_isr_Stop(void);

CY_ISR_PROTO(IR_isr_Interrupt);

void IR_isr_SetVector(cyisraddress address);
cyisraddress IR_isr_GetVector(void);

void IR_isr_SetPriority(uint8 priority);
uint8 IR_isr_GetPriority(void);

void IR_isr_Enable(void);
uint8 IR_isr_GetState(void);
void IR_isr_Disable(void);

void IR_isr_SetPending(void);
void IR_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the IR_isr ISR. */
#define IR_isr_INTC_VECTOR            ((reg32 *) IR_isr__INTC_VECT)

/* Address of the IR_isr ISR priority. */
#define IR_isr_INTC_PRIOR             ((reg8 *) IR_isr__INTC_PRIOR_REG)

/* Priority of the IR_isr interrupt. */
#define IR_isr_INTC_PRIOR_NUMBER      IR_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable IR_isr interrupt. */
#define IR_isr_INTC_SET_EN            ((reg32 *) IR_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the IR_isr interrupt. */
#define IR_isr_INTC_CLR_EN            ((reg32 *) IR_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the IR_isr interrupt state to pending. */
#define IR_isr_INTC_SET_PD            ((reg32 *) IR_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the IR_isr interrupt. */
#define IR_isr_INTC_CLR_PD            ((reg32 *) IR_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_IR_isr_H */


/* [] END OF FILE */
