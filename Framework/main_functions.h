/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main_functions.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief functions called from main function
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

#define APPE_VERSION        0
#define APPE_SUB_VERSION    1

#define INPUT_DATA_FILE     "Data/appe_test_data.csv"
#define OUTPUT_DATA_FILE    "Data/appe_output_data.csv"

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

enum appeModelRetVal
{
    APPE_CLEAN_EXIT,
    APPE_FAULT_RETURN
};

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

extern const char* mainTerminalStrings[];

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void printLogo(uint8_t majorVersion, uint8_t subVersion);

#endif // MAIN_FUNCTIONS_H

/******************************************************************************
**                               End Of File
*******************************************************************************/



