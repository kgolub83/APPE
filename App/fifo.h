#ifndef FIFO_H
#define FIFO_H
/*******************************************************************************
**                               Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
/*******************************************************************************
**                               Constants
*******************************************************************************/    
#define FIFO_CLEAN    0      /* clean exit return value */
#define FIFO_EMPTY    -1     /* return value for fifo empty error */
#define FIFO_FULL     -2     /* return value for fifo full error */

/*******************************************************************************
**                              Data Structures
*******************************************************************************/

struct fifo_ctrl
{
    uint16_t elements;                   /* fifo depth - max fifo elements */
    const uint16_t dataElementSize;    /* size of data stored in fifo element*/
    volatile bool fifoLock;              /* control variable */
    volatile uint16_t head;              /* circular fifo head pointer */
    volatile uint16_t tail;              /* circular fifo tail pointer */
    volatile bool full;                  /* fifo full flag */
    volatile bool empty;                 /* fifo empty flag */
};

typedef struct
{
    void *buffer;                /* pointer to fifo memory */
    struct fifo_ctrl ctrl;      /* buffer control variables */
} circ_fifo_t;


/*******************************************************************************
**                                Macros
*******************************************************************************/

#define FifoCriticalSectionLock_m()         //define critical section guard here
#define FifoCriticalSectionUnlock_m()

/*******************************************************************************
**                                Global and static variables
*******************************************************************************/    

    
/*******************************************************************************
**                               Public function prototypes - API
*******************************************************************************/
extern void fifoInit(struct fifo_ctrl *this);
extern int fifoDelElement(struct fifo_ctrl *this);

extern int fifoWrite(circ_fifo_t * const this, const void *data);
extern int fifoRead(circ_fifo_t * const this, void *data);

#endif // FIFO_H

/******************************************************************************
**                               End Of File
*******************************************************************************/
