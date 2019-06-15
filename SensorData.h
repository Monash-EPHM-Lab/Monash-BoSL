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


 class SensorData
 {
     public:
        SensorData(const uint8_t rows, const uint8_t cols);  //class for data structure handling sensor data
        // in shape of a table. Each column is one timestamp of sensor readings, EC, Temp, Bat...
        // Rows represent different reading sets
        void shift(void);  // method to shift array to right, removing oldest logged data making space for new data.
        void print(void); // dump (print) entire array to serial port.

        void save(float data, const int sensor); // save new data into array. sensor ID index for index at which to store data.
        void save(int data, const int sensor); //overloaded definition for different sensor data types.
        void save(char data, const int sensor); //char, int, float should cover main types.

        char ***dataArr;  //make private. First axis (index) is for column or one timestamp reading. 
        //Second axis (index) is for rows or sensor value.

 //       float getVal(void);
        
     private:
        const uint8_t historyNum;
        const uint8_t channelNum;

        const uint8_t MAX_DATA_SIZE;
     
        bool *transmitArr;

        void commitMemory(char data, const int sensor);
};
 
 #endif