#include <SensorData.h>


SensorData::SensorData(const uint8_t rows, const uint8_t cols) : 
    channelNum(rows),
    historyNum(cols),
    MAX_DATA_SIZE(6) // specify this value as max size of stored readings. 6bytes
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
void SensorData::save(float data, int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, "%f", data);
}

// shift should be called before save otherwise last save will be overwritten
void SensorData::save(int data, int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, "%d", data);
}


void SensorData::save(char data, int sensor) { 
    snprintf(dataArr[0][sensor], MAX_DATA_SIZE, data);
}


/////////////////////////////////////////////////////////////////
void SensorData::shift() { // make sure shift works properly
    //shift data array, removing oldest entry and filling new column with zeros
    for(uint8_t entry = 0; entry < historyNum-1; entry++){
        for(uint8_t channel = 0; channel < channelNum; channel++){  
            uint8_t replace = (historyNum-1) - entry;
            dataArr[replace][channel] = dataArr[replace - 1][channel];
        }
    }

    //initalise first column with zeros
    for(uint8_t channel = 0; channel < channelNum; channel++) {  
        dataArr[0][channel] = "0.0";  
    }

    //shift trasmit array
    for(uint8_t entry = 0; entry < historyNum-1; entry++){
        uint8_t replace = (historyNum-1) - entry;
        transmitArr[replace] = transmitArr[replace - 1];
    }
    transmitArr[0] = false; // initialise as untransmitted
}


/////////////////////////////////////////////////////////////////
float SensorData::getFloat(int sensor, int index) {
    float value = atof(dataArr[index][sensor]);
    return value;
}

float SensorData::getLastFloat(int sensor) {
    float value = atof(dataArr[0][sensor]);  //most recent stored value
    return value;
}

float SensorData::get2ndLastFloat(int sensor) {
    float value = atof(dataArr[1][sensor]); //second most recent stored value
    return value;
}

/////////////////////////////////////////////////////////////////
char SensorData::getValue(int sensor, int index) {
    char value = dataArr[index][sensor];
    return value;
}

char SensorData::getLastFloat(int sensor) {
    char value = dataArr[0][sensor];  //most recent stored value
    return value;
}

char SensorData::get2ndLastFloat(int sensor) {
    char value = dataArr[1][sensor]; //second most recent stored value, hence 1
    return value;
}


/////////////////////////////////////////////////////////////////
void SensorData::print() {  //dump array
  //return char array with \n every historyNum positions
    Serial.println("----SENSOR DATA----");
    for (uint8_t i = 0; i<historyNum; i++){
        for (uint8_t j = 0; j<channelNum; j++){
            Serial.print(dataArr[i][j]);
            Serial.print(" ");
        }
        Serial.println("");
    }
    Serial.println("-------------------");
    for (uint8_t i = 0; i<historyNum; i++) {
            Serial.print(transmitArr[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
    Serial.println("-------------------");
}
