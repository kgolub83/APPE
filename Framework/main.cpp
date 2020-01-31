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

int main(int argc, char **argv)
{
    VirtualProcessor codedProcesorA;
    VirtualProcessor codedProcesorB;  
    VirtualProcessor supervisor;
    IOdata testData;
    ComDataObj* comChannels = ComDataObj::getInstance();   //Instanciate communication queues-FIFOs
    samples_vector_t inputDataA, inputDataB;
    tst_data_attributes_t inputDataAttributes;
    output_vector_t outputData;

    std::cout << mainTerminalStrings[initCom] << std::hex << comChannels << std::endl;
    std::cout << std::dec << std::endl;

    assert(!testData.readCSV(INPUT_DATA_FILE));    //read input data and check consistency
    
    std::cout << mainTerminalStrings[initData] << std::endl;
    
    std::cout << mainTerminalStrings[initFramework] << std::endl;   
    std::cout << std::thread::hardware_concurrency() 
              << mainTerminalStrings[infoConcurrency] << std::endl;
    
    //configure processing unit A  
    codedProcesorA.installInitCalback(procInitCodeA);
    codedProcesorA.installUserCalback(processorCodeA);
    codedProcesorA.setRunCycles(testData.getSamplesNo());
    codedProcesorA.setOutputCom(COM_SOCKET_A,PROC_A_OUT_COM_CHANNEL);
    
    //configure processing unit B
    codedProcesorB.installInitCalback(procInitCodeB);
    codedProcesorB.installUserCalback(processorCodeB);
    codedProcesorB.setRunCycles(testData.getSamplesNo());
    codedProcesorB.setOutputCom(COM_SOCKET_A, PROC_B_OUT_COM_CHANNEL);
    
    //configure supervisor processing unit
    supervisor.installInitCalback(supervisorInitCode);
    supervisor.installSupervisor(supervisorCode);
    supervisor.installExitCalback(supervisorExitRoutine);
    supervisor.setRunCycles(testData.getSamplesNo());
    supervisor.setInputCom(COM_SOCKET_C, PROC_A_OUT_COM_CHANNEL);
    supervisor.setInputCom(COM_SOCKET_D, PROC_B_OUT_COM_CHANNEL);
    
    //set test data and parameters
    inputDataA = testData.getSensorData(DATA_VECTOR_A);
    inputDataB = testData.getSensorData(DATA_VECTOR_B);
    testData.getDataAttributes(inputDataAttributes);
    setTestDataAttributes(&inputDataAttributes);

    //start virtual processing units as threads
    std::thread threadSupervisor(&VirtualProcessor::executeSupervisor, &supervisor,
                                std::ref(outputData));    
    
    std::thread threadProcA(&VirtualProcessor::executeUserProgram, &codedProcesorA,
                            std::ref(inputDataA), std::ref(inputDataB));

    std::thread threadProcB(&VirtualProcessor::executeUserProgram, &codedProcesorB, 
                            std::ref(inputDataA), std::ref(inputDataB));
    
    threadProcA.join();
    threadProcB.join();
    threadSupervisor.join();
    
    testData.writeCSV("acs_output_data.csv", outputData);
    
    return 0;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

 