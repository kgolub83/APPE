#ifndef IODATA_H
#define IODATA_H

#include <cstdint>
#include <vector>
#include <string>
#include "c_wrapper.h"

#define TST_DATA_ATTRIBUTES     5
#define CSV_SEPARATOR           ','

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
