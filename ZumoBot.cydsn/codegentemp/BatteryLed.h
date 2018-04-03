/*******************************************************************************
* File Name: BatteryLed.h  
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

#if !defined(CY_PINS_BatteryLed_H) /* Pins BatteryLed_H */
#define CY_PINS_BatteryLed_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BatteryLed_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BatteryLed__PORT == 15 && ((BatteryLed__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BatteryLed_Write(uint8 value);
void    BatteryLed_SetDriveMode(uint8 mode);
uint8   BatteryLed_ReadDataReg(void);
uint8   BatteryLed_Read(void);
void    BatteryLed_SetInterruptMode(uint16 position, uint16 mode);
uint8   BatteryLed_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BatteryLed_SetDriveMode() function.
     *  @{
     */
        #define BatteryLed_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BatteryLed_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BatteryLed_DM_RES_UP          PIN_DM_RES_UP
        #define BatteryLed_DM_RES_DWN         PIN_DM_RES_DWN
        #define BatteryLed_DM_OD_LO           PIN_DM_OD_LO
        #define BatteryLed_DM_OD_HI           PIN_DM_OD_HI
        #define BatteryLed_DM_STRONG          PIN_DM_STRONG
        #define BatteryLed_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BatteryLed_MASK               BatteryLed__MASK
#define BatteryLed_SHIFT              BatteryLed__SHIFT
#define BatteryLed_WIDTH              1u

/* Interrupt constants */
#if defined(BatteryLed__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BatteryLed_SetInterruptMode() function.
     *  @{
     */
        #define BatteryLed_INTR_NONE      (uint16)(0x0000u)
        #define BatteryLed_INTR_RISING    (uint16)(0x0001u)
        #define BatteryLed_INTR_FALLING   (uint16)(0x0002u)
        #define BatteryLed_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BatteryLed_INTR_MASK      (0x01u) 
#endif /* (BatteryLed__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BatteryLed_PS                     (* (reg8 *) BatteryLed__PS)
/* Data Register */
#define BatteryLed_DR                     (* (reg8 *) BatteryLed__DR)
/* Port Number */
#define BatteryLed_PRT_NUM                (* (reg8 *) BatteryLed__PRT) 
/* Connect to Analog Globals */                                                  
#define BatteryLed_AG                     (* (reg8 *) BatteryLed__AG)                       
/* Analog MUX bux enable */
#define BatteryLed_AMUX                   (* (reg8 *) BatteryLed__AMUX) 
/* Bidirectional Enable */                                                        
#define BatteryLed_BIE                    (* (reg8 *) BatteryLed__BIE)
/* Bit-mask for Aliased Register Access */
#define BatteryLed_BIT_MASK               (* (reg8 *) BatteryLed__BIT_MASK)
/* Bypass Enable */
#define BatteryLed_BYP                    (* (reg8 *) BatteryLed__BYP)
/* Port wide control signals */                                                   
#define BatteryLed_CTL                    (* (reg8 *) BatteryLed__CTL)
/* Drive Modes */
#define BatteryLed_DM0                    (* (reg8 *) BatteryLed__DM0) 
#define BatteryLed_DM1                    (* (reg8 *) BatteryLed__DM1)
#define BatteryLed_DM2                    (* (reg8 *) BatteryLed__DM2) 
/* Input Buffer Disable Override */
#define BatteryLed_INP_DIS                (* (reg8 *) BatteryLed__INP_DIS)
/* LCD Common or Segment Drive */
#define BatteryLed_LCD_COM_SEG            (* (reg8 *) BatteryLed__LCD_COM_SEG)
/* Enable Segment LCD */
#define BatteryLed_LCD_EN                 (* (reg8 *) BatteryLed__LCD_EN)
/* Slew Rate Control */
#define BatteryLed_SLW                    (* (reg8 *) BatteryLed__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BatteryLed_PRTDSI__CAPS_SEL       (* (reg8 *) BatteryLed__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BatteryLed_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BatteryLed__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BatteryLed_PRTDSI__OE_SEL0        (* (reg8 *) BatteryLed__PRTDSI__OE_SEL0) 
#define BatteryLed_PRTDSI__OE_SEL1        (* (reg8 *) BatteryLed__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BatteryLed_PRTDSI__OUT_SEL0       (* (reg8 *) BatteryLed__PRTDSI__OUT_SEL0) 
#define BatteryLed_PRTDSI__OUT_SEL1       (* (reg8 *) BatteryLed__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BatteryLed_PRTDSI__SYNC_OUT       (* (reg8 *) BatteryLed__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BatteryLed__SIO_CFG)
    #define BatteryLed_SIO_HYST_EN        (* (reg8 *) BatteryLed__SIO_HYST_EN)
    #define BatteryLed_SIO_REG_HIFREQ     (* (reg8 *) BatteryLed__SIO_REG_HIFREQ)
    #define BatteryLed_SIO_CFG            (* (reg8 *) BatteryLed__SIO_CFG)
    #define BatteryLed_SIO_DIFF           (* (reg8 *) BatteryLed__SIO_DIFF)
#endif /* (BatteryLed__SIO_CFG) */

/* Interrupt Registers */
#if defined(BatteryLed__INTSTAT)
    #define BatteryLed_INTSTAT            (* (reg8 *) BatteryLed__INTSTAT)
    #define BatteryLed_SNAP               (* (reg8 *) BatteryLed__SNAP)
    
	#define BatteryLed_0_INTTYPE_REG 		(* (reg8 *) BatteryLed__0__INTTYPE)
#endif /* (BatteryLed__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BatteryLed_H */


/* [] END OF FILE */
