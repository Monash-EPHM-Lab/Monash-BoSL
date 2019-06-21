#include <SensorData.h>


SensorData::SensorData(const uint8_t rows, const uint8_t cols) : 
    channelNum(rows),
    historyNum(cols)
    {
        
    dataArr = new char**[channelNum];
    for (uint8_t i = 0; i < channelNum; i++) {
        dataArr[i] = new char*[historyNum];
        for (uint8_t j = 0; j < historyNum; j++) {
            dataArr[i][j] = new char[MAX_DATA_SIZE];
            float val = i*channelNum + j*3.142; //for testing
            saveAt(val, i, j);
        }
    }

  //  memset(dataArray, 0, sizeof(dataArray));
    transmitArr = new bool [historyNum];
    for (uint8_t j = 0; j<historyNum; j++) {  
        transmitArr[j] = 0; //for testing
    }
}

/////////////////////////////////////////////////////////////////
//saves in the most recent (first column) for given sensor index
//use saveAt() to save at any position
// shift should be called before save otherwise last save will be overwritten
// template to accept any data type. 
//should imply datatype in all cases and not require <> specification.
template <typename T>
void SensorData::saveNew(T data, int sensor) { 
    saveAt(data, sensor, 0);
}

/////////////////////////////////////////////////////////////////
// save at a specific position
//saveAt overload methods for different sensor data types
void SensorData::saveAt(float data, int sensor, int historyPos) { 
    char str_temp[MAX_DATA_SIZE];
    dtostrf(data, 4, 2, str_temp);
    snprintf(dataArr[sensor][historyPos], MAX_DATA_SIZE, "%s", str_temp);
}

void SensorData::saveAt(int data, int sensor, int historyPos) { 
    snprintf(dataArr[sensor][historyPos], MAX_DATA_SIZE, "%d", data);
}

void SensorData::saveAt(char data, int sensor, int historyPos) { 
    snprintf(dataArr[sensor][historyPos], MAX_DATA_SIZE, "%s", data);
  //  snprintf(dataArr[sensor][historyPos], MAX_DATA_SIZE, "%s", data);
}

/////////////////////////////////////////////////////////////////
void SensorData::shift() { // make sure shift works properly
    //shift data array, removing oldest entry and filling new column with zeros
    for(uint8_t channel = 0; channel < channelNum; channel++){
        for(uint8_t entry = 0; entry < historyNum-1; entry++){
            uint8_t replace = (historyNum-1) - entry;
            dataArr[channel][replace] = dataArr[channel][replace - 1];
        }
    }

    //initalise first column with zeros
    for(uint8_t channel = 0; channel < channelNum; channel++) {  
        dataArr[channel][0] = "0.0";  
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
    float value = atof(dataArr[sensor][index]);
    return value;
}

float SensorData::getLastFloat(int sensor) {
    float value = getFloat(sensor, 0);  //most recent stored value
    return value;
}

float SensorData::get2ndLastFloat(int sensor) {
    float value = getFloat(sensor, 1); //second most recent stored value
    return value;
}

/////////////////////////////////////////////////////////////////
char SensorData::getValue(int sensor, int index) {  //returns char instead of float
    char value = dataArr[sensor][index];
    return value;
}

char SensorData::getLastValue(int sensor) {
    char value = getValue(sensor, 0);  //most recent stored value
    return value;
}

char SensorData::get2ndLastValue(int sensor) {
    char value = getValue(sensor, 1); //second most recent stored value, hence 1
    return value;
}


/////////////////////////////////////////////////////////////////
void SensorData::print() {  //dump array
  //return char array with \n every historyNum positions
    Serial.println("----SENSOR DATA----");
    for (uint8_t j = 0; j<channelNum; j++){
        for (uint8_t i = 0; i<historyNum; i++){
            for (uint8_t c = 0; c<MAX_DATA_SIZE; c++) {
                int A = (int) dataArr[j][i][c];
                Serial.print(A);
                Serial.print(' ');
            }
            Serial.print('\n');
            Serial.println(dataArr[j][i]);
            //Serial.print(dataArr[j][i]);
            //Serial.print(" ");
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


    Serial.print("test: ");
    Serial.println(test);
    Serial.println((char*) test); //try this but in example test was type uint8_t
}
