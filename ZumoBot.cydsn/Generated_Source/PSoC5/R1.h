/*******************************************************************************
* File Name: R1.h  
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

#if !defined(CY_PINS_R1_H) /* Pins R1_H */
#define CY_PINS_R1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "R1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 R1__PORT == 15 && ((R1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    R1_Write(uint8 value);
void    R1_SetDriveMode(uint8 mode);
uint8   R1_ReadDataReg(void);
uint8   R1_Read(void);
void    R1_SetInterruptMode(uint16 position, uint16 mode);
uint8   R1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the R1_SetDriveMode() function.
     *  @{
     */
        #define R1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define R1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define R1_DM_RES_UP          PIN_DM_RES_UP
        #define R1_DM_RES_DWN         PIN_DM_RES_DWN
        #define R1_DM_OD_LO           PIN_DM_OD_LO
        #define R1_DM_OD_HI           PIN_DM_OD_HI
        #define R1_DM_STRONG          PIN_DM_STRONG
        #define R1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define R1_MASK               R1__MASK
#define R1_SHIFT              R1__SHIFT
#define R1_WIDTH              1u

/* Interrupt constants */
#if defined(R1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in R1_SetInterruptMode() function.
     *  @{
     */
        #define R1_INTR_NONE      (uint16)(0x0000u)
        #define R1_INTR_RISING    (uint16)(0x0001u)
        #define R1_INTR_FALLING   (uint16)(0x0002u)
        #define R1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define R1_INTR_MASK      (0x01u) 
#endif /* (R1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define R1_PS                     (* (reg8 *) R1__PS)
/* Data Register */
#define R1_DR                     (* (reg8 *) R1__DR)
/* Port Number */
#define R1_PRT_NUM                (* (reg8 *) R1__PRT) 
/* Connect to Analog Globals */                                                  
#define R1_AG                     (* (reg8 *) R1__AG)                       
/* Analog MUX bux enable */
#define R1_AMUX                   (* (reg8 *) R1__AMUX) 
/* Bidirectional Enable */                                                        
#define R1_BIE                    (* (reg8 *) R1__BIE)
/* Bit-mask for Aliased Register Access */
#define R1_BIT_MASK               (* (reg8 *) R1__BIT_MASK)
/* Bypass Enable */
#define R1_BYP                    (* (reg8 *) R1__BYP)
/* Port wide control signals */                                                   
#define R1_CTL                    (* (reg8 *) R1__CTL)
/* Drive Modes */
#define R1_DM0                    (* (reg8 *) R1__DM0) 
#define R1_DM1                    (* (reg8 *) R1__DM1)
#define R1_DM2                    (* (reg8 *) R1__DM2) 
/* Input Buffer Disable Override */
#define R1_INP_DIS                (* (reg8 *) R1__INP_DIS)
/* LCD Common or Segment Drive */
#define R1_LCD_COM_SEG            (* (reg8 *) R1__LCD_COM_SEG)
/* Enable Segment LCD */
#define R1_LCD_EN                 (* (reg8 *) R1__LCD_EN)
/* Slew Rate Control */
#define R1_SLW                    (* (reg8 *) R1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define R1_PRTDSI__CAPS_SEL       (* (reg8 *) R1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define R1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) R1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define R1_PRTDSI__OE_SEL0        (* (reg8 *) R1__PRTDSI__OE_SEL0) 
#define R1_PRTDSI__OE_SEL1        (* (reg8 *) R1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define R1_PRTDSI__OUT_SEL0       (* (reg8 *) R1__PRTDSI__OUT_SEL0) 
#define R1_PRTDSI__OUT_SEL1       (* (reg8 *) R1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define R1_PRTDSI__SYNC_OUT       (* (reg8 *) R1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(R1__SIO_CFG)
    #define R1_SIO_HYST_EN        (* (reg8 *) R1__SIO_HYST_EN)
    #define R1_SIO_REG_HIFREQ     (* (reg8 *) R1__SIO_REG_HIFREQ)
    #define R1_SIO_CFG            (* (reg8 *) R1__SIO_CFG)
    #define R1_SIO_DIFF           (* (reg8 *) R1__SIO_DIFF)
#endif /* (R1__SIO_CFG) */

/* Interrupt Registers */
#if defined(R1__INTSTAT)
    #define R1_INTSTAT            (* (reg8 *) R1__INTSTAT)
    #define R1_SNAP               (* (reg8 *) R1__SNAP)
    
	#define R1_0_INTTYPE_REG 		(* (reg8 *) R1__0__INTTYPE)
#endif /* (R1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_R1_H */


/* [] END OF FILE */
