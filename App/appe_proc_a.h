/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_proc.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   - redundant - asymmetric processor A implementation 
 *          - one of two acceleration pedal position encoder processing models 
 *          - procesing, filtering and domain checking of two orthogonal position 
 *          sensor signals  
 * 
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef APPE_PROC_A_H
#define APPE_PROC_A_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern */

/*******************************************************************************
**                                Includes
*******************************************************************************/

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define IIR_AVG_COEFFICIENT     0.0595819f /* filter LP frequency coefficient*/

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

#ifdef __cplusplus
} /* extern */
#endif /* extern */

#endif /* APPE_PROC_A_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/