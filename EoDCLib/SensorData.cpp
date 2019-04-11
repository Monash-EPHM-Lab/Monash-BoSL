#include <SensorData.h>
#include <Memory.h>

SensorData::SensorData(const uint8_t rows, const uint8_t cols):
     dataHistoryNum(rows);
     dataChannelNum(cols);
    {
    dataArray = new float [dataChannelNum][dataHistoryNum];
    memset(dataArray, 0, sizeof(dataArray))
    transmitArray = new bool [dataHistoryNum];
    for (uint8_t i = 0; i<dataHistoryNum; i++) {  
        transmitArray[i] = {true};
    }
}

 
void SensorData::shift(){
    //shift data array
    for(uint8_t channel = 0; channel < dataChannelNum; channel++){
    for(uint8_t entry = 0; entry < dataHistoryNum-1; entry++){
   
    dataArray[channel][entry] = dataArray[channel][entry+1];
   
    }
    }
    //shift trasmitt array
    for(uint8_t entry = 0; entry < dataHistoryNum-1; entry++){
   
    transmitArray[entry] = transmitArray[entry+1];
   
    }
    /*
float SensorData::getVal(void) {
    return dataArray[0][0];
} */