#ifndef COMMUNICATION_QUEUE_H
#define COMMUNICATION_QUEUE_H

#include <queue>
#include "c_wrapper.h"

typedef enum
{
    COM_FIFO_SUCCESS,
    COM_FIFO_EMPTY
} com_fifo_e;

typedef enum
{
    //names of the communication channels
    PROC_A_OUT_COM_CHANNEL = 0,
    PROC_B_OUT_COM_CHANNEL,
    //defines number of channels used 
    COM_CHANNELS_NO        
} com_channel_e;

typedef enum
{
    //names of the communication socket
    COM_SOCKET_A = 0,
    COM_SOCKET_B,
    COM_SOCKET_C,
    COM_SOCKET_D,
    //defines number of sockets used
    COM_SOCKETS_NO
} com_socket_e;

typedef std::queue<com_data_t> com_queue_t; 

class ComDataObj
{
private:

    ComDataObj();   // private constructor
    static ComDataObj* singleInstance_p;    //holds one and only object
    
    com_queue_t comFIFO[COM_CHANNELS_NO];  

public:
    static ComDataObj* getInstance();
    //static method that returns only instance of ComDataObje

    // ComDataObj functionalities
    void addElement(com_channel_e, com_data_t&);
    com_fifo_e getElement(com_channel_e, com_data_t&);
    sample_data_t printSize(com_channel_e channel);

};

#endif // COMMUNICATION_QUEUE_H
