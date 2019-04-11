#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>

/*index:
0 = Time        seconds since last transmit
1 = Pressure    in pascal 
2 = Temp        in Celcius
3 = EC          in something
4 = Battery     in percent                      */
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
 //       float getVal(void);
        
     private:
        const uint8_t dataHistoryNum;
        const uint8_t dataChannelNum;
     
        float *dataArray;
        bool *transmitArray;
};
 
 #endif