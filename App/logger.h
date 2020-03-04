/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file logger.h
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
**                                 Guards
*******************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                               Includes
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/   

#define STD_ERR_OUT               stderr        /*standard error output definition*/
/*#define STD_ERR_OUT                 (fopen("Data/std_err_redirect.log", "a+"))*/
#define LOG_FILE                    "Data/log_file.log"  /*log file name*/
#define LOG_STRING_CHARACTERS       64U             /*user defined logger mesage max characters*/
#define LOG_FILENAME_CHARACTERS     24U             /*current log file source record max characters*/
#define LOG_FUNCTION_CHARACTERS     24U             /*urrent log function record max character*/
#define LOG_MAX_DATA                5U              /*max data containers per record*/
#define LOG_BUFFER_RECORDS          4096U           /*RAM log records FIFO buffer elements*/

#define LOG_LEVEL_POSITION          0x1000          /*1 represents nibble (4 bits) position for log level encoding*/
#define LOG_LEVEL_MASK              (LOG_LEVEL_POSITION * 0xF)
#define LOG_LEVEL_OFFSET            (uint8_t)(log(LOG_LEVEL_POSITION)/log(2))
#define EVENT_SUBTYPE_POSITION      0x0100          /*1 represents nibble (4 bits) position for log event subtype encoding*/
#define EVENT_SUBTYPE_MASK          (EVENT_SUBTYPE_POSITION * 0xF)
#define EVENT_SUBTYPE_OFFSET        (uint8_t)(log(EVENT_SUBTYPE_POSITION)/log(2))

#define VT100_ENABLED               true
#define VT100_DISABLED              false
#define LOG_TO_FILE_ENABLED         true
#define LOG_TO_FILE_DISABLED        false
#define WRITE_TO_STDERR_ENABLED     true
#define WRITE_TO_STDERR_DISABLED    false

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*User macro definitions for logging actions*/
#define LogFull_m(CODE, STRING, DATA_NO, ...)       logData(__FILE__, __FUNCTION__, __LINE__, CODE, STRING, DATA_NO, __VA_ARGS__)
#define LogCompact_m(CODE, STRING, DATA_NO, ...)    logData("/", "", 0, CODE, STRING, DATA_NO, __VA_ARGS__)
#define LogBasic_m(CODE, DATA_NO, ...)              logData("/", "", 0, CODE, "", DATA_NO, __VA_ARGS__)

/*******************************************************************************
**                              Data Structures
*******************************************************************************/

/*Return function values definitions for logger functions */
typedef enum 
{
    LOG_SUCCESS = 0,
    LOG_FAIL,             
    LOG_DISABLED,          
    LOG_FILE_CREATION_FAIL,
    LOG_FILE_OPEN_FAIL,     
    LOG_BUFFER_EMPTY        
} log_ret_val_e;

/*log levels definitions*/
typedef enum
{
    VERBOSE,
    TRACE,
    DEBUG,
    INFO,
    LOG,
    WARNING,
    ERROR,
    FATAL,
    LOG_LEVELS_NO
} log_type_e;

/*log subtypes definitions*/
typedef enum
{
    HAL,
    SYS,
    COM,
    APP,
    HMI,
    EVENT_SUBTYPES_NO
} event_subtype_e;

/*log dump write level definitions*/
typedef enum
{
    LOG_WRITE_FULL,
    LOG_WRITE_COMPACT,
    LOG_WRITE_BASIC,
    LOG_writeLevelS_NO
} log_writeLevel_e;

/*******************************************************************************
 * Log evenets definitions -> use place holders to define user events
 * 
 * HIERARCHY:
 * LOG_LEVEL
 *     -> LOG_EVENT_SUBTYPE
 *         -> UserEvents - place user events here
 * 
 * *****************************************************************************/
typedef enum 
{
    VERBOSE_EVENT = VERBOSE*LOG_LEVEL_POSITION,    /*verbose events starts here*/
        VERBOSE_HAL_EVENT = VERBOSE_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here verbose hardware abstraction layer/drivers events*/
            
        VERBOSE_SYS_EVENT = VERBOSE_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here verbose system/OS events*/
            
        VERBOSE_COM_EVENT = VERBOSE_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here verbose communication events*/
            
        VERBOSE_APP_EVENT = VERBOSE_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here verbose application events*/
            
        VERBOSE_HMI_EVENT = VERBOSE_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here verbose presentation events*/
            
    TRACE_EVENT = TRACE*LOG_LEVEL_POSITION,        /*trace events starts here*/
        TRACE_HAL_EVENT = TRACE_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here TRACE hardware abstraction layer/drivers events*/
            
        TRACE_SYS_EVENT = TRACE_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here TRACE system/OS events*/
            
        TRACE_COM_EVENT = TRACE_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here TRACE communication events*/
            
        TRACE_APP_EVENT = TRACE_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here TRACE application events*/
            TRACE_APPE_OUTPUT,
        TRACE_HMI_EVENT = TRACE_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here TRACE presentation events*/
        
    DEBUG_EVENT = DEBUG*LOG_LEVEL_POSITION,        /*debug events starts here*/
        DEBUG_HAL_EVENT = DEBUG_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here DEBUG hardware abstraction layer/drivers events*/
            
        DEBUG_SYS_EVENT = DEBUG_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here DEBUG system/OS events*/
            
        DEBUG_COM_EVENT = DEBUG_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here DEBUG communication events*/
            
        DEBUG_APP_EVENT = DEBUG_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here DEBUG application events*/
            
        DEBUG_HMI_EVENT = DEBUG_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here DEBUG presentation events*/
        
    INFO_EVENT = INFO*LOG_LEVEL_POSITION,        /*info events starts here*/
        INFO_HAL_EVENT = INFO_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here INFO hardware abstraction layer/drivers events*/
            
        INFO_SYS_EVENT = INFO_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here INFO system/OS events*/
            SYS_INIT,
            PROGRAM_START_SUCCESS,
            DECODING_LUT_CHECKED,
            SYSTEM_STATE_CHANGED,
        INFO_COM_EVENT = INFO_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here INFO communication events*/
            
        INFO_APP_EVENT = INFO_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here INFO application events*/
            
        INFO_HMI_EVENT = INFO_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here INFO presentation events*/
        
    LOG_EVENT = LOG*LOG_LEVEL_POSITION,        /*log events starts here*/
        LOG_HAL_EVENT = LOG_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here LOG hardware abstraction layer/drivers events*/
            
        LOG_SYS_EVENT = LOG_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here LOG system/OS events*/
            
        LOG_COM_EVENT = LOG_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here LOG communication events*/
            
        LOG_APP_EVENT = LOG_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here LOG application events*/
            
        LOG_HMI_EVENT = LOG_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here LOG presentation events*/
        
    WARNING_EVENT = WARNING*LOG_LEVEL_POSITION,        /*warning events starts here*/
        WARNING_HAL_EVENT = WARNING_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here WARNING hardware abstraction layer/drivers events*/
            
        WARNING_SYS_EVENT = WARNING_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here WARNING system/OS events*/
            WAR_DATA_DIFFERENCE,
            WAR_SIGNAL_ASYMMETRY,
        WARNING_COM_EVENT = WARNING_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here WARNING communication events*/

        WARNING_APP_EVENT = WARNING_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here WARNING application events*/
            WAR_SUPERVISOR,
            WAR_SUPERVISOR_COM,
            WAR_SUPERVISOR_SYS,
            WAR_SUPERVISOR_DATA_DIF,
            WAR_SUPERVISOR_SIGNAL_DIF,
        WARNING_HMI_EVENT = WARNING_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here WARNING presentation events*/
        
    ERROR_EVENT = ERROR*LOG_LEVEL_POSITION,        /*error events starts here*/
        ERROR_HAL_EVENT = ERROR_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here ERROR hardware abstraction layer/drivers events*/
            ERR_SIGNAL_OUT_OF_BOUNDS,
        ERROR_SYS_EVENT = ERROR_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here ERROR system/OS events*/
            ERR_DECODING_LUT,
            ERR_SYS_COM_FAULT,
        ERROR_COM_EVENT = ERROR_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here ERROR communication events*/
            ERR_XTEA_DECRYPTION,
            ERR_SIP_SIGNATURE,
            ERR_CRC_CHECKSUM,
            ERR_COM_SEQUENCE,
            ERR_COM_CHANNEL_A,
            ERR_COM_CHANNEL_B,
            ERR_SYS_COM,
        ERROR_APP_EVENT = ERROR_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here ERROR application events*/
            
        ERROR_HMI_EVENT = ERROR_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here ERROR presentation events*/
        
    FATAL_EVENT = FATAL*LOG_LEVEL_POSITION,        /*fatal events starts here*/
        FATAL_HAL_EVENT = FATAL_EVENT + HAL*EVENT_SUBTYPE_POSITION, /*place here FATAL hardware abstraction layer/drivers events*/
            FATAL_DECODING_LUTS,
        FATAL_SYS_EVENT = FATAL_EVENT + SYS*EVENT_SUBTYPE_POSITION, /*place here FATAL system/OS events*/
            FATAL_PROCESSING_FAULT,
        FATAL_COM_EVENT = FATAL_EVENT + COM*EVENT_SUBTYPE_POSITION, /*place here FATAL communication events*/
            
        FATAL_APP_EVENT = FATAL_EVENT + APP*EVENT_SUBTYPE_POSITION, /*place here FATAL application events*/
            FATAL_SUPERVISOR_SYS,
            FATAL_SUPERVISOR_DATA,
        FATAL_HMI_EVENT = FATAL_EVENT + HMI*EVENT_SUBTYPE_POSITION, /*place here FATAL presentation events*/
            
    NUM_OF_LOG_EVENTS
} event_code_e;

/*log record structure definition*/
typedef struct
{
    event_code_e eventID; 
    time_t dateTime;
    clock_t cpuTime;
    log_type_e level;
    event_subtype_e subtype;
    uint16_t line;
    char fileName[LOG_FILENAME_CHARACTERS];
    char functionName[LOG_FUNCTION_CHARACTERS];
    char logString[LOG_STRING_CHARACTERS];
    uint8_t dataElementsNo; 
    int data[LOG_MAX_DATA];
} log_record_t;

typedef int (*log_arg_fnp)(const log_record_t * record);        /*user function pointer with log record argument type definition*/
typedef void (*log_fnp)(void);                                  /*user void function pointer*/

/*logger settings structure definition*/
typedef struct
{
    FILE *fp;
    FILE *stdErrOut;
    volatile bool lock;
    volatile log_record_t recordBuffer[LOG_BUFFER_RECORDS];
    log_type_e level;
    log_writeLevel_e writeLevel; 
    bool stdErrEnable;    
    bool logToFile;
    bool vt100FormatingEnable;
    log_arg_fnp executeUserFn;
    log_fnp runLockFn;
    log_fnp runUnlockFn;
} log_settings_t;
    
/*******************************************************************************
**                               Public function prototypes - API
*******************************************************************************/

extern void installUserLogDumpFn(log_arg_fnp userFunction);
extern log_ret_val_e logInit(log_type_e logLevel, log_writeLevel_e logWriteLevel, bool logToFileEna, bool stderrEna, bool vt100FormatingEna);
extern log_ret_val_e logData(const char *filename, const char *functionName, uint16_t line, event_code_e eventCode, const char *logString, uint32_t dataNo, ...);
extern log_ret_val_e logWrite(void);
extern void logTest(const char *message);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* LOGGER_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
