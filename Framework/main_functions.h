/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief 
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H


/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include "virtual_processor.h"
#include "c_wrapper.h"
#include "io_data.h"
#include "communication_queue.h"

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define INPUT_DATA_FILE     "Data/acs_test_data.csv"
#define OUTPUT_DATA_FILE    "Data/acs_output_data.csv"

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

enum mainMessages
{
    initCom,
    initData,
    initFramework,
    infoConcurrency
};

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/
extern const char* mainTerminalStrings[];
/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

#endif // MAIN_FUNCTIONS_H

/******************************************************************************
**                               End Of File
*******************************************************************************/



