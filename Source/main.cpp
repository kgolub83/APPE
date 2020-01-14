/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
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

#include "main_functions.h"

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
* @returns 
*******************************************************************************/

int main(int argc, char **argv)
{
    VirtualProcessor codedProcesorA;
    VirtualProcessor codedProcesorB;  
    VirtualProcessor supervisor;
    IOdata testData;
    ComDataObj* comChannels = ComDataObj::getInstance();   //Instanciate communication queues-FIFOs
    samples_vector_t inputDataA, inputDataB;
    output_vector_t outputData;

    std::cout << mainTerminalStrings[initCom] << std::hex << comChannels << std::endl;
    std::cout << std::dec << std::endl;

    //read input data
    assert(!testData.readCSV(INPUT_DATA_FILE));    //read input data and check consistency
    
    std::cout << mainTerminalStrings[initData] << std::endl;
    
    std::cout << mainTerminalStrings[initFramework] << std::endl;   
    std::cout << std::thread::hardware_concurrency() 
              << mainTerminalStrings[infoConcurrency] << std::endl;
              
    codedProcesorA.installUserCalback(processorCode);
    codedProcesorA.setRunCycles(testData.getSamplesNo());
    codedProcesorA.setOutputCom(COM_SOCKET_A,PROC_A_OUT_COM_CHANNEL);
    
    codedProcesorB.installUserCalback(processorCode);
    codedProcesorB.setRunCycles(testData.getSamplesNo());
    codedProcesorB.setOutputCom(COM_SOCKET_A, PROC_B_OUT_COM_CHANNEL);
    
    supervisor.installSupervisor(supervisorCode);
    supervisor.setRunCycles(testData.getSamplesNo());
    supervisor.setInputCom(COM_SOCKET_C, PROC_A_OUT_COM_CHANNEL);
    supervisor.setInputCom(COM_SOCKET_D, PROC_B_OUT_COM_CHANNEL);
    
    inputDataA = testData.getSensorData(DATA_VECTOR_A);
    inputDataB = testData.getSensorData(DATA_VECTOR_B);

    std::thread threadProcA(&VirtualProcessor::executeUserProgram, &codedProcesorA,
                            std::ref(inputDataA), std::ref(inputDataB));

    std::thread threadProcB(&VirtualProcessor::executeUserProgram, &codedProcesorB, 
                            std::ref(inputDataA), std::ref(inputDataB));

    std::thread threadSupervisor(&VirtualProcessor::executeSupervisor, &supervisor,
                                std::ref(outputData));
    
    threadProcA.join();
    threadProcB.join();
    threadSupervisor.join();
    
    testData.writeCSV("acs_output_data.csv", outputData);

    return 0;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

 