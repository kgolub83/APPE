/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief main function call 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "main_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

const char* mainTerminalStrings[] =
{
    [initCom] = "Virtual Communication channels initialized at ",
    [initData] = "Data loading success...",
    [initFramework] = "Dual processing framework started...", 
    [infoConcurrency] = " physical concurrent processing units available"
};

static const char appeLogo[] =
{
    "\033[H\33[J"       /*clear screen*/
	"\033[91m"          /*colore it*/
	"\r\n" 
	"     ___    ____  ____  ______            __  ___          __     __         \r\n"
	"    /   |  / __ \\/ __ \\/ ____/           /  |/  /___  ____/ /__  / /       GLOBAL LOGIC (R)          \r\n"
    "   / /| | / /_/ / /_/ / __/    ______   / /|_/ / __ \\/ __  / _ \\/ /      All rights reserved     \r\n"
	"  / ___ |/ ____/ ____/ /___   /_____/  / /  / / /_/ / /_/ /  __/ /         Licensed by GL        \r\n"
	" /_/  |_/_/   /_/   /_____/           /_/  /_/\\____/\\__,_/\\___/_/       Programing by KG v%d.%d   \r\n"
	"\033[0m"	        /*reset formating*/
    "\r\n"
};

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

void printLogo(uint8_t majorVersion, uint8_t subVersion)
{
    printf(appeLogo, majorVersion,subVersion);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/