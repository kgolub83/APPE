/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief main function call 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/
#include "io_data.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function IOdata()
* 
* @brief constructor method for IOdata object
*
* @param 
*
*@return 
*******************************************************************************/
IOdata::IOdata()
{
}

IOdata::~IOdata()
{
}

IOdata_ret_val_t IOdata::readCSV(std::string fileName)
{
    std::fstream inputFile;
    std::string line, word; 
    std::stringstream streamBuffer;
    uint32_t sample;
    bool flagEmpty, flagDataInconsistency;
    settings_vector_t inputDataSettings;
    IOdata_ret_val_t retVal;
    
    inputFile.clear();
    inputFile.open(fileName, std::fstream::in);   //open input data *.csv file
    
    assert(inputFile.is_open());      //check if file opened seccessfuly
  
    //reset fault flags
    flagEmpty = false;      
    flagDataInconsistency = false;
    
    /****** Read test data settings ******/
    inputFile >> line;          //get test data format and meta data
    streamBuffer.clear();
    streamBuffer.str(line);     //convert string data to stream for stream manipulations
    
    inputDataSettings.clear();
    for(uint8_t i=0; i < TST_DATA_ATTRIBUTES; i++)
    {
        std::getline(streamBuffer, word, CSV_SEPARATOR);     //read every attributes
        sample = stoi(word);        // convert string to integer
        inputDataSettings.push_back(sample);      // add data to vector
    }
    
    dataSampleRate = inputDataSettings[TST_DATA_SAMPLE_RATE];
    dataSamplesNo = inputDataSettings[TST_DATA_SAMPLES_NO];
    dataResolution = inputDataSettings[TST_DATA_RESOLUTION];
    dataGuardRegion = inputDataSettings[TST_DATA_GUARD_REGION];
    decoderRange = inputDataSettings[TST_DATA_OUTPUT_DECODER_RANGE];
    
    /****** Read input test data vectors ******/
    for(data_vector_no_t i = 0; i< DATA_VECTORS_NO; i++) 
    {   
        inputFile >> line;          //read rows untill the end of file
        dataSamples[i].clear(); 
        streamBuffer.clear();
        streamBuffer.str(line);     //convert line string to stream
        
        while (std::getline(streamBuffer, word, CSV_SEPARATOR)) //read every sample data
        { 
            sample = stoi(word);        // convert string to integer
            dataSamples[i].push_back(sample);      // add data to vector
        } 
        
        if(dataSamples[i].empty())
        {
            flagEmpty = true;
        }
        
        if(dataSamplesNo != dataSamples[i].size())
        {
            flagDataInconsistency = true;
        }
        
        line.clear();
    }
    
    inputFile.close();
    
    if(flagEmpty)       //check for missing data
    {
        retVal = IO_DATA_FILE_EMPTY;
        std::cerr << "Input file data missing!" << std::endl;
    } else if(flagDataInconsistency)
    {
        retVal = IO_DATA_FAULT;
        std::cerr << "Input data inconsistency!" << std::endl;
    } 
    else
    {
        std::cout << dataSamplesNo << " data points successfuly loaded!" << std::endl;
        retVal = IO_DATA_SUCCESS;
    }
    
    return retVal;
}

IOdata_ret_val_t IOdata::writeCSV(std::string fileName, output_vector_t &data)
{
    std::fstream outFile;
    std::streampos lastEntry;
    IOdata_ret_val_t retVal;
    
    outFile.open(fileName, std::fstream::out);

    assert(outFile.is_open());  //check if file opened successfully
    
    std::cout << data.size();
    
    /* write input data vectors */
    for(data_vector_no_t i = 0; i<DATA_VECTORS_NO; i++)
    {
        for(auto &i: dataSamples[i])
        {
            outFile << i;
            lastEntry = outFile.tellp();    //record last entry position 
            outFile << CSV_SEPARATOR;
        }
        outFile.seekp(lastEntry, std::ios::beg);    //set position before last separator 
        outFile << std::endl;
    }
    
    /* write processed outupt data */
    for(auto &i: data)
    {
        outFile << i.output;
        lastEntry = outFile.tellp();    //record last entry position 
        outFile << CSV_SEPARATOR;
    }
    outFile.seekp(lastEntry, std::ios::beg);    //set position before last separator 
    outFile << std::endl;
    
    /* write processing flags */
    for(auto &i: data)
    {
        outFile << i.flags;
        lastEntry = outFile.tellp();    //record last entry position 
        outFile << CSV_SEPARATOR;
    }
    outFile.seekp(lastEntry, std::ios::beg);    //set position before last separator 
    outFile << std::endl;
    
    /* write output states */
    for(auto &i: data)
    {
        outFile << i.state;
        lastEntry = outFile.tellp();    //record last entry position 
        outFile << CSV_SEPARATOR;
    }
    outFile.seekp(lastEntry, std::ios::beg);    //set position before last separator 
    outFile << std::endl;    
    
    /*write debug data*/
    for(data_vector_no_t k = 0; k<(DEBUG_VECTORS*DATA_VECTORS_NO); k++)
    {
        for(auto &i: data)
        {
            outFile << i.outputDebug[k];
            lastEntry = outFile.tellp();    //record last entry position 
            outFile << CSV_SEPARATOR;
        }
        outFile.seekp(lastEntry, std::ios::beg);    //set position before last separator 
        outFile << std::endl;
    }
    
    outFile.close();
    retVal = IO_DATA_SUCCESS;
    
    return retVal;
}

samples_vector_t IOdata::getSensorData(data_vector_no_t vectorNo)
{
    return dataSamples[vectorNo];
}

void IOdata::getDataAttributes(tst_data_attributes_t& attributes)
{
    attributes.sampleRate = dataSampleRate;
    attributes.samplesNo = dataSamplesNo;
    attributes.resolution = dataResolution;
    attributes.guardRegion = dataGuardRegion;
    attributes.decoderRange = decoderRange;
}

samples_no_t IOdata::getSamplesNo(void)
{
    return dataSamplesNo;
}


/******************************************************************************
**                               End Of File
*******************************************************************************/


