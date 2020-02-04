/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file integrator.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief 
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/  

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef int integrator_data_t;

typedef struct
{
    integrator_data_t accumulator;
    integrator_data_t upCount;
    integrator_data_t downCount;
    integrator_data_t lowClipVal;
    integrator_data_t hiClipVal;
    integrator_data_t upThreshold;
    integrator_data_t downThreshold;
    bool status;
} integrator_t;

typedef integrator_t * const integrator_pt;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void integratorReset(integrator_pt integrator);
extern bool integratorUp(integrator_pt integrator);
extern bool integratorDown(integrator_pt integrator);
extern bool integratorStatus(integrator_pt integrator);
extern bool integratorTest(integrator_data_t topValue, integrator_data_t hysteresis);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* INTEGRATOR_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/
