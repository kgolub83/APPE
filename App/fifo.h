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
**                                 Guards
*******************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern */

/*******************************************************************************
**                               Includes
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/    

#define __LINUX

/*******************************************************************************
**                              Data Structures
*******************************************************************************/

typedef enum
{
    FIFO_CLEAN = 0,     /* clean exit return value */
    FIFO_EMPTY,         /* return value for fifo empty error */
    FIFO_FULL           /* return value for fifo full error */
} fifo_ret_val_t;

struct fifo_ctrl
{
    uint16_t elements;                  /* fifo depth - max fifo elements */
    const uint16_t dataElementSize;     /* size of data stored in fifo element*/
    volatile bool fifoLock;             /* control variable */
    volatile uint16_t head;             /* circular fifo head pointer */
    volatile uint16_t tail;             /* circular fifo tail pointer */
    volatile bool full;                 /* fifo full flag */
    volatile bool empty;                /* fifo empty flag */
};

typedef struct
{
    void *buffer;               /* pointer to fifo memory */
    struct fifo_ctrl ctrl;      /* buffer control variables */
} circ_fifo_t;


/*******************************************************************************
**                                Macros
*******************************************************************************/

/*define critical section guards*/

#define FifoCriticalSectionLock_m(x) (pthread_mutex_lock(x))        
#define FifoCriticalSectionUnlock_m(x) (pthread_mutex_unlock(x))

/*******************************************************************************
**                                Global and static variables
*******************************************************************************/    

    
/*******************************************************************************
**                               Public function prototypes - API
*******************************************************************************/

extern void fifoInit(struct fifo_ctrl *this);
extern fifo_ret_val_t fifoWrite(circ_fifo_t * const this, const void *data);
extern fifo_ret_val_t fifoRead(circ_fifo_t * const this, void *data);
extern int fifoDelLastElement(struct fifo_ctrl *this);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* FIFO_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
