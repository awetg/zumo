/*******************************************************************************
* File Name: L3.h  
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

#if !defined(CY_PINS_L3_H) /* Pins L3_H */
#define CY_PINS_L3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "L3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 L3__PORT == 15 && ((L3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    L3_Write(uint8 value);
void    L3_SetDriveMode(uint8 mode);
uint8   L3_ReadDataReg(void);
uint8   L3_Read(void);
void    L3_SetInterruptMode(uint16 position, uint16 mode);
uint8   L3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the L3_SetDriveMode() function.
     *  @{
     */
        #define L3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define L3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define L3_DM_RES_UP          PIN_DM_RES_UP
        #define L3_DM_RES_DWN         PIN_DM_RES_DWN
        #define L3_DM_OD_LO           PIN_DM_OD_LO
        #define L3_DM_OD_HI           PIN_DM_OD_HI
        #define L3_DM_STRONG          PIN_DM_STRONG
        #define L3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define L3_MASK               L3__MASK
#define L3_SHIFT              L3__SHIFT
#define L3_WIDTH              1u

/* Interrupt constants */
#if defined(L3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in L3_SetInterruptMode() function.
     *  @{
     */
        #define L3_INTR_NONE      (uint16)(0x0000u)
        #define L3_INTR_RISING    (uint16)(0x0001u)
        #define L3_INTR_FALLING   (uint16)(0x0002u)
        #define L3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define L3_INTR_MASK      (0x01u) 
#endif /* (L3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define L3_PS                     (* (reg8 *) L3__PS)
/* Data Register */
#define L3_DR                     (* (reg8 *) L3__DR)
/* Port Number */
#define L3_PRT_NUM                (* (reg8 *) L3__PRT) 
/* Connect to Analog Globals */                                                  
#define L3_AG                     (* (reg8 *) L3__AG)                       
/* Analog MUX bux enable */
#define L3_AMUX                   (* (reg8 *) L3__AMUX) 
/* Bidirectional Enable */                                                        
#define L3_BIE                    (* (reg8 *) L3__BIE)
/* Bit-mask for Aliased Register Access */
#define L3_BIT_MASK               (* (reg8 *) L3__BIT_MASK)
/* Bypass Enable */
#define L3_BYP                    (* (reg8 *) L3__BYP)
/* Port wide control signals */                                                   
#define L3_CTL                    (* (reg8 *) L3__CTL)
/* Drive Modes */
#define L3_DM0                    (* (reg8 *) L3__DM0) 
#define L3_DM1                    (* (reg8 *) L3__DM1)
#define L3_DM2                    (* (reg8 *) L3__DM2) 
/* Input Buffer Disable Override */
#define L3_INP_DIS                (* (reg8 *) L3__INP_DIS)
/* LCD Common or Segment Drive */
#define L3_LCD_COM_SEG            (* (reg8 *) L3__LCD_COM_SEG)
/* Enable Segment LCD */
#define L3_LCD_EN                 (* (reg8 *) L3__LCD_EN)
/* Slew Rate Control */
#define L3_SLW                    (* (reg8 *) L3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define L3_PRTDSI__CAPS_SEL       (* (reg8 *) L3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define L3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) L3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define L3_PRTDSI__OE_SEL0        (* (reg8 *) L3__PRTDSI__OE_SEL0) 
#define L3_PRTDSI__OE_SEL1        (* (reg8 *) L3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define L3_PRTDSI__OUT_SEL0       (* (reg8 *) L3__PRTDSI__OUT_SEL0) 
#define L3_PRTDSI__OUT_SEL1       (* (reg8 *) L3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define L3_PRTDSI__SYNC_OUT       (* (reg8 *) L3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(L3__SIO_CFG)
    #define L3_SIO_HYST_EN        (* (reg8 *) L3__SIO_HYST_EN)
    #define L3_SIO_REG_HIFREQ     (* (reg8 *) L3__SIO_REG_HIFREQ)
    #define L3_SIO_CFG            (* (reg8 *) L3__SIO_CFG)
    #define L3_SIO_DIFF           (* (reg8 *) L3__SIO_DIFF)
#endif /* (L3__SIO_CFG) */

/* Interrupt Registers */
#if defined(L3__INTSTAT)
    #define L3_INTSTAT            (* (reg8 *) L3__INTSTAT)
    #define L3_SNAP               (* (reg8 *) L3__SNAP)
    
	#define L3_0_INTTYPE_REG 		(* (reg8 *) L3__0__INTTYPE)
#endif /* (L3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_L3_H */


/* [] END OF FILE */
