#ifndef VIRTUALPROCESSOR_H
#define VIRTUALPROCESSOR_H

#include <cstdint>
#include "c_wrapper.h"
#include "io_data.h"
#include "communication_queue.h"
#include <queue>
#include <vector>

#define VP_SLEEP_TIMEOUT                10  //virtual processor sleep timeout
#define SUPERVISOR_OVERRUN_FACTOR       3   //mximum alowable supervisor itterations times number of sample points 

typedef void (*usrFncPtr)(input_data_pt, com_data_pt);
typedef void (*supervisorFncPtr)(com_data_pt, com_data_pt, output_data_pt);
typedef uint32_t run_cycles_t;

class VirtualProcessor
{
private:
    uint8_t id;
    usrFncPtr userCallback;
    supervisorFncPtr supervisorCallback;
    run_cycles_t runCycles;
    com_channel_e inComChannel[COM_SOCKETS_NO];
    std::vector<com_socket_e> inSockets; 
    com_channel_e outComChannel[COM_SOCKETS_NO];
    std::vector<com_socket_e> outSockets;
    
public:
    VirtualProcessor();
    ~VirtualProcessor();
    
    void installUserCalback(usrFncPtr);
    void installSupervisor(supervisorFncPtr);    
    void executeUserProgram(samples_vector_t&, samples_vector_t&);
    void executeSupervisor(output_vector_t&);
    void setRunCycles(run_cycles_t);
    void setInputCom(com_socket_e, com_channel_e);
    void setOutputCom(com_socket_e, com_channel_e);
};

#endif // VIRTUALPROCESSOR_H
