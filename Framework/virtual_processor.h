/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file virtual_processor.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief modeled processor container class
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

#ifndef VIRTUALPROCESSOR_H
#define VIRTUALPROCESSOR_H

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <cstdint>
#include "c_wrapper.h"
#include "io_data.h"
#include "communication_queue.h"
#include <queue>
#include <vector>

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define VP_SLEEP_TIMEOUT                1   //virtual processor sleep timeout
#define SUPERVISOR_OVERRUN_FACTOR       3   //maximum alowable supervisor iterations times number of sample points 

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef void(*initFncPtr)(uint8_t);
typedef void (*usrFncPtr)(input_data_pt, com_channel_pt);
typedef void (*supervisorFncPtr)(com_channel_pt, com_channel_pt, output_data_pt);
typedef uint32_t run_cycles_t;

class VirtualProcessor
{
private:
    uint8_t id;
    initFncPtr initFnCallback;
    usrFncPtr userCallback;
    supervisorFncPtr supervisorCallback;
    initFncPtr exitFnCallback;
    run_cycles_t runCycles;
    com_channel_e inComChannel[COM_SOCKETS_NO];
    std::vector<com_socket_e> inSockets; 
    com_channel_e outComChannel[COM_SOCKETS_NO];
    std::vector<com_socket_e> outSockets;
    
public:
    VirtualProcessor();
    ~VirtualProcessor();
    
    void installInitCalback(initFncPtr);
    void installUserCalback(usrFncPtr);
    void installSupervisor(supervisorFncPtr);
    void installExitCalback(initFncPtr function);
    void initProcessor(uint8_t);
    void executeUserProgram(samples_vector_t&, samples_vector_t&);
    void executeSupervisor(output_vector_t&);
    void setRunCycles(run_cycles_t);
    void setInputCom(com_socket_e, com_channel_e);
    void setOutputCom(com_socket_e, com_channel_e);
};

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/


#endif // VIRTUALPROCESSOR_H

/******************************************************************************
**                               End Of File
*******************************************************************************/
