#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>
#include <SensorData.h>
#include <SPI.h>
#include <SdFat.h>

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

#define MAX_DATA_SIZE 20 // max number of bytes per data point or tag
#define MAX_LINE_SIZE 100 // max number of bytes per line

#define DELIMITER   44 // ASCII code for , (comma) character used to seperate values in files
#define LF          10  // linefeed or newline character. ASCII code for \n

/* pins defined for SPI atmega328p 
MOSI is pin 11 
MISO is pin 12 
SCK is pin 13  */
#define chipSelect 8 //chip select pin is #8


//class for data structure handling sensor data
 class SensorData
 {
     public:
        SensorData(char* file, int logHistory);
        // manages SD card reading/writing 

        void newFile(char* name);

        //overloaded methods for acccepting different types of data to be stored
        // tag is phrase which preceeds data, e.g EC=
        void composeLine(char* data, char* tag, char* del = DELIMITER);
        void composeLine(float data, char* tag, char* del = DELIMITER);
        void composeLine(int data, char* tag, char* del = DELIMITER);

        void writeLine(void); //write the composed dataLine to SD card memory 

        void readLast(char* tag = ""); //read the last line at given tag in the file
        void readLine(uint8_t lineNum, char* tag = ""); //read the tag in the given line from the bottom 

      //  char getValue(int sensor, int index);  //return char value at given index. 0 is most recent
      //  char getLastValue(int sensor);  //return char value of most recent value

        void dump(void); // dump entire file to serial port.

        
     private:
        int historyNum;  // max number of datapoints per transmission
     
        bool *transmitArr;

        char dataLine[MAX_LINE_SIZE];  //line to be saved in SD card
        char filename[MAX_DATA_SIZE];  //filename max 20 characters

        byte keep_SPCR; // SPCR register var
};
 
 #endif