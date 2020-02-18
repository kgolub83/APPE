/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file virtual_processor.c
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

#include "virtual_processor.h"
#include "communication_queue.h"
#include <iostream>
#include <thread>
#include <cassert>

static uint8_t processor_count; 

VirtualProcessor::VirtualProcessor()
{
    std::cout << "Virtual processor crearted..." << +processor_count << std::endl;
    id = processor_count++;
    
    /*init function call pointers as NULL pointers*/
    initFnCallback = NULL;
    userCallback = NULL;
    supervisorCallback = NULL;
    exitFnCallback = NULL;
}

VirtualProcessor::~VirtualProcessor()
{
    std::cout << "Virtual processor " << +id << " shut down!" << std::endl;
}

void VirtualProcessor::initProcessor(uint8_t id)
{
    initFnCallback(id);
}

void VirtualProcessor::executeUserProgram(samples_vector_t &inputDataA, samples_vector_t &inputDataB)
{
    ComDataObj* const comChannels = ComDataObj::getInstance();
    input_data_t inputData;
    com_channel_t  comFrame;
    com_socket_e activeSocket;
    samples_no_t samplesNo;
    
    assert(initFnCallback);     //chek if initialistion function is installed
    assert(userCallback);       //checks if user callback function is set
    
    initProcessor(id);
    
    std::cout << "User application " << +id << " started..." << std::endl;
    
    //!@TODO check socket-channel consistency - should be only one communication channel installed
    activeSocket = outSockets.front();
    
    samplesNo = inputDataA.size();
    
    for(samples_no_t i=0; i < samplesNo; i++)
    {
        inputData.sensorSampleA = inputDataA[i]; 
        inputData.sensorSampleB = inputDataB[i]; 
        inputData.sampleNo = i;
        
        userCallback(&inputData, &comFrame);
        
        comChannels->addElement(outComChannel[activeSocket], comFrame);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(VP_SLEEP_TIMEOUT));
    }
    std::cout << "User function " << +id << " executed!" << std::endl;
}

void VirtualProcessor::executeSupervisor(output_vector_t &outputData)
{
    ComDataObj* const comChannels = ComDataObj::getInstance();   //Instanciate communication queues-FIFOs
    com_channel_t  comFrameA, comFrameB;
    output_data_t outSample;
    run_cycles_t iterations, iterationCount, maxIterations;
    com_channel_e inChannelA, inChannelB;
    bool syncFlagA, syncFlagB;
    
    assert(supervisorCallback);   //checks if user callback function is set
    
    if(NULL != initFnCallback)      //check if initialisation routine is installed
    {
        initProcessor(id);      //run user initialisation routine
    }
    
    //init buffer flags - indicates unprocessed data
    syncFlagA = false;
    syncFlagB = false;
    iterations = 0;
    iterationCount = 0;

    
    //!@TODO check socket-channel consistency - it should be only two input communication channels defined
    inChannelA = inComChannel[inSockets.front()];
    inChannelB = inComChannel[inSockets.back()];
    
    maxIterations = SUPERVISOR_OVERRUN_FACTOR*runCycles;
    
    outputData.clear();
    
    while((iterations < runCycles) && (iterationCount < maxIterations))
    {
        if(!syncFlagA) //check if data already pulled from communication fifo
        {
            if(COM_FIFO_SUCCESS == comChannels->getElement(inChannelA, comFrameA))
            {
                syncFlagA = true;
            }
        }
        
        if(!syncFlagB) //check if data already pulled from communication fifo
        {
            if(COM_FIFO_SUCCESS == comChannels->getElement(inChannelB, comFrameB))
            {
                syncFlagB = true;
            }
        }
        
        if(syncFlagA && syncFlagB) //if data from both channels retreived
        {
            supervisorCallback(&comFrameA, &comFrameB, &outSample); //run user program
            
            syncFlagA = false;
            syncFlagB = false;
            outputData.push_back(outSample);
            
            iterations++;
        }
        
        iterationCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(VP_SLEEP_TIMEOUT));
    }
    
    if(NULL != exitFnCallback)      //check if exit routine installed
    {
        exitFnCallback(id);     //run user exit routine
    }
    
}

void VirtualProcessor::installInitCalback(initFncPtr function)
{
    initFnCallback = function;
}

void VirtualProcessor::installUserCalback(usrFncPtr function)
{
    userCallback = function;
}

void VirtualProcessor::installExitCalback(initFncPtr function)
{
    exitFnCallback = function;
}

void VirtualProcessor::installSupervisor(supervisorFncPtr function)
{
    supervisorCallback = function;
}

void VirtualProcessor::setRunCycles(uint32_t cycles)
{
    runCycles = cycles;
}

void VirtualProcessor::setInputCom(com_socket_e socket, com_channel_e channel)
{
    inComChannel[socket] = channel;
    inSockets.push_back(socket);
}

void VirtualProcessor::setOutputCom(com_socket_e socket, com_channel_e channel)
{
    outComChannel[socket] = channel;
    outSockets.push_back(socket);
}