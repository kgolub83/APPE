/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file communication_queue.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief singleton class emulating communication channels between processors and
 *        supervisor
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <iostream>
#include "communication_queue.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

ComDataObj* ComDataObj::singleInstance_p = NULL;

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
* @returns 
*******************************************************************************/

ComDataObj::ComDataObj()
{
    
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/

ComDataObj* ComDataObj::getInstance(void)   
{
    if (NULL == singleInstance_p)   // if not yet instantiated
    {
        singleInstance_p = new ComDataObj;  //create one and only object      
    }

    return singleInstance_p;
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/

void ComDataObj::addElement(com_channel_e channel, com_channel_t data)
{
    comFIFO[channel].push(data);        //write data to FIFO
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/

com_fifo_e ComDataObj::getElement(com_channel_e channel, com_channel_t& data)
{
    com_fifo_e ret_val;
    
    if(comFIFO[channel].empty())
    {
        ret_val = COM_FIFO_EMPTY;
    } else
    {
        data = comFIFO[channel].front();    //read oldest element
        comFIFO[channel].pop();             //remove oldest element
        ret_val = COM_FIFO_SUCCESS;
    }
    return ret_val;
}    

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/

sample_data_t ComDataObj::printSize(com_channel_e channel)
{
    sample_data_t fifoSize;
    fifoSize = comFIFO[channel].size();
    
    std::cout << "FIFO " << channel << " size is " << fifoSize << std::endl;
    
    return fifoSize;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
