/*******************************************************************************
* File Name: R2.h  
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

#if !defined(CY_PINS_R2_H) /* Pins R2_H */
#define CY_PINS_R2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "R2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 R2__PORT == 15 && ((R2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    R2_Write(uint8 value);
void    R2_SetDriveMode(uint8 mode);
uint8   R2_ReadDataReg(void);
uint8   R2_Read(void);
void    R2_SetInterruptMode(uint16 position, uint16 mode);
uint8   R2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the R2_SetDriveMode() function.
     *  @{
     */
        #define R2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define R2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define R2_DM_RES_UP          PIN_DM_RES_UP
        #define R2_DM_RES_DWN         PIN_DM_RES_DWN
        #define R2_DM_OD_LO           PIN_DM_OD_LO
        #define R2_DM_OD_HI           PIN_DM_OD_HI
        #define R2_DM_STRONG          PIN_DM_STRONG
        #define R2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define R2_MASK               R2__MASK
#define R2_SHIFT              R2__SHIFT
#define R2_WIDTH              1u

/* Interrupt constants */
#if defined(R2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in R2_SetInterruptMode() function.
     *  @{
     */
        #define R2_INTR_NONE      (uint16)(0x0000u)
        #define R2_INTR_RISING    (uint16)(0x0001u)
        #define R2_INTR_FALLING   (uint16)(0x0002u)
        #define R2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define R2_INTR_MASK      (0x01u) 
#endif /* (R2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define R2_PS                     (* (reg8 *) R2__PS)
/* Data Register */
#define R2_DR                     (* (reg8 *) R2__DR)
/* Port Number */
#define R2_PRT_NUM                (* (reg8 *) R2__PRT) 
/* Connect to Analog Globals */                                                  
#define R2_AG                     (* (reg8 *) R2__AG)                       
/* Analog MUX bux enable */
#define R2_AMUX                   (* (reg8 *) R2__AMUX) 
/* Bidirectional Enable */                                                        
#define R2_BIE                    (* (reg8 *) R2__BIE)
/* Bit-mask for Aliased Register Access */
#define R2_BIT_MASK               (* (reg8 *) R2__BIT_MASK)
/* Bypass Enable */
#define R2_BYP                    (* (reg8 *) R2__BYP)
/* Port wide control signals */                                                   
#define R2_CTL                    (* (reg8 *) R2__CTL)
/* Drive Modes */
#define R2_DM0                    (* (reg8 *) R2__DM0) 
#define R2_DM1                    (* (reg8 *) R2__DM1)
#define R2_DM2                    (* (reg8 *) R2__DM2) 
/* Input Buffer Disable Override */
#define R2_INP_DIS                (* (reg8 *) R2__INP_DIS)
/* LCD Common or Segment Drive */
#define R2_LCD_COM_SEG            (* (reg8 *) R2__LCD_COM_SEG)
/* Enable Segment LCD */
#define R2_LCD_EN                 (* (reg8 *) R2__LCD_EN)
/* Slew Rate Control */
#define R2_SLW                    (* (reg8 *) R2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define R2_PRTDSI__CAPS_SEL       (* (reg8 *) R2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define R2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) R2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define R2_PRTDSI__OE_SEL0        (* (reg8 *) R2__PRTDSI__OE_SEL0) 
#define R2_PRTDSI__OE_SEL1        (* (reg8 *) R2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define R2_PRTDSI__OUT_SEL0       (* (reg8 *) R2__PRTDSI__OUT_SEL0) 
#define R2_PRTDSI__OUT_SEL1       (* (reg8 *) R2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define R2_PRTDSI__SYNC_OUT       (* (reg8 *) R2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(R2__SIO_CFG)
    #define R2_SIO_HYST_EN        (* (reg8 *) R2__SIO_HYST_EN)
    #define R2_SIO_REG_HIFREQ     (* (reg8 *) R2__SIO_REG_HIFREQ)
    #define R2_SIO_CFG            (* (reg8 *) R2__SIO_CFG)
    #define R2_SIO_DIFF           (* (reg8 *) R2__SIO_DIFF)
#endif /* (R2__SIO_CFG) */

/* Interrupt Registers */
#if defined(R2__INTSTAT)
    #define R2_INTSTAT            (* (reg8 *) R2__INTSTAT)
    #define R2_SNAP               (* (reg8 *) R2__SNAP)
    
	#define R2_0_INTTYPE_REG 		(* (reg8 *) R2__0__INTTYPE)
#endif /* (R2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_R2_H */


/* [] END OF FILE */
