/*******************************************************************************
* File Name: ShieldLed.h  
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

#if !defined(CY_PINS_ShieldLed_H) /* Pins ShieldLed_H */
#define CY_PINS_ShieldLed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ShieldLed_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ShieldLed__PORT == 15 && ((ShieldLed__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ShieldLed_Write(uint8 value);
void    ShieldLed_SetDriveMode(uint8 mode);
uint8   ShieldLed_ReadDataReg(void);
uint8   ShieldLed_Read(void);
void    ShieldLed_SetInterruptMode(uint16 position, uint16 mode);
uint8   ShieldLed_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ShieldLed_SetDriveMode() function.
     *  @{
     */
        #define ShieldLed_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ShieldLed_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ShieldLed_DM_RES_UP          PIN_DM_RES_UP
        #define ShieldLed_DM_RES_DWN         PIN_DM_RES_DWN
        #define ShieldLed_DM_OD_LO           PIN_DM_OD_LO
        #define ShieldLed_DM_OD_HI           PIN_DM_OD_HI
        #define ShieldLed_DM_STRONG          PIN_DM_STRONG
        #define ShieldLed_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ShieldLed_MASK               ShieldLed__MASK
#define ShieldLed_SHIFT              ShieldLed__SHIFT
#define ShieldLed_WIDTH              1u

/* Interrupt constants */
#if defined(ShieldLed__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ShieldLed_SetInterruptMode() function.
     *  @{
     */
        #define ShieldLed_INTR_NONE      (uint16)(0x0000u)
        #define ShieldLed_INTR_RISING    (uint16)(0x0001u)
        #define ShieldLed_INTR_FALLING   (uint16)(0x0002u)
        #define ShieldLed_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ShieldLed_INTR_MASK      (0x01u) 
#endif /* (ShieldLed__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ShieldLed_PS                     (* (reg8 *) ShieldLed__PS)
/* Data Register */
#define ShieldLed_DR                     (* (reg8 *) ShieldLed__DR)
/* Port Number */
#define ShieldLed_PRT_NUM                (* (reg8 *) ShieldLed__PRT) 
/* Connect to Analog Globals */                                                  
#define ShieldLed_AG                     (* (reg8 *) ShieldLed__AG)                       
/* Analog MUX bux enable */
#define ShieldLed_AMUX                   (* (reg8 *) ShieldLed__AMUX) 
/* Bidirectional Enable */                                                        
#define ShieldLed_BIE                    (* (reg8 *) ShieldLed__BIE)
/* Bit-mask for Aliased Register Access */
#define ShieldLed_BIT_MASK               (* (reg8 *) ShieldLed__BIT_MASK)
/* Bypass Enable */
#define ShieldLed_BYP                    (* (reg8 *) ShieldLed__BYP)
/* Port wide control signals */                                                   
#define ShieldLed_CTL                    (* (reg8 *) ShieldLed__CTL)
/* Drive Modes */
#define ShieldLed_DM0                    (* (reg8 *) ShieldLed__DM0) 
#define ShieldLed_DM1                    (* (reg8 *) ShieldLed__DM1)
#define ShieldLed_DM2                    (* (reg8 *) ShieldLed__DM2) 
/* Input Buffer Disable Override */
#define ShieldLed_INP_DIS                (* (reg8 *) ShieldLed__INP_DIS)
/* LCD Common or Segment Drive */
#define ShieldLed_LCD_COM_SEG            (* (reg8 *) ShieldLed__LCD_COM_SEG)
/* Enable Segment LCD */
#define ShieldLed_LCD_EN                 (* (reg8 *) ShieldLed__LCD_EN)
/* Slew Rate Control */
#define ShieldLed_SLW                    (* (reg8 *) ShieldLed__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ShieldLed_PRTDSI__CAPS_SEL       (* (reg8 *) ShieldLed__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ShieldLed_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ShieldLed__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ShieldLed_PRTDSI__OE_SEL0        (* (reg8 *) ShieldLed__PRTDSI__OE_SEL0) 
#define ShieldLed_PRTDSI__OE_SEL1        (* (reg8 *) ShieldLed__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ShieldLed_PRTDSI__OUT_SEL0       (* (reg8 *) ShieldLed__PRTDSI__OUT_SEL0) 
#define ShieldLed_PRTDSI__OUT_SEL1       (* (reg8 *) ShieldLed__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ShieldLed_PRTDSI__SYNC_OUT       (* (reg8 *) ShieldLed__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ShieldLed__SIO_CFG)
    #define ShieldLed_SIO_HYST_EN        (* (reg8 *) ShieldLed__SIO_HYST_EN)
    #define ShieldLed_SIO_REG_HIFREQ     (* (reg8 *) ShieldLed__SIO_REG_HIFREQ)
    #define ShieldLed_SIO_CFG            (* (reg8 *) ShieldLed__SIO_CFG)
    #define ShieldLed_SIO_DIFF           (* (reg8 *) ShieldLed__SIO_DIFF)
#endif /* (ShieldLed__SIO_CFG) */

/* Interrupt Registers */
#if defined(ShieldLed__INTSTAT)
    #define ShieldLed_INTSTAT            (* (reg8 *) ShieldLed__INTSTAT)
    #define ShieldLed_SNAP               (* (reg8 *) ShieldLed__SNAP)
    
	#define ShieldLed_0_INTTYPE_REG 		(* (reg8 *) ShieldLed__0__INTTYPE)
#endif /* (ShieldLed__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ShieldLed_H */


/* [] END OF FILE */
