/***************************************************
  This is a library for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Library modified for SIM7000 by Botletics: https://github.com/botletics
// Further modified by Boris Deletic - Monash University Civil Engineering

#include "SIM7000.h"




SIM7000::SIM7000(void) : _type(SIM7000A)
{
  // apn = F("FONAnet");
  apn = F("");
  apnusername = 0;
  apnpassword = 0;
  mySerial = 0;
  httpsredirect = false;
  useragent = F("FONA");
  ok_reply = F("OK");
}

uint8_t SIM7000::type(void) {
  return _type;
}

boolean SIM7000::begin(Stream &port) {
  mySerial = &port;

  DEBUG_PRINTLN(F("Attempting to open comm with ATs"));
  // give 7 seconds to reboot
  int16_t timeout = 7000;

  while (timeout > 0) {
    while (mySerial->available()) mySerial->read();
    if (sendCheckReply(F("AT"), ok_reply))
      break;
    while (mySerial->available()) mySerial->read();
    if (sendCheckReply(F("AT"), F("AT"))) 
      break;
    delay(500);
    timeout-=500;
  }

  if (timeout <= 0) {
#ifdef ADAFRUIT_FONA_DEBUG
    DEBUG_PRINTLN(F("Timeout: No response to AT... last ditch attempt."));
#endif
    sendCheckReply(F("AT"), ok_reply);
    delay(100);
    sendCheckReply(F("AT"), ok_reply);
    delay(100);
    sendCheckReply(F("AT"), ok_reply);
    delay(100);
  }

  // turn off Echo!
  sendCheckReply(F("ATE0"), ok_reply);
  delay(100);

  if (! sendCheckReply(F("ATE0"), ok_reply)) {
    return false;
  }

  // turn on hangupitude

  delay(100);
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN("ATI");

  mySerial->println("ATI");
  readline(500, true);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);




  if (prog_char_strstr(replybuffer, (prog_char *)F("SIM7000A")) != 0) {
    _type = SIM7000A;
  } else if (prog_char_strstr(replybuffer, (prog_char *)F("SIM7000C")) != 0) {
    _type = SIM7000C;
  } else if (prog_char_strstr(replybuffer, (prog_char *)F("SIM7000E")) != 0) {
    _type = SIM7000E;
  } else if (prog_char_strstr(replybuffer, (prog_char *)F("SIM7000G")) != 0) {
    _type = SIM7000G;
  } 

#if defined(FONA_PREF_SMS_STORAGE)
    sendCheckReply(F("AT+CPMS=" FONA_PREF_SMS_STORAGE "," FONA_PREF_SMS_STORAGE "," FONA_PREF_SMS_STORAGE), ok_reply);
#endif

  return true;
}


/********* Serial port ********************************************/


boolean SIM7000::setBaudrate(uint16_t baud) {
  return sendCheckReply(F("AT+IPR="), baud, ok_reply);
}


/********* POWER, BATTERY & ADC ********************************************/

/* returns value in mV (uint16_t) */
boolean SIM7000::getBattVoltage(uint16_t *v) {
  	return sendParseReply(F("AT+CBC"), F("+CBC: "), v, ',', 2);
}


/* powers down the SIM module */
boolean SIM7000::powerDown(void) {
  if (! sendCheckReply(F("AT+CPOWD=1"), F("NORMAL POWER DOWN"))) { // Normal power off
    return false;
  }
  return true;
}


/* returns the percentage charge of battery as reported by sim800 */
boolean SIM7000::getBattPercent(uint16_t *p) {
  return sendParseReply(F("AT+CBC"), F("+CBC: "), p, ',', 1);
}

boolean SIM7000::getADCVoltage(uint16_t *v) {
  return sendParseReply(F("AT+CADC?"), F("+CADC: 1,"), v);
}


/********* POWER, BATTERY & ADC ********************************************/

// Uses the AT+CFUN command to set functionality (refer to AT+CFUN in manual)
// 0 --> Minimum functionality
// 1 --> Full functionality
// 4 --> Disable RF
// 5 --> Factory test mode
// 6 --> Restarts module
// 7 --> Offline mode
boolean SIM7000::setFunctionality(uint8_t option) {
  return sendCheckReply(F("AT+CFUN="), option, ok_reply);
}

// 2  - Automatic
// 13 - GSM only
// 38 - LTE only
// 51 - GSM and LTE only
boolean SIM7000::setPreferredMode(uint8_t mode) {
  return sendCheckReply(F("AT+CNMP="), mode, ok_reply);
}

// 1 - CAT-M
// 2 - NB-IoT
// 3 - CAT-M and NB-IoT
boolean SIM7000::setPreferredLTEMode(uint8_t mode) {
  return sendCheckReply(F("AT+CMNB="), mode, ok_reply);
}

// Useful for choosing a certain carrier only
// For example, AT&T uses band 12 in the US for LTE CAT-M
// whereas Verizon uses band 13
// Mode: "CAT-M" or "NB-IOT"
// Band: The cellular EUTRAN band number
boolean SIM7000::setOperatingBand(const char * mode, uint8_t band) {
  char cmdBuff[24];

  sprintf(cmdBuff, "AT+CBANDCFG=\"%s\",%i", mode, band);

  return sendCheckReply(cmdBuff, ok_reply);
}

// Sleep mode reduces power consumption significantly while remaining registered to the network
// NOTE: USB port must be disconnected before this will take effect
boolean SIM7000::enableSleepMode(bool onoff) {
  return sendCheckReply(F("AT+CSCLK="), onoff, ok_reply);
}

// Set e-RX parameters
// Mode options:
// 0  Disable the use of eDRX
// 1  Enable the use of eDRX
// 2  Enable the use of eDRX and auto report
// 3  Disable the use of eDRX(Reserved)

// Connection type options:
// 4 - CAT-M
// 5 - NB-IoT

// See AT command manual for eDRX values (options 0-15)

// NOTE: Network must support eDRX mode
boolean SIM7000::set_eDRX(uint8_t mode, uint8_t connType, char * eDRX_val) {
  if (strlen(eDRX_val) > 4) return false;

  char auxStr[21];

  sprintf(auxStr, "AT+CEDRXS=%i,%i,%s", mode, connType, eDRX_val);

  return sendCheckReply(auxStr, ok_reply);
}

// NOTE: Network must support PSM and modem needs to restart before it takes effect
boolean SIM7000::enablePSM(bool onoff) {
  return sendCheckReply(F("AT+CPSMS="), onoff, ok_reply);
}

// Enable, disable, or set the blinking frequency of the network status LED
// Default settings are the following:
// Not connected to network --> 1,64,800
// Connected to network     --> 2,64,3000
// Data connection enabled  --> 3,64,300
boolean SIM7000::setNetLED(bool onoff, uint8_t mode, uint16_t timer_on, uint16_t timer_off) {
  if (onoff) {
    if (! sendCheckReply(F("AT+CNETLIGHT=0"), ok_reply)) return false;

    if (mode > 0) {
      char auxStr[24];

      sprintf(auxStr, "AT+SLEDS=%i,%i,%i", mode, timer_on, timer_off);

      return sendCheckReply(auxStr, ok_reply);
    }
    else {
      return false;
    }
  }
  else {
    return sendCheckReply(F("AT+CNETLIGHT=0"), ok_reply);
  }
}

/********* SIM ***********************************************************/

uint8_t SIM7000::unlockSIM(char *pin)
{
  char sendbuff[14] = "AT+CPIN=";
  sendbuff[8] = pin[0];
  sendbuff[9] = pin[1];
  sendbuff[10] = pin[2];
  sendbuff[11] = pin[3];
  sendbuff[12] = '\0';

  return sendCheckReply(sendbuff, ok_reply);
}

uint8_t SIM7000::getSIMCCID(char *ccid) {
  getReply(F("AT+CCID"));
  // up to 28 chars for reply, 20 char total ccid
  if (replybuffer[0] == '+') {
    // fona 3g?
    strncpy(ccid, replybuffer+8, 20);
  } else {
    // fona 800 or 800
    strncpy(ccid, replybuffer, 20);
  }
  ccid[20] = 0;

  readline(); // eat 'OK'

  return strlen(ccid);
}

/********* IMEI **********************************************************/

uint8_t SIM7000::getIMEI(char *imei) {
  getReply(F("AT+GSN"));

  // up to 15 chars
  strncpy(imei, replybuffer, 15);
  imei[15] = 0;

  readline(); // eat 'OK'

  return strlen(imei);
}

/********* NETWORK *******************************************************/

uint8_t SIM7000::getNetworkStatus(void) {
  uint16_t status;

  if (_type >= SIM7000A) {
    if (! sendParseReply(F("AT+CGREG?"), F("+CGREG: "), &status, ',', 1)) return 0;
  }
  else {
    if (! sendParseReply(F("AT+CREG?"), F("+CREG: "), &status, ',', 1)) return 0;
  }

  return status;
}


uint8_t SIM7000::getRSSI(void) {
  uint16_t reply;

  if (! sendParseReply(F("AT+CSQ"), F("+CSQ: "), &reply) ) return 0;

  return reply;
}




/********* TIME **********************************************************/

/*
boolean SIM7000::enableNetworkTimeSync(boolean onoff) {
  if (onoff) {
    if (! sendCheckReply(F("AT+CLTS=1"), ok_reply))
      return false;
  } else {
    if (! sendCheckReply(F("AT+CLTS=0"), ok_reply))
      return false;
  }

  flushInput(); // eat any 'Unsolicted Result Code'

  return true;
}
*/

boolean SIM7000::enableNTPTimeSync(boolean onoff, FONAFlashStringPtr ntpserver) {
  if (onoff) {
    if (! sendCheckReply(F("AT+CNTPCID=1"), ok_reply))
      return false;

    mySerial->print(F("AT+CNTP=\""));
    if (ntpserver != 0) {
      mySerial->print(ntpserver);
    } else {
      mySerial->print(F("pool.ntp.org"));
    }
    mySerial->println(F("\",0"));
    readline(FONA_DEFAULT_TIMEOUT_MS);
    if (strcmp(replybuffer, "OK") != 0)
      return false;

    if (! sendCheckReply(F("AT+CNTP"), ok_reply, 10000))
      return false;

    uint16_t status;
    readline(10000);
    if (! parseReply(F("+CNTP:"), &status))
      return false;
  } else {
    if (! sendCheckReply(F("AT+CNTPCID=0"), ok_reply))
      return false;
  }

  return true;
}

boolean SIM7000::getTime(char *buff, uint16_t maxlen) {
  getReply(F("AT+CCLK?"), (uint16_t) 10000);
  if (strncmp(replybuffer, "+CCLK: ", 7) != 0)
    return false;

  char *p = replybuffer+7;
  uint16_t lentocopy = min(maxlen-1, (int)strlen(p));
  strncpy(buff, p, lentocopy+1);
  buff[lentocopy] = 0;

  readline(); // eat OK

  return true;
}

/********* Real Time Clock ********************************************/

boolean SIM7000::readRTC(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *hr, uint8_t *min, uint8_t *sec) {
  uint16_t v;
  if (! sendParseReply(F("AT+CCLK?"), F("+CCLK: "), &v, '/', 0) ) return false;
  *year = v;

  DEBUG_PRINTLN(*year);

  return true;
}

boolean SIM7000::enableRTC(uint8_t i) {
  if (! sendCheckReply(F("AT+CLTS="), i, ok_reply))
    return false;
  return sendCheckReply(F("AT&W"), ok_reply);
}


/********* GPRS **********************************************************/

boolean SIM7000::enableGPRS(boolean onoff) {
  if (onoff) {
    /*
    // if (_type < SIM7000A) { // UNCOMMENT FOR LTE ONLY!
      // disconnect all sockets
      sendCheckReply(F("AT+CIPSHUT"), F("SHUT OK"), 20000);

      if (! sendCheckReply(F("AT+CGATT=1"), ok_reply, 10000))
        return false;

    // set bearer profile! connection type GPRS
    if (! sendCheckReply(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""), ok_reply, 10000))
      return false;
    // } // UNCOMMENT FOR LTE ONLY! */

    delay(200); // This seems to help the next line run the first time
    
    // set bearer profile access point name
    if (apn) {
      // Send command AT+SAPBR=3,1,"APN","<apn value>" where <apn value> is the configured APN value.
      if (! sendCheckReplyQuoted(F("AT+SAPBR=3,1,\"APN\","), apn, ok_reply, 10000))
        return false;

      // if (_type < SIM7000A) { // UNCOMMENT FOR LTE ONLY!
        // send AT+CSTT,"apn","user","pass"
        flushInput();

        mySerial->print(F("AT+CSTT=\""));
        mySerial->print(apn);
        if (apnusername) {
          mySerial->print("\",\"");
          mySerial->print(apnusername);
        }
        if (apnpassword) {
          mySerial->print("\",\"");
          mySerial->print(apnpassword);
        }
        mySerial->println("\"");

        DEBUG_PRINT(F("\t---> ")); DEBUG_PRINT(F("AT+CSTT=\""));
        DEBUG_PRINT(apn);
        
        if (apnusername) {
          DEBUG_PRINT("\",\"");
          DEBUG_PRINT(apnusername); 
        }
        if (apnpassword) {
          DEBUG_PRINT("\",\"");
          DEBUG_PRINT(apnpassword); 
        }
        DEBUG_PRINTLN("\"");
        
        if (! expectReply(ok_reply)) return false;
      // } // UNCOMMENT FOR LTE ONLY!
    
      // set username/password
      if (apnusername) {
        // Send command AT+SAPBR=3,1,"USER","<user>" where <user> is the configured APN username.
        if (! sendCheckReplyQuoted(F("AT+SAPBR=3,1,\"USER\","), apnusername, ok_reply, 10000))
          return false;
      }
      if (apnpassword) {
        // Send command AT+SAPBR=3,1,"PWD","<password>" where <password> is the configured APN password.
        if (! sendCheckReplyQuoted(F("AT+SAPBR=3,1,\"PWD\","), apnpassword, ok_reply, 10000))
          return false;
      }
    }

    // open bearer
    if (! sendCheckReply(F("AT+SAPBR=1,1"), ok_reply, 30000))
      return false;

    // if (_type < SIM7000A) { // UNCOMMENT FOR LTE ONLY!
      // bring up wireless connection
      if (! sendCheckReply(F("AT+CIICR"), ok_reply, 10000))
        return false;
    // } // UNCOMMENT FOR LTE ONLY!
  } else {
    // disconnect all sockets
    if (! sendCheckReply(F("AT+CIPSHUT"), F("SHUT OK"), 20000))
      return false;

    // close bearer
    if (! sendCheckReply(F("AT+SAPBR=0,1"), ok_reply, 10000))
      return false;

    // if (_type < SIM7000A) { // UNCOMMENT FOR LTE ONLY!
      if (! sendCheckReply(F("AT+CGATT=0"), ok_reply, 10000))
        return false;
  // } // UNCOMMENT FOR LTE ONLY!

  }
  return true;
}

void SIM7000::getNetworkInfo(void) {
	getReply(F("AT+CPSI?"));
	getReply(F("AT+COPS?"));
}

int8_t SIM7000::GPRSstate(void) {
  uint16_t state;

  if (! sendParseReply(F("AT+CGATT?"), F("+CGATT: "), &state) )
    return -1;

  return state;
}

void SIM7000::setNetworkSettings(FONAFlashStringPtr apn,
              FONAFlashStringPtr username, FONAFlashStringPtr password) {
  this->apn = apn;
  this->apnusername = username;
  this->apnpassword = password;

  if (_type >= SIM7000A) sendCheckReplyQuoted(F("AT+CGDCONT=1,\"IP\","), apn, ok_reply, 10000);
}

boolean SIM7000::getGSMLoc(uint16_t *errorcode, char *buff, uint16_t maxlen) {

  getReply(F("AT+CIPGSMLOC=1,1"), (uint16_t)10000);

  if (! parseReply(F("+CIPGSMLOC: "), errorcode))
    return false;

  char *p = replybuffer+14;
  uint16_t lentocopy = min(maxlen-1, (int)strlen(p));
  strncpy(buff, p, lentocopy+1);

  readline(); // eat OK

  return true;
}

boolean SIM7000::getGSMLoc(float *lat, float *lon) {

  uint16_t returncode;
  char gpsbuffer[120];

  // make sure we could get a response
  if (! getGSMLoc(&returncode, gpsbuffer, 120))
    return false;

  // make sure we have a valid return code
  if (returncode != 0)
    return false;

  // +CIPGSMLOC: 0,-74.007729,40.730160,2015/10/15,19:24:55
  // tokenize the gps buffer to locate the lat & long
  char *longp = strtok(gpsbuffer, ",");
  if (! longp) return false;

  char *latp = strtok(NULL, ",");
  if (! latp) return false;

  *lat = atof(latp);
  *lon = atof(longp);

  return true;
}

boolean SIM7000::postData(const char *request_type, const char *URL, const char *body, const char *token, uint32_t bodylen) {
  // NOTE: Need to open socket/enable GPRS before using this function
  // char auxStr[64];

  // Make sure HTTP service is terminated so initialization will run
  sendCheckReply(F("AT+HTTPTERM"), ok_reply, 10000);

  // Initialize HTTP service
  if (! sendCheckReply(F("AT+HTTPINIT"), ok_reply, 10000))
    return false;

  // Set HTTP parameters
  if (! sendCheckReply(F("AT+HTTPPARA=\"CID\",1"), ok_reply, 10000))
    return false;

  // Specify URL
  char urlBuff[strlen(URL) + 22];

  sprintf(urlBuff, "AT+HTTPPARA=\"URL\",\"%s\"", URL);

  if (! sendCheckReply(urlBuff, ok_reply, 10000))
    return false;

  // Perform request based on specified request Type
  if (strlen(body) > 0) bodylen = strlen(body);

  if (request_type == "GET") {
  	if (! sendCheckReply(F("AT+HTTPACTION=0"), ok_reply, 10000))
    	return false;
  }
  else if (request_type == "POST" && bodylen > 0 ) { // POST with content body
  	if (! sendCheckReply(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""), ok_reply, 10000))
    	return false;

    if (strlen(token) > 0) {
      char tokenStr[strlen(token) + 55];

	  	sprintf(tokenStr, "AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer %s\"", token);

	  	if (! sendCheckReply(tokenStr, ok_reply, 10000))
	  		return false;
	  }

    char dataBuff[sizeof(bodylen) + 20];

		sprintf(dataBuff, "AT+HTTPDATA=%d,10000", bodylen);
		if (! sendCheckReply(dataBuff, "DOWNLOAD", 10000))
	    return false;

    delay(100); // Needed for fast baud rates (ex: 115200 baud with SAMD21 hardware serial)

		if (! sendCheckReply(body, ok_reply, 10000))
	    return false;

  	if (! sendCheckReply(F("AT+HTTPACTION=1"), ok_reply, 10000))
    	return false;
  }
  else if (request_type == "POST" && bodylen == 0) { // POST with query parameters
  	if (! sendCheckReply(F("AT+HTTPACTION=1"), ok_reply, 10000))
    	return false;
  }
  else if (request_type == "HEAD") {
  	if (! sendCheckReply(F("AT+HTTPACTION=2"), ok_reply, 10000))
    	return false;
  }

  // Parse response status and size
  uint16_t status, datalen;
  readline(10000);
  if (! parseReply(F("+HTTPACTION:"), &status, ',', 1))
    return false;
  if (! parseReply(F("+HTTPACTION:"), &datalen, ',', 2))
    return false;

  DEBUG_PRINT("HTTP status: "); DEBUG_PRINTLN(status);
  DEBUG_PRINT("Data length: "); DEBUG_PRINTLN(datalen);

  if (status != 200) return false;

  getReply(F("AT+HTTPREAD"));

  // Terminate HTTP service
  sendCheckReply(F("AT+HTTPTERM"), ok_reply, 10000);

  return true;
}

/********************************* HTTPS FUNCTION *********************************/
// boolean SIM7000_3G::postData3G(const char *server, uint16_t port, const char *connType, char *URL) {
boolean SIM7000::postData(const char *server, uint16_t port, const char *connType, const char *URL, const char *body) {
  // Sample request URL: "GET /dweet/for/{deviceID}?temp={temp}&batt={batt} HTTP/1.1\r\nHost: dweet.io\r\n\r\n"

  // Start HTTPS stack
	if (! sendCheckReply(F("AT+CHTTPSSTART"), ok_reply, 10000))
  	return false;

	DEBUG_PRINTLN(F("Waiting 1s to ensure connection..."));
  delay(1000);
  
  // Construct the AT command based on function parameters
  // char auxStr[strlen(URL)+strlen(server)+7];
  char auxStr[200];
  uint8_t connTypeNum = 1;
  
  if (strcmp(connType, "HTTP") == 0) {
  	connTypeNum = 1;
  }
  if (strcmp(connType, "HTTPS") == 0) {
  	connTypeNum = 2;
  }

  sprintf(auxStr, "AT+CHTTPSOPSE=\"%s\",%d,%d", server, port, connTypeNum);

  // Connect to HTTPS server
  // if (! sendCheckReply(F("AT+CHTTPSOPSE=\"www.dweet.io\",443,2"), ok_reply, 10000)) // Use port 443 and HTTPS
  //   return false;
  // if (! sendCheckReply(auxStr, ok_reply, 10000))
  //   return false;


  if (! sendCheckReply(auxStr, ok_reply, 10000))
    return false;
  
  // readline(10000);

  // if (strstr(replybuffer, "+HTTPSOPSE: 0") == 0) {
  //   return false;
  // }

  DEBUG_PRINTLN(F("Waiting 1s to make sure it works..."));
  delay(1000);

  // Send data to server
  sprintf(auxStr, "AT+CHTTPSSEND=%i", strlen(URL) + strlen(body)); // URL and body must include \r\n as needed

  if (! sendCheckReply(auxStr, ">", 10000))
    return false;

  if (! sendCheckReply(URL, ok_reply, 10000))
    return false;
  
  delay(1000);

  // Check server response length
  uint16_t replyLen;
  sendParseReply(F("AT+CHTTPSRECV?"), F("+CHTTPSRECV: LEN,"), &replyLen);

  // Get server response content
  sprintf(auxStr, "AT+CHTTPSRECV=%i", replyLen);
  getReply(auxStr, 2000);

  if (replyLen > 0) {
    readRaw(replyLen);
    flushInput();
    DEBUG_PRINT("\t<--- "); DEBUG_PRINTLN(replybuffer);
  }
  
  // Close HTTP/HTTPS session
  if (! sendCheckReply(F("AT+CHTTPSCLSE"), ok_reply, 10000))
    return false;

  readline(10000);
  DEBUG_PRINT("\t<--- "); DEBUG_PRINTLN(replybuffer);

  // Stop HTTP/HTTPS stack
  if (! sendCheckReply(F("AT+CHTTPSSTOP"), F("+CHTTPSSTOP: 0"), 10000))
    return false;

  readline(); // Eat OK
  DEBUG_PRINT("\t<--- "); DEBUG_PRINTLN(replybuffer);

  return (replyLen > 0);
}

/********* TCP FUNCTIONS  ************************************/


boolean SIM7000::TCPconnect(char *server, uint16_t port) {
  flushInput();

  // close all old connections
  if (! sendCheckReply(F("AT+CIPSHUT"), F("SHUT OK"), 20000) ) return false;

  // single connection at a time
  if (! sendCheckReply(F("AT+CIPMUX=0"), ok_reply) ) return false;

  // manually read data
  if (! sendCheckReply(F("AT+CIPRXGET=1"), ok_reply) ) return false;


  DEBUG_PRINT(F("AT+CIPSTART=\"TCP\",\""));
  DEBUG_PRINT(server);
  DEBUG_PRINT(F("\",\""));
  DEBUG_PRINT(port);
  DEBUG_PRINTLN(F("\""));


  mySerial->print(F("AT+CIPSTART=\"TCP\",\""));
  mySerial->print(server);
  mySerial->print(F("\",\""));
  mySerial->print(port);
  mySerial->println(F("\""));

  if (! expectReply(ok_reply)) return false;
  if (! expectReply(F("CONNECT OK"))) return false;

  // looks like it was a success (?)
  return true;
}

boolean SIM7000::TCPclose(void) {
  return sendCheckReply(F("AT+CIPCLOSE"), F("CLOSE OK"));
}

boolean SIM7000::TCPconnected(void) {
  if (! sendCheckReply(F("AT+CIPSTATUS"), ok_reply, 100) ) return false;
  readline(100);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return (strcmp(replybuffer, "STATE: CONNECT OK") == 0);
}

boolean SIM7000::TCPsend(char *packet, uint8_t len) {

  DEBUG_PRINT(F("AT+CIPSEND="));
  DEBUG_PRINTLN(len);
#ifdef ADAFRUIT_FONA_DEBUG
  for (uint16_t i=0; i<len; i++) {
  DEBUG_PRINT(F(" 0x"));
  DEBUG_PRINT(packet[i], HEX);
  }
#endif
  DEBUG_PRINTLN();


  mySerial->print(F("AT+CIPSEND="));
  mySerial->println(len);
  readline();

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  if (replybuffer[0] != '>') return false;

  mySerial->write(packet, len);
  readline(3000); // wait up to 3 seconds to send the data

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);


  return (strcmp(replybuffer, "SEND OK") == 0);
}

uint16_t SIM7000::TCPavailable(void) {
  uint16_t avail;

  if (! sendParseReply(F("AT+CIPRXGET=4"), F("+CIPRXGET: 4,"), &avail, ',', 0) ) return false;


  DEBUG_PRINT (avail); DEBUG_PRINTLN(F(" bytes available"));


  return avail;
}


uint16_t SIM7000::TCPread(uint8_t *buff, uint8_t len) {
  uint16_t avail;

  mySerial->print(F("AT+CIPRXGET=2,"));
  mySerial->println(len);
  readline();
  if (! parseReply(F("+CIPRXGET: 2,"), &avail, ',', 0)) return false;

  readRaw(avail);

#ifdef ADAFRUIT_FONA_DEBUG
  DEBUG_PRINT (avail); DEBUG_PRINTLN(F(" bytes read"));
  for (uint8_t i=0;i<avail;i++) {
  DEBUG_PRINT(F(" 0x")); DEBUG_PRINT(replybuffer[i], HEX);
  }
  DEBUG_PRINTLN();
#endif

  memcpy(buff, replybuffer, avail);

  return avail;
}



/********* HTTP LOW LEVEL FUNCTIONS  ************************************/

boolean SIM7000::HTTP_init() {
  return sendCheckReply(F("AT+HTTPINIT"), ok_reply);
}

boolean SIM7000::HTTP_term() {
  return sendCheckReply(F("AT+HTTPTERM"), ok_reply);
}

void SIM7000::HTTP_para_start(FONAFlashStringPtr parameter,
                                    boolean quoted) {
  flushInput();


  DEBUG_PRINT(F("\t---> "));
  DEBUG_PRINT(F("AT+HTTPPARA=\""));
  DEBUG_PRINT(parameter);
  DEBUG_PRINTLN('"');


  mySerial->print(F("AT+HTTPPARA=\""));
  mySerial->print(parameter);
  if (quoted)
    mySerial->print(F("\",\""));
  else
    mySerial->print(F("\","));
}

boolean SIM7000::HTTP_para_end(boolean quoted) {
  if (quoted)
    mySerial->println('"');
  else
    mySerial->println();

  return expectReply(ok_reply);
}

boolean SIM7000::HTTP_para(FONAFlashStringPtr parameter,
                                 const char *value) {
  HTTP_para_start(parameter, true);
  mySerial->print(value);
  return HTTP_para_end(true);
}

boolean SIM7000::HTTP_para(FONAFlashStringPtr parameter,
                                 FONAFlashStringPtr value) {
  HTTP_para_start(parameter, true);
  mySerial->print(value);
  return HTTP_para_end(true);
}

boolean SIM7000::HTTP_para(FONAFlashStringPtr parameter,
                                 int32_t value) {
  HTTP_para_start(parameter, false);
  mySerial->print(value);
  return HTTP_para_end(false);
}

boolean SIM7000::HTTP_data(uint32_t size, uint32_t maxTime) {
  flushInput();


  DEBUG_PRINT(F("\t---> "));
  DEBUG_PRINT(F("AT+HTTPDATA="));
  DEBUG_PRINT(size);
  DEBUG_PRINT(',');
  DEBUG_PRINTLN(maxTime);


  mySerial->print(F("AT+HTTPDATA="));
  mySerial->print(size);
  mySerial->print(",");
  mySerial->println(maxTime);

  return expectReply(F("DOWNLOAD"));
}

boolean SIM7000::HTTP_action(uint8_t method, uint16_t *status,
                                   uint16_t *datalen, int32_t timeout) {
  // Send request.
  if (! sendCheckReply(F("AT+HTTPACTION="), method, ok_reply))
    return false;

  // Parse response status and size.
  readline(timeout);
  if (! parseReply(F("+HTTPACTION:"), status, ',', 1))
    return false;
  if (! parseReply(F("+HTTPACTION:"), datalen, ',', 2))
    return false;

  return true;
}

boolean SIM7000::HTTP_readall(uint16_t *datalen) {
  getReply(F("AT+HTTPREAD"));
  if (! parseReply(F("+HTTPREAD:"), datalen, ',', 0))
    return false;

  return true;
}

boolean SIM7000::HTTP_ssl(boolean onoff) {
  return sendCheckReply(F("AT+HTTPSSL="), onoff ? 1 : 0, ok_reply);
}

/********* HTTP HIGH LEVEL FUNCTIONS ***************************/

boolean SIM7000::HTTP_GET_start(char *url,
              uint16_t *status, uint16_t *datalen){
  if (! HTTP_setup(url))
    return false;

  // HTTP GET
  if (! HTTP_action(FONA_HTTP_GET, status, datalen, 30000))
    return false;

  DEBUG_PRINT(F("Status: ")); DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: ")); DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (! HTTP_readall(datalen))
    return false;

  return true;
}


void SIM7000::HTTP_GET_end(void) {
  HTTP_term();
}

boolean SIM7000::HTTP_POST_start(char *url,
              FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen){
  if (! HTTP_setup(url))
    return false;

  if (! HTTP_para(F("CONTENT"), contenttype)) {
    return false;
  }

  // HTTP POST data
  if (! HTTP_data(postdatalen, 10000))
    return false;
  mySerial->write(postdata, postdatalen);
  if (! expectReply(ok_reply))
    return false;

  // HTTP POST
  if (! HTTP_action(FONA_HTTP_POST, status, datalen))
    return false;

  DEBUG_PRINT(F("Status: ")); DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: ")); DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (! HTTP_readall(datalen))
    return false;

  return true;
}

void SIM7000::HTTP_POST_end(void) {
  HTTP_term();
}

void SIM7000::setUserAgent(FONAFlashStringPtr useragent) {
  this->useragent = useragent;
}

void SIM7000::setHTTPSRedirect(boolean onoff) {
  httpsredirect = onoff;
}

/********* HTTP HELPERS ****************************************/

boolean SIM7000::HTTP_setup(char *url) {
  // Handle any pending
  HTTP_term();

  // Initialize and set parameters
  if (! HTTP_init())
    return false;
  if (! HTTP_para(F("CID"), 1))
    return false;
  if (! HTTP_para(F("UA"), useragent))
    return false;
  if (! HTTP_para(F("URL"), url))
    return false;

  // HTTPS redirect
  if (httpsredirect) {
    if (! HTTP_para(F("REDIR"), 1))
      return false;

    if (! HTTP_ssl(true))
      return false;
  }

  return true;
}

/********* HELPERS *********************************************/

boolean SIM7000::expectReply(FONAFlashStringPtr reply,
                                   uint16_t timeout) {
  readline(timeout);

  DEBUG_PRINT(F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}

/********* LOW LEVEL *******************************************/

inline int SIM7000::available(void) {
  return mySerial->available();
}

inline size_t SIM7000::write(uint8_t x) {
  return mySerial->write(x);
}

inline int SIM7000::read(void) {
  return mySerial->read();
}

inline int SIM7000::peek(void) {
  return mySerial->peek();
}

inline void SIM7000::flush() {
  mySerial->flush();
}

void SIM7000::flushInput() {
    // Read all available serial input to flush pending data.
    uint16_t timeoutloop = 0;
    while (timeoutloop++ < 40) {
        while(available()) {
            read();
            timeoutloop = 0;  // If char was received reset the timer
        }
        delay(1);
    }
}

uint16_t SIM7000::readRaw(uint16_t b) {
  uint16_t idx = 0;

  while (b && (idx < sizeof(replybuffer)-1)) {
    if (mySerial->available()) {
      replybuffer[idx] = mySerial->read();
      idx++;
      b--;
    }
  }
  replybuffer[idx] = 0;

  return idx;
}

uint8_t SIM7000::readline(uint16_t timeout, boolean multiline) {
  uint16_t replyidx = 0;

  while (timeout--) {
    if (replyidx >= 254) {
      //DEBUG_PRINTLN(F("SPACE"));
      break;
    }

    while(mySerial->available()) {
      char c =  mySerial->read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;

        if (!multiline) {
          timeout = 0;         // the second 0x0A is the end of the line
          break;
        }
      }
      replybuffer[replyidx] = c;
      //DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
      replyidx++;
    }

    if (timeout == 0) {
      //DEBUG_PRINTLN(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = 0;  // null term
  return replyidx;
}

uint8_t SIM7000::getReply(const char *send, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN(send);


  mySerial->println(send);

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

uint8_t SIM7000::getReply(FONAFlashStringPtr send, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN(send);


  mySerial->println(send);

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

// Send prefix, suffix, and newline. Return response (and also set replybuffer with response).
uint8_t SIM7000::getReply(FONAFlashStringPtr prefix, char *suffix, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINT(prefix); DEBUG_PRINTLN(suffix);


  mySerial->print(prefix);
  mySerial->println(suffix);

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

// Send prefix, suffix, and newline. Return response (and also set replybuffer with response).
uint8_t SIM7000::getReply(FONAFlashStringPtr prefix, int32_t suffix, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINT(prefix); DEBUG_PRINTLN(suffix, DEC);


  mySerial->print(prefix);
  mySerial->println(suffix, DEC);

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

// Send prefix, suffix, suffix2, and newline. Return response (and also set replybuffer with response).
uint8_t SIM7000::getReply(FONAFlashStringPtr prefix, int32_t suffix1, int32_t suffix2, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINT(prefix);
  DEBUG_PRINT(suffix1, DEC); DEBUG_PRINT(','); DEBUG_PRINTLN(suffix2, DEC);


  mySerial->print(prefix);
  mySerial->print(suffix1);
  mySerial->print(',');
  mySerial->println(suffix2, DEC);

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

// Send prefix, ", suffix, ", and newline. Return response (and also set replybuffer with response).
uint8_t SIM7000::getReplyQuoted(FONAFlashStringPtr prefix, FONAFlashStringPtr suffix, uint16_t timeout) {
  flushInput();


  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINT(prefix);
  DEBUG_PRINT('"'); DEBUG_PRINT(suffix); DEBUG_PRINTLN('"');


  mySerial->print(prefix);
  mySerial->print('"');
  mySerial->print(suffix);
  mySerial->println('"');

  uint8_t l = readline(timeout);

  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

boolean SIM7000::sendCheckReply(const char *send, const char *reply, uint16_t timeout) {
  if (! getReply(send, timeout) )
	  return false;
/*
  for (uint8_t i=0; i<strlen(replybuffer); i++) {
  DEBUG_PRINT(replybuffer[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  for (uint8_t i=0; i<strlen(reply); i++) {
    DEBUG_PRINT(reply[i], HEX); DEBUG_PRINT(" ");
  }
  DEBUG_PRINTLN();
  */
  return (strcmp(replybuffer, reply) == 0);
}

boolean SIM7000::sendCheckReply(FONAFlashStringPtr send, FONAFlashStringPtr reply, uint16_t timeout) {
	if (! getReply(send, timeout) )
		return false;

  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}

boolean SIM7000::sendCheckReply(const char* send, FONAFlashStringPtr reply, uint16_t timeout) {
  if (! getReply(send, timeout) )
	  return false;
  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}


// Send prefix, suffix, and newline.  Verify FONA response matches reply parameter.
boolean SIM7000::sendCheckReply(FONAFlashStringPtr prefix, char *suffix, FONAFlashStringPtr reply, uint16_t timeout) {
  getReply(prefix, suffix, timeout);
  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}

// Send prefix, suffix, and newline.  Verify FONA response matches reply parameter.
boolean SIM7000::sendCheckReply(FONAFlashStringPtr prefix, int32_t suffix, FONAFlashStringPtr reply, uint16_t timeout) {
  getReply(prefix, suffix, timeout);
  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}

// Send prefix, suffix, suffix2, and newline.  Verify FONA response matches reply parameter.
boolean SIM7000::sendCheckReply(FONAFlashStringPtr prefix, int32_t suffix1, int32_t suffix2, FONAFlashStringPtr reply, uint16_t timeout) {
  getReply(prefix, suffix1, suffix2, timeout);
  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}

// Send prefix, ", suffix, ", and newline.  Verify FONA response matches reply parameter.
boolean SIM7000::sendCheckReplyQuoted(FONAFlashStringPtr prefix, FONAFlashStringPtr suffix, FONAFlashStringPtr reply, uint16_t timeout) {
  getReplyQuoted(prefix, suffix, timeout);
  return (prog_char_strcmp(replybuffer, (prog_char*)reply) == 0);
}


boolean SIM7000::parseReply(FONAFlashStringPtr toreply,
          uint16_t *v, char divider, uint8_t index) {
  char *p = prog_char_strstr(replybuffer, (prog_char*)toreply);  // get the pointer to the voltage
  if (p == 0) return false;
  p+=prog_char_strlen((prog_char*)toreply);
  //DEBUG_PRINTLN(p);
  for (uint8_t i=0; i<index;i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
    //DEBUG_PRINTLN(p);

  }
  *v = atoi(p);

  return true;
}

boolean SIM7000::parseReply(FONAFlashStringPtr toreply,
          char *v, char divider, uint8_t index) {
  uint8_t i=0;
  char *p = prog_char_strstr(replybuffer, (prog_char*)toreply);
  if (p == 0) return false;
  p+=prog_char_strlen((prog_char*)toreply);

  for (i=0; i<index;i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
  }

  for(i=0; i<strlen(p);i++) {
    if(p[i] == divider)
      break;
    v[i] = p[i];
  }

  v[i] = '\0';

  return true;
}

// Parse a quoted string in the response fields and copy its value (without quotes)
// to the specified character array (v).  Only up to maxlen characters are copied
// into the result buffer, so make sure to pass a large enough buffer to handle the
// response.
boolean SIM7000::parseReplyQuoted(FONAFlashStringPtr toreply,
          char *v, int maxlen, char divider, uint8_t index) {
  uint8_t i=0, j;
  // Verify response starts with toreply.
  char *p = prog_char_strstr(replybuffer, (prog_char*)toreply);
  if (p == 0) return false;
  p+=prog_char_strlen((prog_char*)toreply);

  // Find location of desired response field.
  for (i=0; i<index;i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
  }

  // Copy characters from response field into result string.
  for(i=0, j=0; j<maxlen && i<strlen(p); ++i) {
    // Stop if a divier is found.
    if(p[i] == divider)
      break;
    // Skip any quotation marks.
    else if(p[i] == '"')
      continue;
    v[j++] = p[i];
  }

  // Add a null terminator if result string buffer was not filled.
  if (j < maxlen)
    v[j] = '\0';

  return true;
}

boolean SIM7000::sendParseReply(FONAFlashStringPtr tosend,
				      FONAFlashStringPtr toreply,
				      uint16_t *v, char divider, uint8_t index) {
  getReply(tosend);

  if (! parseReply(toreply, v, divider, index)) return false;

  readline(); // eat 'OK'

  return true;
}

boolean SIM7000::parseReplyFloat(FONAFlashStringPtr toreply,
          float *f, char divider, uint8_t index) {
  char *p = prog_char_strstr(replybuffer, (prog_char*)toreply);  // get the pointer to the voltage
  if (p == 0) return false;
  p+=prog_char_strlen((prog_char*)toreply);
  //DEBUG_PRINTLN(p);
  for (uint8_t i=0; i<index;i++) {
    // increment dividers
    p = strchr(p, divider);
    if (!p) return false;
    p++;
    //DEBUG_PRINTLN(p);

  }
  *f = atof(p);

  return true;
}

// needed for CBC and others

boolean SIM7000::sendParseReplyFloat(FONAFlashStringPtr tosend,
				      FONAFlashStringPtr toreply,
				      float *f, char divider, uint8_t index) {
  getReply(tosend);

  if (! parseReplyFloat(toreply, f, divider, index)) return false;

  readline(); // eat 'OK'

  return true;
}

