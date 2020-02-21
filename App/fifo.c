/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file fifo.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief signal procesing digital filters implementations
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                             Includes
*******************************************************************************/  

#include "fifo.h"
#include <string.h>

#ifdef __LINUX
#include <pthread.h>
#include <errno.h>
#endif  /*__LINUX*/

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

#ifdef __LINUX
/* static MUTEX lock object */ 
static pthread_mutex_t mutexLock;
#endif /*__LINUX*/

/*******************************************************************************
**                             Code
*******************************************************************************/

/*!****************************************************************************
* @function fifoInit
* 
* @brief initializes fifo structure with default values
*        init function must be called after instantiation of the fifo structure
*        for proper initial conditions and proper operation
*
* @param *this pointer to fifo structure
*
* @return void function
******************************************************************************/
void fifoInit(struct fifo_ctrl *this)
{
    this->fifoLock = false;
    this->empty = true;
    this->full = false;
    this->head = 0;
    this->tail = 0;
}

/*!****************************************************************************
* @function fifo_write_ctl
* 
* @brief Control function for writing to fifo
*        Function is called when writting to fifo
*        It handles, tail and head pointer and fifo full flag
*
* @param *this pointer to fifo structure
*
* @return void function
*******************************************************************************/
static inline void fifo_write_ctl(struct fifo_ctrl *this)
{
    /* check for the last element in row */
    if (this->head >= this->elements) 
    {
        /* reset head pointer */
        this->head = 0;    
    }

    /* check for fifo full */
    if (this->head == this->tail) 
    {
        /* rise fifo full flag */
        this->full = true;        
    }
    
    /* reset fifo empty flag */
    this->empty = false;        
}

/*!*****************************************************************************
* @function fifo_read_ctl
* 
* @brief Control function for reading from fifo
*        Function is called when reading from fifo 
*        It handles, tail and head pointer and fifo full flag
*
* @param *this pointer to fifo structure
*
* @return void function
*******************************************************************************/
static inline void fifo_read_ctl(struct fifo_ctrl *this)
{
    if(this->tail >= this->elements)
    {
        /* reset fifo tail pointer */
        this->tail = 0;        
    }

    if (this->head == this->tail)
    {
        /* rise fifo empty flag */
        this->empty = true;    
    }
 
    /* reset fifo full flag */
    this->full = false;        
}

/*!*****************************************************************************
* @function fifoWrite
* 
* @brief  Writes one data structure to fifo and return whether 
*         is executed properly
*
* @param *this pointer to fifo structure
* @param data element to be written to fifo
*
* @return whether writing is executed properly
*******************************************************************************/
fifo_ret_val_t fifoWrite(circ_fifo_t * const this, const void *data)
{
    if(this->ctrl.head == this->ctrl.tail && (this->ctrl.empty == false))
    {
        return FIFO_FULL;  
    }
    
    /* load data to fifo */
    FifoCriticalSectionLock_m(&mutexLock);        /*Enter critical section*/
    memcpy(this->buffer + this->ctrl.dataElementSize*(this->ctrl.head++), data, this->ctrl.dataElementSize); 
    FifoCriticalSectionUnlock_m(&mutexLock);        /*Leave critical section*/

    /* menage fifo controll variables */
    fifo_write_ctl(&this->ctrl); 
    
    return FIFO_CLEAN;
}

/*!*****************************************************************************
* @function fifoRead
* 
* @brief Reads one data structure from fifo and returns whether is executed properly
*
* @param *this pointer to fifo structure
* @param *data pointer to element to be read from fifo
*
* @return whether read is executed properly
*******************************************************************************/
fifo_ret_val_t fifoRead(circ_fifo_t * const this, void *data)
{
    /* check if fifo is empty */
    if ((this->ctrl.head == this->ctrl.tail) && (this->ctrl.full == false))        
    {
        return FIFO_EMPTY; 
    }

    /* pull data from fifo */
    FifoCriticalSectionLock_m(&mutexLock);        /*Enter critical section*/
    memcpy(data, this->buffer + this->ctrl.dataElementSize*(this->ctrl.tail++), this->ctrl.dataElementSize);
    FifoCriticalSectionUnlock_m(&mutexLock);        /*Leave critical section*/
    /* manage fifo controll variables */
    fifo_read_ctl(&this->ctrl); 

    return FIFO_CLEAN;
}

/*!********************************************************
* @function Fifo_delElement
* @brief Deletes fifo element
*
* Deletes last (most recently added) element from fifo
*
* @param *this pointer to fifo structure
*
* @return whether deletion is executed properly
**********************************************************/
int fifoDelLastElement(struct fifo_ctrl *this)
{
    /* check if fifo is empty */
    if ((this->head == this->tail) && (this->full == false))        
    {
        return FIFO_EMPTY; 
    }

    /* delete data from fifo */
    this->head--;    

    /* menage fifo controll variables */    
    if(this->head >= this->elements)
    {
        /* set fifo head pointer to last qeue element */
        this->head = this->elements -1;        
    }

    if (this->head == this->tail)
    {
        /* rise fifo empty flag */
        this->empty = true;    
    }

    /* reset fifo full flag */
    this->full = false;        
    
    return FIFO_CLEAN;        
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
