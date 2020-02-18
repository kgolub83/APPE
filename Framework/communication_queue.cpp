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

#include <iostream>
#include "communication_queue.h"

ComDataObj* ComDataObj::singleInstance_p = NULL;


ComDataObj::ComDataObj()
{
    
}
    
ComDataObj* ComDataObj::getInstance(void)   
{
    if (NULL == singleInstance_p) // if not yet instantiated
    {
          singleInstance_p = new ComDataObj;
          //create one and only object
    }

    return singleInstance_p;
}

void ComDataObj::addElement(com_channel_e channel, com_channel_t data)
{
    comFIFO[channel].push(data);        //write data to FIFO
}

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

sample_data_t ComDataObj::printSize(com_channel_e channel)
{
    sample_data_t fifoSize;
    fifoSize = comFIFO[channel].size();
    
    std::cout << "FIFO " << channel << " size is " << fifoSize << std::endl;
    
    return fifoSize;
}
