/*******************************************************************************
* File Name: MotorPwmRight.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MotorPwmRight_H) /* Pins MotorPwmRight_H */
#define CY_PINS_MotorPwmRight_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MotorPwmRight_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MotorPwmRight__PORT == 15 && ((MotorPwmRight__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MotorPwmRight_Write(uint8 value);
void    MotorPwmRight_SetDriveMode(uint8 mode);
uint8   MotorPwmRight_ReadDataReg(void);
uint8   MotorPwmRight_Read(void);
void    MotorPwmRight_SetInterruptMode(uint16 position, uint16 mode);
uint8   MotorPwmRight_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MotorPwmRight_SetDriveMode() function.
     *  @{
     */
        #define MotorPwmRight_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MotorPwmRight_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MotorPwmRight_DM_RES_UP          PIN_DM_RES_UP
        #define MotorPwmRight_DM_RES_DWN         PIN_DM_RES_DWN
        #define MotorPwmRight_DM_OD_LO           PIN_DM_OD_LO
        #define MotorPwmRight_DM_OD_HI           PIN_DM_OD_HI
        #define MotorPwmRight_DM_STRONG          PIN_DM_STRONG
        #define MotorPwmRight_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MotorPwmRight_MASK               MotorPwmRight__MASK
#define MotorPwmRight_SHIFT              MotorPwmRight__SHIFT
#define MotorPwmRight_WIDTH              1u

/* Interrupt constants */
#if defined(MotorPwmRight__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MotorPwmRight_SetInterruptMode() function.
     *  @{
     */
        #define MotorPwmRight_INTR_NONE      (uint16)(0x0000u)
        #define MotorPwmRight_INTR_RISING    (uint16)(0x0001u)
        #define MotorPwmRight_INTR_FALLING   (uint16)(0x0002u)
        #define MotorPwmRight_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MotorPwmRight_INTR_MASK      (0x01u) 
#endif /* (MotorPwmRight__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorPwmRight_PS                     (* (reg8 *) MotorPwmRight__PS)
/* Data Register */
#define MotorPwmRight_DR                     (* (reg8 *) MotorPwmRight__DR)
/* Port Number */
#define MotorPwmRight_PRT_NUM                (* (reg8 *) MotorPwmRight__PRT) 
/* Connect to Analog Globals */                                                  
#define MotorPwmRight_AG                     (* (reg8 *) MotorPwmRight__AG)                       
/* Analog MUX bux enable */
#define MotorPwmRight_AMUX                   (* (reg8 *) MotorPwmRight__AMUX) 
/* Bidirectional Enable */                                                        
#define MotorPwmRight_BIE                    (* (reg8 *) MotorPwmRight__BIE)
/* Bit-mask for Aliased Register Access */
#define MotorPwmRight_BIT_MASK               (* (reg8 *) MotorPwmRight__BIT_MASK)
/* Bypass Enable */
#define MotorPwmRight_BYP                    (* (reg8 *) MotorPwmRight__BYP)
/* Port wide control signals */                                                   
#define MotorPwmRight_CTL                    (* (reg8 *) MotorPwmRight__CTL)
/* Drive Modes */
#define MotorPwmRight_DM0                    (* (reg8 *) MotorPwmRight__DM0) 
#define MotorPwmRight_DM1                    (* (reg8 *) MotorPwmRight__DM1)
#define MotorPwmRight_DM2                    (* (reg8 *) MotorPwmRight__DM2) 
/* Input Buffer Disable Override */
#define MotorPwmRight_INP_DIS                (* (reg8 *) MotorPwmRight__INP_DIS)
/* LCD Common or Segment Drive */
#define MotorPwmRight_LCD_COM_SEG            (* (reg8 *) MotorPwmRight__LCD_COM_SEG)
/* Enable Segment LCD */
#define MotorPwmRight_LCD_EN                 (* (reg8 *) MotorPwmRight__LCD_EN)
/* Slew Rate Control */
#define MotorPwmRight_SLW                    (* (reg8 *) MotorPwmRight__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MotorPwmRight_PRTDSI__CAPS_SEL       (* (reg8 *) MotorPwmRight__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MotorPwmRight_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MotorPwmRight__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MotorPwmRight_PRTDSI__OE_SEL0        (* (reg8 *) MotorPwmRight__PRTDSI__OE_SEL0) 
#define MotorPwmRight_PRTDSI__OE_SEL1        (* (reg8 *) MotorPwmRight__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MotorPwmRight_PRTDSI__OUT_SEL0       (* (reg8 *) MotorPwmRight__PRTDSI__OUT_SEL0) 
#define MotorPwmRight_PRTDSI__OUT_SEL1       (* (reg8 *) MotorPwmRight__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MotorPwmRight_PRTDSI__SYNC_OUT       (* (reg8 *) MotorPwmRight__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MotorPwmRight__SIO_CFG)
    #define MotorPwmRight_SIO_HYST_EN        (* (reg8 *) MotorPwmRight__SIO_HYST_EN)
    #define MotorPwmRight_SIO_REG_HIFREQ     (* (reg8 *) MotorPwmRight__SIO_REG_HIFREQ)
    #define MotorPwmRight_SIO_CFG            (* (reg8 *) MotorPwmRight__SIO_CFG)
    #define MotorPwmRight_SIO_DIFF           (* (reg8 *) MotorPwmRight__SIO_DIFF)
#endif /* (MotorPwmRight__SIO_CFG) */

/* Interrupt Registers */
#if defined(MotorPwmRight__INTSTAT)
    #define MotorPwmRight_INTSTAT            (* (reg8 *) MotorPwmRight__INTSTAT)
    #define MotorPwmRight_SNAP               (* (reg8 *) MotorPwmRight__SNAP)
    
	#define MotorPwmRight_0_INTTYPE_REG 		(* (reg8 *) MotorPwmRight__0__INTTYPE)
#endif /* (MotorPwmRight__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MotorPwmRight_H */


/* [] END OF FILE */
