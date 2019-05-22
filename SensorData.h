#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>

/*index:
0 = Time        
1 = Pressure     
2 = Temp        
3 = EC          
4 = Battery                          */
#define Time 0
#define Pressue 1
#define Temp 2
#define EC 3
#define Bat 4 


 class SensorData
 {
     public:
        SensorData(const uint8_t rows, const uint8_t cols);
        void shift(void);
        void print(void);
        float **dataArr;

 //       float getVal(void);
        
     private:
        const uint8_t historyNum;
        const uint8_t channelNum;
     
        bool *transmitArr;
};
 
 #endif