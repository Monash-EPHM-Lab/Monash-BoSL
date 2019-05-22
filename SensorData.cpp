#include <SensorData.h>


SensorData::SensorData(const uint8_t rows, const uint8_t cols) : 
    channelNum(rows),
    historyNum(cols)
    {

    dataArr = new float*[channelNum];
    for (uint8_t i = 0; i < channelNum; i++) {
        dataArr[i] = new float[historyNum];
        for (uint8_t j = 0; j < historyNum; j++) {
            dataArr[i][j] = i*historyNum + j;
        }
    }

  //  memset(dataArray, 0, sizeof(dataArray));
    transmitArr = new bool [historyNum];
    for (uint8_t j = 0; j<historyNum; j++) {  
        transmitArr[j] = (j+1) % 2;
    }
}

void SensorData::set(){
    
}
 
void SensorData::shift(){ // make sure shift works properly
    //shift data array
    for(uint8_t channel = 0; channel < channelNum; channel++){
        for(uint8_t entry = 0; entry < historyNum-1; entry++){
            uint8_t replace = (historyNum-1) - entry;
            dataArr[channel][replace] = dataArr[channel][replace - 1];
        }
        dataArr[channel][0] = 0.0;
    }
    //shift trasmitt array
        for(uint8_t entry = 0; entry < historyNum-1; entry++){
            uint8_t replace = (historyNum-1) - entry;
            transmitArr[replace] = transmitArr[replace - 1];
       
        }
        transmitArr[0] = 1; //its safer to set this bit to one when initiallising as we don't want to send empty data
}

void SensorData::print() {
  //return char array with \n every historyNum positions
  Serial.println("----SENSOR DATA----");
  for (uint8_t i = 0; i<channelNum; i++){
      for (uint8_t j = 0; j<historyNum; j++){
        Serial.print(dataArr[i][j]);
        Serial.print(" ");
        }
        Serial.println("");
  }
  for (uint8_t j = 0; j<historyNum; j++){
        Serial.print(transmitArr[j]);
        Serial.print(" ");
        }
        Serial.println(" ");
  Serial.println("-------------------");
}
    /*
}
float SensorData::getVal(void) {
    return dataArray[0][0];
} */