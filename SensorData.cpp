#include <SensorData.h>


SensorData::SensorData(const uint8_t rows, const uint8_t cols) : 
    channelNum(rows),
    historyNum(cols),
    MAX_DATA_SIZE(8) // specify this value 
    {

    dataArr = new char**[historyNum];
    for (uint8_t i = 0; i < historyNum; i++) {
        dataArr[i] = new char*[channelNum];
        for (uint8_t j = 0; j < channelNum; j++) {
            dataArr[i][j] = new char[MAX_DATA_SIZE];
            float val = (float) i*channelNum + j; //for testing
            snprintf(dataArr[i][j], MAX_DATA_SIZE, "%f", val);
            //dataArr[i][j] = i*channelNum + j;
        }
    }

  //  memset(dataArray, 0, sizeof(dataArray));
    transmitArr = new bool [historyNum];
    for (uint8_t j = 0; j<historyNum; j++) {  
        transmitArr[j] = (j+1) % 2; //for testing
    }
}

/////////////////////////////////////////////////////////////////
//save overload methods for different sensor data types
void SensorData::save(float data, const int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, "%f", data);
}

// shift should be called before save otherwise last save will be overwritten
void SensorData::save(int data, const int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, "%d", data);
}


void SensorData::save(char data, const int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, data);
}
/////////////////////////////////////////////////////////////////
 
void SensorData::shift() { // make sure shift works properly
    //shift data array
    for(uint8_t entry = 0; entry < historyNum-1; entry++){
        for(uint8_t channel = 0; channel < channelNum; channel++){  // swap indecies
            uint8_t replace = (historyNum-1) - entry;
            dataArr[replace][channel] = dataArr[replace - 1][channel];
        }
        dataArr[0][channel] = "0.0";  
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
    for (uint8_t i = 0; i<historyNum; i++){
        for (uint8_t i = 0; j<channelNum; j++){
            Serial.print(dataArr[i][j]);
            Serial.print(" ");
        }
        Serial.println("");
    }
    Serial.println("-------------------");
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
