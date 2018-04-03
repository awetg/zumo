/*******************************************************************************
* File Name: MotorDirRight.h  
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

#if !defined(CY_PINS_MotorDirRight_H) /* Pins MotorDirRight_H */
#define CY_PINS_MotorDirRight_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MotorDirRight_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MotorDirRight__PORT == 15 && ((MotorDirRight__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MotorDirRight_Write(uint8 value);
void    MotorDirRight_SetDriveMode(uint8 mode);
uint8   MotorDirRight_ReadDataReg(void);
uint8   MotorDirRight_Read(void);
void    MotorDirRight_SetInterruptMode(uint16 position, uint16 mode);
uint8   MotorDirRight_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MotorDirRight_SetDriveMode() function.
     *  @{
     */
        #define MotorDirRight_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MotorDirRight_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MotorDirRight_DM_RES_UP          PIN_DM_RES_UP
        #define MotorDirRight_DM_RES_DWN         PIN_DM_RES_DWN
        #define MotorDirRight_DM_OD_LO           PIN_DM_OD_LO
        #define MotorDirRight_DM_OD_HI           PIN_DM_OD_HI
        #define MotorDirRight_DM_STRONG          PIN_DM_STRONG
        #define MotorDirRight_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MotorDirRight_MASK               MotorDirRight__MASK
#define MotorDirRight_SHIFT              MotorDirRight__SHIFT
#define MotorDirRight_WIDTH              1u

/* Interrupt constants */
#if defined(MotorDirRight__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MotorDirRight_SetInterruptMode() function.
     *  @{
     */
        #define MotorDirRight_INTR_NONE      (uint16)(0x0000u)
        #define MotorDirRight_INTR_RISING    (uint16)(0x0001u)
        #define MotorDirRight_INTR_FALLING   (uint16)(0x0002u)
        #define MotorDirRight_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MotorDirRight_INTR_MASK      (0x01u) 
#endif /* (MotorDirRight__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorDirRight_PS                     (* (reg8 *) MotorDirRight__PS)
/* Data Register */
#define MotorDirRight_DR                     (* (reg8 *) MotorDirRight__DR)
/* Port Number */
#define MotorDirRight_PRT_NUM                (* (reg8 *) MotorDirRight__PRT) 
/* Connect to Analog Globals */                                                  
#define MotorDirRight_AG                     (* (reg8 *) MotorDirRight__AG)                       
/* Analog MUX bux enable */
#define MotorDirRight_AMUX                   (* (reg8 *) MotorDirRight__AMUX) 
/* Bidirectional Enable */                                                        
#define MotorDirRight_BIE                    (* (reg8 *) MotorDirRight__BIE)
/* Bit-mask for Aliased Register Access */
#define MotorDirRight_BIT_MASK               (* (reg8 *) MotorDirRight__BIT_MASK)
/* Bypass Enable */
#define MotorDirRight_BYP                    (* (reg8 *) MotorDirRight__BYP)
/* Port wide control signals */                                                   
#define MotorDirRight_CTL                    (* (reg8 *) MotorDirRight__CTL)
/* Drive Modes */
#define MotorDirRight_DM0                    (* (reg8 *) MotorDirRight__DM0) 
#define MotorDirRight_DM1                    (* (reg8 *) MotorDirRight__DM1)
#define MotorDirRight_DM2                    (* (reg8 *) MotorDirRight__DM2) 
/* Input Buffer Disable Override */
#define MotorDirRight_INP_DIS                (* (reg8 *) MotorDirRight__INP_DIS)
/* LCD Common or Segment Drive */
#define MotorDirRight_LCD_COM_SEG            (* (reg8 *) MotorDirRight__LCD_COM_SEG)
/* Enable Segment LCD */
#define MotorDirRight_LCD_EN                 (* (reg8 *) MotorDirRight__LCD_EN)
/* Slew Rate Control */
#define MotorDirRight_SLW                    (* (reg8 *) MotorDirRight__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MotorDirRight_PRTDSI__CAPS_SEL       (* (reg8 *) MotorDirRight__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MotorDirRight_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MotorDirRight__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MotorDirRight_PRTDSI__OE_SEL0        (* (reg8 *) MotorDirRight__PRTDSI__OE_SEL0) 
#define MotorDirRight_PRTDSI__OE_SEL1        (* (reg8 *) MotorDirRight__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MotorDirRight_PRTDSI__OUT_SEL0       (* (reg8 *) MotorDirRight__PRTDSI__OUT_SEL0) 
#define MotorDirRight_PRTDSI__OUT_SEL1       (* (reg8 *) MotorDirRight__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MotorDirRight_PRTDSI__SYNC_OUT       (* (reg8 *) MotorDirRight__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MotorDirRight__SIO_CFG)
    #define MotorDirRight_SIO_HYST_EN        (* (reg8 *) MotorDirRight__SIO_HYST_EN)
    #define MotorDirRight_SIO_REG_HIFREQ     (* (reg8 *) MotorDirRight__SIO_REG_HIFREQ)
    #define MotorDirRight_SIO_CFG            (* (reg8 *) MotorDirRight__SIO_CFG)
    #define MotorDirRight_SIO_DIFF           (* (reg8 *) MotorDirRight__SIO_DIFF)
#endif /* (MotorDirRight__SIO_CFG) */

/* Interrupt Registers */
#if defined(MotorDirRight__INTSTAT)
    #define MotorDirRight_INTSTAT            (* (reg8 *) MotorDirRight__INTSTAT)
    #define MotorDirRight_SNAP               (* (reg8 *) MotorDirRight__SNAP)
    
	#define MotorDirRight_0_INTTYPE_REG 		(* (reg8 *) MotorDirRight__0__INTTYPE)
#endif /* (MotorDirRight__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MotorDirRight_H */


/* [] END OF FILE */
