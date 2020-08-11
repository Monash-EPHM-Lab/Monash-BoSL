This folder contains the program for a data logging script without SD card functionality. Running multifile-logging.ino directly from this folder will work, however you can also copy the BoSL-lib folder directly into the Arduino libraries directory and then u can (and must) run multifile-logging.ino independently.

****
If you have the BoSL-lib installed in your Arduino IDE, and also the SensorEC.cpp, SIM7000.cpp, ..., in the multifile-logging.ino directory then you will get a COMPILE ERROR. You must remove the copies of the library files if installing as a library.