#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>


// tags for storing and retreiving data in CSV file on SD card
#define TIME_TAG "t="
#define DEPTH_TAG "depth="
#define TEMP_TAG "temp="
#define EC_TAG "EC="
#define BAT_TAG "bat="
/////////////////////////////



#define logHistory 60  // max number of readings/columns to be transmitted
#define MAX_DATA_SIZE 20 // max number of bytes per data point or tag
#define MAX_LINE_SIZE 100 // max number of bytes per line

#define DELIMITER   ',' // character used to seperate values in files
#define LF          '\n'  // linefeed or newline character. ASCII code for \n

/* pins defined for SPI atmega328p 
MOSI is pin 11 
MISO is pin 12 
SCK is pin 13  */
#define chipSelect 8 //chip select pin is #8


//class for data structure handling sensor data
 class SensorData
 {
     public:
        SensorData(char* file);
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
        bool *transmitArr;

        char dataLine[MAX_LINE_SIZE];  //line to be saved in SD card
        char filename[MAX_DATA_SIZE];  //filename max 20 characters

        byte keep_SPCR; // SPCR register var
};
 
 #endif
