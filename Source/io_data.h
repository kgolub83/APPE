#ifndef IODATA_H
#define IODATA_H

#include <cstdint>
#include <vector>
#include <string>
#include "c_wrapper.h"

#define DATA_VECTORS_NO 2
#define DATA_VECTOR_A   0
#define DATA_VECTOR_B   1
#define CSV_SEPARATOR   ','

typedef uint8_t data_vector_no_t;
typedef std::vector<sample_data_t> samples_vector_t;
typedef std::vector<output_data_t> output_vector_t;
typedef samples_vector_t::iterator samples_vector_it;

typedef enum
{
    IO_DATA_SUCCESS = 0,
    IO_DATA_FAULT,
    IO_DATA_FILE_EMPTY
}IOdata_ret_val_t;    
    
class IOdata
{
private:
    samples_vector_t dataSamples[DATA_VECTORS_NO];
    samples_no_t dataSamplesNo;
    output_vector_t outputData;
    
public:
    IOdata();
    ~IOdata();

    IOdata_ret_val_t readCSV(std::string fileName);
    IOdata_ret_val_t writeCSV(std::string fileName, output_vector_t&);
    samples_vector_t getSensorData(data_vector_no_t);
    samples_no_t getSamplesNo(void);
};

#endif // IODATA_H
