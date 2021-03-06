/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file io_data.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief class handling input test data and writes data to.csv file
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

#ifndef IODATA_H
#define IODATA_H

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <cstdint>
#include <vector>
#include <string>
#include "c_wrapper.h"

/*******************************************************************************
**                               Constants
*******************************************************************************/  


#define TST_DATA_ATTRIBUTES     5U
#define CSV_SEPARATOR           ','

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef std::vector<sample_data_t> samples_vector_t;
typedef std::vector<output_data_t> output_vector_t;
typedef std::vector<data_attribute_t> settings_vector_t;
typedef samples_vector_t::iterator samples_vector_it;

enum testDataAttrib
{
    TST_DATA_SAMPLE_RATE, 
    TST_DATA_SAMPLES_NO,
    TST_DATA_RESOLUTION,
    TST_DATA_GUARD_REGION,
    TST_DATA_OUTPUT_DECODER_RANGE
};

typedef enum
{
    IO_DATA_SUCCESS = 0,
    IO_DATA_FAULT,
    IO_DATA_FILE_EMPTY
} IOdata_ret_val_t;    

/*! @TODO Should be a singleton object */    
class IOdata
{
private:
    samples_vector_t dataSamples[DATA_VECTORS_NO];
    data_attribute_t dataSampleRate;
    samples_no_t dataSamplesNo;
    data_attribute_t dataResolution;
    data_attribute_t dataGuardRegion;

    output_vector_t outputData;
    data_attribute_t decoderRange;
    
public:
    IOdata();
    ~IOdata();

    IOdata_ret_val_t readCSV(std::string fileName);
    IOdata_ret_val_t writeCSV(std::string fileName, output_vector_t&);
    samples_vector_t getSensorData(data_vector_no_t);
    void getDataAttributes(tst_data_attributes_t&);
    samples_no_t getSamplesNo(void);
};

#endif // IODATA_H

/******************************************************************************
**                               End Of File
*******************************************************************************/
