/*******************************************************************************
**                             Includes
*******************************************************************************/   
 
#include "logger.h"
#include "fifo.h"

/*******************************************************************************
**                    Global and static variables
*******************************************************************************/  
static log_settings_t logSettings;        //private logger settings

static circ_fifo_t logFifo =            //private logger fifo instance
{
    .ctrl.dataElementSize = sizeof(log_record_t)     //logger data record structure size
};

/*VT100 coloring definitions*/
static const char *logLevelColors[] =
{
    "\e[90m",
    "\e[32m",
    "\e[34m",
    "\e[36m",
    "\e[35m",
    "\e[33m",
    "\e[31m",
    "\e[41m"
};

/*Logger level names definitions */
static const char *logLevelNames[] = 
{
    "VERBOSE",
    "TRACE", 
    "DEBUG", 
    "INFO", 
    "LOG",
    "WARN", 
    "ERROR", 
    "FATAL"
};

/*Event subtype names definitions */
static const char *eventSubtypeNames[] =
{
    "HAL",
    "SYS",
    "COM",
    "APP",
    "HMI"
};

/*******************************************************************************
**                             Code
*******************************************************************************/

/*!********************************************************
* @function logInit
* @brief Logger initialization functrion
*
* Initializes logger parameters and data structures
* whether initialization is executed properly
*
* @param logLevel define active logging level
* @param logWriteLevel define log write level
* @param logToFileEna write to log file enable
* @param stderrEna write to standard error enable
* @param vt100FormatingEna enable VT100 coloring
*
* @return whether initialization is executed properly
**********************************************************/

log_ret_val_e logInit(log_type_e logLevel, log_writeLevel_e logWriteLevel, bool logToFileEna, bool stderrEna, bool vt100FormatingEna)
{
    
    /*initialise intermideate RAM circular fifo buffer for fast logging*/ 
    logFifo.ctrl.elements = LOG_BUFFER_RECORDS;
    logFifo.buffer = (void *) &logSettings.recordBuffer; 
    fifoInit(&logFifo.ctrl);
    
    /*general logger settings*/
    logSettings.level = logLevel;
    logSettings.writeLevel = logWriteLevel;
    logSettings.logToFile = logToFileEna;
    logSettings.stdErrEnable = stderrEna;
    logSettings.vt100FormatingEnable = vt100FormatingEna;
    
    return LOG_SUCCESS;
}

/*!********************************************************
* @function logData
* @brief Runtime fast loggin function
*
* Stores log events to RAM FIFO
*
* @param *fileName log envent source file 
* @param *functionName log event source function
* @param line log event source file line number
* @param *logString user defined log string
* @param dataNo number of data integer objects to log
* @param ... log data objects values
* 
* @return whether logging succeeded
**********************************************************/
log_ret_val_e logData(const char *fileName, const char *functionName, uint16_t line, event_code_e eventCode, const char *logString, int dataNo, ...)
{
    log_record_t record;
    va_list ap;
    int i;
    
    record.level = (eventCode & LOG_LEVEL_MASK) >> LOG_LEVEL_OFFSET;

    if((logSettings.level > record.level) || (logSettings.lock == true))
    {
        return LOG_DISABLED;
    }

    va_start(ap, dataNo);
    if(LOG_MAX_DATA >= dataNo)
    {
        record.dataElementsNo = dataNo; 
    } else
    {
        record.dataElementsNo = LOG_MAX_DATA;
    }
    
    for(i=0; i<record.dataElementsNo; i++)
    {
        record.data[i] = va_arg(ap, int); 
    }
    va_end(ap);
    
    record.eventID = eventCode;
    
    if(LOG_WRITE_FULL == logSettings.writeLevel)
    {
        record.fileName[LOG_FILENAME_CHARACTERS-1] = '\0'; /*terminate static buffer */
        strncpy(record.fileName, strrchr(fileName, '/')+1, LOG_FILENAME_CHARACTERS-1);
        record.line = line;
        record.functionName[LOG_FUNCTION_CHARACTERS-1] = '\0'; /*terminate static buffer */
        strncpy(record.functionName, functionName, LOG_FUNCTION_CHARACTERS-1);
    }
    
    if((LOG_WRITE_COMPACT == logSettings.writeLevel) || (LOG_WRITE_FULL == logSettings.writeLevel))
    {
        record.logString[LOG_STRING_CHARACTERS-1] = '\0'; /*terminate static buffer*/
        strncpy(record.logString, logString, LOG_STRING_CHARACTERS-1);
    }
    
    time(&record.dateTime);
    record.cpuTime = clock();
    
    fifoWrite(&logFifo, &record);
    
    return LOG_SUCCESS;
}

/*!********************************************************
* @function writeData
* @brief Static function to write data to destination file 
*
* Writes log data to destination file
*
* @param *fp destination file
* @param *localTime log event calendar date and time 
* @param *record log event data
* @param vt100FormatingEna enable vt100 formating
* 
* @return void function
**********************************************************/

inline static void writeData(FILE *fp, struct tm *localTime, log_record_t *record, bool vt100FormatingEna)
{
    int i;
    if((LOG_WRITE_COMPACT == logSettings.writeLevel) || (LOG_WRITE_FULL == logSettings.writeLevel))
    {
        fprintf(fp, "%d-%d-%d %d:%d:%d.", localTime->tm_year+1900, localTime->tm_mon+1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    }
    
    fprintf(fp, "%ld ", record->cpuTime);
    if(LOG_WRITE_FULL == logSettings.writeLevel)
    {
        fprintf(fp, "ID:");
    }
    fprintf(fp, "%04x ", record->eventID);
    
    if(LOG_WRITE_FULL == logSettings.writeLevel)
    {
        fprintf(fp, "fl:%s fn:%s l:%d ", record->fileName, record->functionName, record->line);
        record->subtype = (record->eventID & EVENT_SUBTYPE_MASK) >> EVENT_SUBTYPE_OFFSET;
    }
    
    if((LOG_WRITE_COMPACT == logSettings.writeLevel) || (LOG_WRITE_FULL == logSettings.writeLevel))
    {
        if(vt100FormatingEna)
        {
            fprintf(fp, "%s%s\e[0m ", logLevelColors[record->level], logLevelNames[record->level]);
        } else
        {
            fprintf(fp, "%s ", logLevelNames[record->level]); 
        }
        
        if(LOG_WRITE_FULL == logSettings.writeLevel)
        {
            fprintf(fp, "%s ", eventSubtypeNames[record->subtype]);
        }
        
        fprintf(fp, "%s ", record->logString); 
    }
    
    for(i=0; i<record->dataElementsNo; i++)
    {
        fprintf(fp, "%d ", record->data[i]);
    }
    
    fprintf(fp, "\n");
    
} 

/*!********************************************************
* @function logWrite

* @brief Static function to write data to destination file 
*
* Dumps data from RAM FIFO to destination
*
* @param *function takes no arguments
* 
* @return whether dump succeeded
**********************************************************/

log_ret_val_e logWrite(void)
{
    log_record_t record;
    struct tm *localTime;
    
    logSettings.stdErrOut = STD_ERR_OUT;
    
    if(logSettings.logToFile)
    {
        logSettings.fp = fopen(LOG_FILE, "a+");
        if(NULL == logSettings.fp)
        {
            return LOG_FILE_OPEN_FAIL;
        }
    }
    
    while(false == logFifo.ctrl.empty)
    {
        fifoRead(&logFifo, &record);
        localTime = localtime(&record.dateTime);
        
        if (logSettings.stdErrEnable && logSettings.stdErrOut != NULL)
        {
            writeData(logSettings.stdErrOut, localTime, &record, logSettings.vt100FormatingEnable);
        }
        
        if(logSettings.logToFile)
        {
            writeData(logSettings.fp, localTime, &record, VT100_DISABLED);
        }
        if(NULL != logSettings.executeUserFn)     /*check if user function installed*/
        {
            logSettings.executeUserFn(&record);   /*execute user dump function */
        }
    }
    
    if(logSettings.stdErrOut != stderr)
    {
        fclose(logSettings.stdErrOut);
    }
    
    if(logSettings.logToFile)
    {
        fclose(logSettings.fp);
    }
    
    return LOG_SUCCESS;
}

/*!********************************************************
* @function installUserLogDumpFn
* 
* @brief 
*
* @param 
* 
* @return 
**********************************************************/

void installUserLogDumpFn(log_arg_fnp userFunction)
{
    logSettings.executeUserFn = userFunction;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

