/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file integrator.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief main function call 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "integrator.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @return 
*******************************************************************************/

void integratorReset(integrator_pt integrator)
{
    integrator->accumulator = 0;
    integrator->status = false;
}

inline bool integratorUp(integrator_pt integrator)
{
    integrator->accumulator += integrator->upCount;
   
    if(integrator->accumulator >= integrator->upThreshold)
    {
        integrator->status = true;
        if(integrator->accumulator >= integrator->hiClipVal)
        {
            integrator->accumulator = integrator->hiClipVal;
        }
    }
   
    return integrator->status;
}

inline bool integratorDown(integrator_pt integrator)
{
    integrator->accumulator -= integrator->downCount;
   
    if(integrator->accumulator <= integrator->downThreshold)
    {
        integrator->status = false;
        if(integrator->accumulator <= integrator->lowClipVal)
        {
            integrator->accumulator = integrator->lowClipVal;
        }
    }
   
    return integrator->status;
}

inline bool integratorStatus(integrator_pt integrator)
{    
    if(integrator->accumulator >= integrator->upThreshold)
    {
        integrator->status = true;
        if(integrator->accumulator >= integrator->hiClipVal)
        {
            integrator->accumulator = integrator->hiClipVal;
        }
    } else if(integrator->accumulator <= integrator->downThreshold)
    {
        integrator->status = false;
        if(integrator->accumulator <= integrator->lowClipVal)
        {
            integrator->accumulator = integrator->lowClipVal;
        }
    }

    return integrator->status;
}

bool integratorTest(integrator_data_t topValue, integrator_data_t hysteresis)
{
    integrator_data_t i;
    bool testVar;
    integrator_t testIntegrator =
    {
        .accumulator = 0,
        .upCount = 1,
        .downCount = 1,
        .upThreshold = topValue,
        .hiClipVal = topValue + hysteresis,
        .downThreshold = topValue - hysteresis,
        .lowClipVal = topValue - 2*hysteresis
    };
    
    integratorReset(&testIntegrator);
    
    for(i = 0; i<topValue-1; i++)
    {
        testVar = integratorUp(&testIntegrator);
    }
   
    if(testVar == false) /*integrator must not overflow*/
    {
        testVar = integratorUp(&testIntegrator);
    } else
    {
        printf("#ERR: Integrator overflow before threshold!\n");
        return false;
    }
    
    if(testVar == true) /*check if integrator crossed threshold*/
    {
        for(i=0; i<hysteresis+1; i++)
        {
            testVar = integratorUp(&testIntegrator);
        }
    } else
    {
        printf("#ERR: Integrator overflow not trigered!\n");
        return false;
    }
    
    if (testIntegrator.accumulator == topValue+hysteresis) /*check if integrator clipped*/
    {
        for(i=0; i<((2*hysteresis)-1); i++)
        {
            testVar = integratorDown(&testIntegrator);
        }
    } else
    {
        printf("#ERR: Clip to upper value failed!\n");
        return false;
    }
    
    if(testVar == true) /*check if integrator still in high state*/
    {
        testVar = integratorDown(&testIntegrator);
    } else
    {
        printf("#ERR: Integrator reset befor lower threshold!\n");
        return false;
    }
    
    if(testVar == false)    /*check if integrator reseted properly*/
    {
        for(i=0; i<hysteresis+1; i++)
        {
            testVar = integratorDown(&testIntegrator);
        }
    } else
    {
        printf("#ERR: Integrator failed to reset on low threshold value\n");
    }
    
    return true;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/