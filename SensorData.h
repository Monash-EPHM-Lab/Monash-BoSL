#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>

/*index:
0 = Time        
1 = Pressure     
2 = Temp        
3 = EC          
4 = Battery                          
#define Time 0
#define Pressue 1
#define Temp 2
#define EC 3
#define Bat 4 		
Define these indecies in main script. More explicit		*/

#define MAX_DATA_SIZE 7 // max number of bytes per data point
#define FLOAT_DECIMALS 3 // max number of decimals to save float
#define FLOAT_WIDTH 5  // minimum float width


 class SensorData
 {
     public:
        SensorData(const uint8_t rows, const uint8_t cols);  //class for data structure handling sensor data
        // in shape of a table. Each column is one timestamp of sensor readings, EC, Temp, Bat...
        // Rows represent different reading sets
        void shift(void);  // method to shift array to right, removing oldest logged data making space for new data.
        void print(void); // dump (print) entire array to serial port.

    //    void saveNew(float data, int sensor); // save new data into array. sensor ID index for index at which to store data.
    //    void saveNew(int data, int sensor); //overloaded definition for different sensor data types.
    //    void saveNew(char data, int sensor); //char, int, float should cover main types.
        template <typename T>
        void saveNew(T data, int sensor); //char, int, float should cover main types.

        void saveAt(float data, int sensor, int historyPos);
        void saveAt(int data, int sensor, int historyPos);
        void saveAt(char data, int sensor, int historyPos);

        float getFloat(int sensor, int index);  // get stored sensor value at given index
        float getLastFloat(int sensor); // get most recent float value stored of given sensor
        float get2ndLastFloat(int sensor); //2nd most recent

        char getValue(int sensor, int index);  //return char value at given index. 0 is most recent
        char getLastValue(int sensor);  //return char value of most recent value
        char get2ndLastValue(int sensor); //2nd most recent

        char ***dataArr;  //make private. First axis (index) is for column or one timestamp reading. 
        //Second axis (index) is for rows or sensor value.


 //       float getVal(void);
        
     private:
        char test[6];
        const uint8_t historyNum;
        const uint8_t channelNum;
     
        bool *transmitArr;

        void commitMemory(char data, const int sensor);
};
 
 #endif