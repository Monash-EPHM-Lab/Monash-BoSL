#include "SensorData.h"


//class for managing sensor data to be stored and accessed on SD card

SensorData::SensorData(char* file)
    {  
    memset(dataLine, 0, MAX_LINE_SIZE); //initialise dataline as empty with 0

    transmitArr = new bool [logHistory];
    memset(transmitArr, 1, logHistory);  // initialise every element as transmitted.

    newFile(file); //initial filename
}


/////////////////////////////////////////////////////////////////
// do not pass in arguments greater than MAX_DATA_SIZE (20 characters)
// make sure total line is not longer than MAX_LINE_SIZE (100 characters)
void SensorData::composeLine(char* data, char* tag, char* del = DELIMITER) {
    size_t lnLen = strlen(dataLine);
    if (lnLen + strlen(tag) + strlen(data) + 5 < MAX_LINE_SIZE) {  
        strcat(dataLine, tag); //add tag to the existing data line
        strcat(dataLine, data); //add data 
        strcat(dataLine, del); //add delimiter
    } //check that new line will not be too large.
}

void SensorData::composeLine(int data, char* tag, char* del = DELIMITER) {
    char data_str[MAX_DATA_SIZE];
    snprintf(data_str, MAX_DATA_SIZE, "%d", data);  //convert int to char array

    composeLine(data_str, tag, del); //call 
}

void SensorData::composeLine(float data, char* tag, char* del = DELIMITER) {
    char data_str[MAX_DATA_SIZE];  
    dtostrf(data, 4, 2, data_str);  //convert float to char array

    composeLine(data_str, tag, del);
}


/////////////////////////////////////////////////////////////////
// write the composed line to SD card and then clear line
void SensorData::writeLine(void) {
    size_t lnLen = strlen(dataLine);
    dataLine[lnLen-1] = 0; // remove last delimiter

    ////////////////// begin SD card
    SdFat sd;
    SdFile file;

    if (!sd.begin(chipSelect, SD_SCK_MHZ(8))) {
        sd.initErrorHalt();
    }  // initialise sd card


    file.open(filename, O_WRONLY | O_CREAT | O_AT_END);

    file.println(dataLine);

    file.close();


    memset(dataLine, 0, MAX_LINE_SIZE);  //clear dataLine for new line.
}


/////////////////////////////////////////////////////////////////
// transmit contents of old file before creating new file.
void SensorData::newFile(char* name) {
    strcpy(filename, name);

  //  File newF = SD.open(filename, FILE_WRITE);
  //  newF.close();
}

/////////////////////////////////////////////////////////////////
void SensorData::readLine(uint8_t lineNum, char* tag = "") {
    Serial.print("---LINE: ");
    Serial.println(lineNum);

    SdFat sd;
    SdFile file;

    if (!sd.begin(chipSelect, SD_SCK_MHZ(8))) {
        sd.initErrorHalt();
    }  // initialise sd card

    file.open(filename, O_RDONLY);

    int32_t offset = (MAX_LINE_SIZE * (lineNum + 1) + 3); //characters to go back for last line

    if (offset > file.fileSize()) {
        offset = file.fileSize();
    }

    file.seekEnd(-offset);  
    //go back to the position in the file where lines will concretely be. search buffer. 

    char c;
    uint16_t totalLines = 0; //total lines in search buffer

    while (file.available()) {
        c = file.read();
        if (c == '\n') {
            totalLines++;
        }
    }

    uint32_t n=0; // number of lines read
    uint32_t i=0; // number of chars read. 
    int32_t start = 0; // start char pos of the desired line relative to search buffer

    file.seekEnd(-offset);  //go back to start of search buffer

    while (file.available()) {
        c = file.read();
        i++;
        if (c == '\n') {
            n++; //increase number of lines read count
            if (n == (totalLines - lineNum - 1)) {
                start = i;
            }
        }
    }

    file.seekEnd(start - offset);  //seek negative offset pos', then go forwards start pos'

    if (tag[0] == 0) { // empty tag so return whole line
        while (file.available()) {
            c = file.read(); 
            Serial.print(c); 
            if (c == '\n') {
                break;
            }
        }
    } else { //search for tag
        bool match = true;  //if tag matches character phrase in line
        bool searching = true; //if we are still searching for tag

        while (file.available() && searching) {
             //check if the next len(tag) chars in line match the tag
            for (int i = 0; i < strlen(tag) && match; i++) {
                c = file.read();

                if (c != tag[i]) {  // no match, so exit loop
                    match = false;
                } if (c == '\n') { //reached end of line without match so exit search
                    searching = false;
                    match = false;
                }
            }

            if (match) { // if it was a match stop searching
                searching = false;
            } else { //else keep searching for a new match
                match = true;
            }
        }

        if (match) { //we found tag, so return data until DELIMITER or \n 
            while (file.available()) {
                c = file.read();
                if (c == DELIMITER || c == "\n") {
                    break;
                }
                Serial.print(c);
            }
        }  
    }

    file.close(); // do not corrupt file
}

////////////////////////////////////////////////////////////////
void SensorData::readLast(char* tag = "") {
    readLine(0, tag);
}


/////////////////////////////////////////////////////////////////
void SensorData::dump(void) {  //dump array
    Serial.print("----SENSOR DATA---- Reading file: ");
    Serial.println(filename);

    SdFat sd;
    SdFile file;

    if (!sd.begin(chipSelect, SD_SCK_MHZ(8))) {
        sd.initErrorHalt();
    }  // initialise sd card

    file.open(filename, O_RDONLY);

    char c;
    while (file.available()) {
        c = file.read();
        Serial.print(c);
    }

    file.close();

}
