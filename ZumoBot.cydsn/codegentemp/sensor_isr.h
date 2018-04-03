/*******************************************************************************
* File Name: sensor_isr.h
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
#if !defined(CY_ISR_sensor_isr_H)
#define CY_ISR_sensor_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void sensor_isr_Start(void);
void sensor_isr_StartEx(cyisraddress address);
void sensor_isr_Stop(void);

CY_ISR_PROTO(sensor_isr_Interrupt);

void sensor_isr_SetVector(cyisraddress address);
cyisraddress sensor_isr_GetVector(void);

void sensor_isr_SetPriority(uint8 priority);
uint8 sensor_isr_GetPriority(void);

void sensor_isr_Enable(void);
uint8 sensor_isr_GetState(void);
void sensor_isr_Disable(void);

void sensor_isr_SetPending(void);
void sensor_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the sensor_isr ISR. */
#define sensor_isr_INTC_VECTOR            ((reg32 *) sensor_isr__INTC_VECT)

/* Address of the sensor_isr ISR priority. */
#define sensor_isr_INTC_PRIOR             ((reg8 *) sensor_isr__INTC_PRIOR_REG)

/* Priority of the sensor_isr interrupt. */
#define sensor_isr_INTC_PRIOR_NUMBER      sensor_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable sensor_isr interrupt. */
#define sensor_isr_INTC_SET_EN            ((reg32 *) sensor_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the sensor_isr interrupt. */
#define sensor_isr_INTC_CLR_EN            ((reg32 *) sensor_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the sensor_isr interrupt state to pending. */
#define sensor_isr_INTC_SET_PD            ((reg32 *) sensor_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the sensor_isr interrupt. */
#define sensor_isr_INTC_CLR_PD            ((reg32 *) sensor_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_sensor_isr_H */


/* [] END OF FILE */
