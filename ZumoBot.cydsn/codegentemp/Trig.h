/*******************************************************************************
* File Name: Trig.h  
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

#if !defined(CY_PINS_Trig_H) /* Pins Trig_H */
#define CY_PINS_Trig_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Trig_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Trig__PORT == 15 && ((Trig__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Trig_Write(uint8 value);
void    Trig_SetDriveMode(uint8 mode);
uint8   Trig_ReadDataReg(void);
uint8   Trig_Read(void);
void    Trig_SetInterruptMode(uint16 position, uint16 mode);
uint8   Trig_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Trig_SetDriveMode() function.
     *  @{
     */
        #define Trig_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Trig_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Trig_DM_RES_UP          PIN_DM_RES_UP
        #define Trig_DM_RES_DWN         PIN_DM_RES_DWN
        #define Trig_DM_OD_LO           PIN_DM_OD_LO
        #define Trig_DM_OD_HI           PIN_DM_OD_HI
        #define Trig_DM_STRONG          PIN_DM_STRONG
        #define Trig_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Trig_MASK               Trig__MASK
#define Trig_SHIFT              Trig__SHIFT
#define Trig_WIDTH              1u

/* Interrupt constants */
#if defined(Trig__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Trig_SetInterruptMode() function.
     *  @{
     */
        #define Trig_INTR_NONE      (uint16)(0x0000u)
        #define Trig_INTR_RISING    (uint16)(0x0001u)
        #define Trig_INTR_FALLING   (uint16)(0x0002u)
        #define Trig_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Trig_INTR_MASK      (0x01u) 
#endif /* (Trig__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Trig_PS                     (* (reg8 *) Trig__PS)
/* Data Register */
#define Trig_DR                     (* (reg8 *) Trig__DR)
/* Port Number */
#define Trig_PRT_NUM                (* (reg8 *) Trig__PRT) 
/* Connect to Analog Globals */                                                  
#define Trig_AG                     (* (reg8 *) Trig__AG)                       
/* Analog MUX bux enable */
#define Trig_AMUX                   (* (reg8 *) Trig__AMUX) 
/* Bidirectional Enable */                                                        
#define Trig_BIE                    (* (reg8 *) Trig__BIE)
/* Bit-mask for Aliased Register Access */
#define Trig_BIT_MASK               (* (reg8 *) Trig__BIT_MASK)
/* Bypass Enable */
#define Trig_BYP                    (* (reg8 *) Trig__BYP)
/* Port wide control signals */                                                   
#define Trig_CTL                    (* (reg8 *) Trig__CTL)
/* Drive Modes */
#define Trig_DM0                    (* (reg8 *) Trig__DM0) 
#define Trig_DM1                    (* (reg8 *) Trig__DM1)
#define Trig_DM2                    (* (reg8 *) Trig__DM2) 
/* Input Buffer Disable Override */
#define Trig_INP_DIS                (* (reg8 *) Trig__INP_DIS)
/* LCD Common or Segment Drive */
#define Trig_LCD_COM_SEG            (* (reg8 *) Trig__LCD_COM_SEG)
/* Enable Segment LCD */
#define Trig_LCD_EN                 (* (reg8 *) Trig__LCD_EN)
/* Slew Rate Control */
#define Trig_SLW                    (* (reg8 *) Trig__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Trig_PRTDSI__CAPS_SEL       (* (reg8 *) Trig__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Trig_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Trig__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Trig_PRTDSI__OE_SEL0        (* (reg8 *) Trig__PRTDSI__OE_SEL0) 
#define Trig_PRTDSI__OE_SEL1        (* (reg8 *) Trig__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Trig_PRTDSI__OUT_SEL0       (* (reg8 *) Trig__PRTDSI__OUT_SEL0) 
#define Trig_PRTDSI__OUT_SEL1       (* (reg8 *) Trig__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Trig_PRTDSI__SYNC_OUT       (* (reg8 *) Trig__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Trig__SIO_CFG)
    #define Trig_SIO_HYST_EN        (* (reg8 *) Trig__SIO_HYST_EN)
    #define Trig_SIO_REG_HIFREQ     (* (reg8 *) Trig__SIO_REG_HIFREQ)
    #define Trig_SIO_CFG            (* (reg8 *) Trig__SIO_CFG)
    #define Trig_SIO_DIFF           (* (reg8 *) Trig__SIO_DIFF)
#endif /* (Trig__SIO_CFG) */

/* Interrupt Registers */
#if defined(Trig__INTSTAT)
    #define Trig_INTSTAT            (* (reg8 *) Trig__INTSTAT)
    #define Trig_SNAP               (* (reg8 *) Trig__SNAP)
    
	#define Trig_0_INTTYPE_REG 		(* (reg8 *) Trig__0__INTTYPE)
#endif /* (Trig__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Trig_H */


/* [] END OF FILE */
