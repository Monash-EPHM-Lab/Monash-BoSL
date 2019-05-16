#include <SensorData.h>


SensorData::SensorData(const uint8_t rows, const uint8_t cols) : 
    historyNum(rows),
    channelNum(cols)
    {

    dataArr = new float*[historyNum];
    for (int i = 0; i < historyNum; i++) {
        dataArr[i] = new float[channelNum];
        for (int j = 0; j < channelNum; j++) {
            dataArr[i][j] = i+j;
        }
    }

  //  memset(dataArray, 0, sizeof(dataArray));
    transmitArr = new bool [historyNum];
    for (uint8_t i = 0; i<historyNum; i++) {  
        transmitArr[i] = {true};
    }
}

 
void SensorData::shift(){ // make sure shift works properly
    //shift data array
    for(uint8_t channel = 0; channel < channelNum; channel++){
        for(uint8_t entry = 0; entry < historyNum-1; entry++){
            int replace = historyNum - entry;
            dataArr[channel][replace - 1] = dataArr[channel][replace];
        }
        dataArr[channel][0] = 0.0;
    }
    //shift trasmitt array
    for(uint8_t entry = 0; entry < historyNum-1; entry++){
   
    transmitArr[entry] = transmitArr[entry+1];
   
    }
}

char SensorData::dumpArray() {
  //return char array with \n every historyNum positions
}
    /*
}
float SensorData::getVal(void) {
    return dataArray[0][0];
} */