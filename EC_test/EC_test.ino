
bool polarityEC;
uint16_t ECVar;

void setup() {
 
  pinMode(A2,INPUT); //this is for autologging function at SCAN interval! Just place a 100ohm shunt from 5v to A5 and it should enter into this logging


  //Probe stuff
  pinMode(A0,INPUT); //EC PIN is A0
  pinMode(A3,OUTPUT);//EC Power is A3
  pinMode(A1,OUTPUT);//EC Ground is A1
  digitalWrite(A1,LOW);//EC Ground is set to low 
  digitalWrite(A3,LOW);

  Serial.begin(19200);

  polarityEC = true;

}

void loop() {
  
  GetTempANDEC();
  delay(5000);
}

void GetTempANDEC(){
  //EC
  ECVar = 0.0;
  uint16_t readMax = 1012;
  if (polarityEC == true){ //need to do computation with ec var cause its no longer symetric 
  digitalWrite(A1,LOW);
  digitalWrite(A3,HIGH);
  delay(100);//this value may need to be adjusted
  ECVar = analogRead(A0);
  digitalWrite(A1,LOW);
  digitalWrite(A3,LOW);
  
  polarityEC = false;
  }else{
  digitalWrite(A1,HIGH);
  digitalWrite(A3,LOW);
  delay(100);//this value may need to be adjusted
  ECVar = analogRead(A0);
  ECVar = readMax - ECVar;
  digitalWrite(A1,LOW);
  digitalWrite(A3,LOW);
  
  polarityEC = true; 
  }
  Serial.print(F(", Recorded EC = "));
  Serial.println(ECVar);

  
}
