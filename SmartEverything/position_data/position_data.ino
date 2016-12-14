#include <SME_basic.h>
#include <Wire.h>
#include <Arduino.h>
#include <sl868a.h>
#include <HTS221.h>
#include <LSM9DS1.h>
long long int next_millis = 0;
long long int last = 0;

struct data {
  char id;
  bool buttonPressed;
  short x;
  short y;
  short z;
};

struct data2 {
  char id;
  short temp;
  float latitude;
  float longitude;
};

  data frame;
  data2 frame2;

// the setup function runs once when you press reset or power the board
void setup() {

 // SerialUSB.begin(115200);
  smeHumidity.begin();
  smeGps.begin();
  smeAccelerometer.begin();
  
  //SerialUSB.println("Initialize SIGFOX");
 // while (!initSigfox()) {
  //  SerialUSB.println("Could not initialize, retrying...");
 // }
  SigFox.begin(19200);
  //while (!SerialUSB);
  sigfoxCommandMode();
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  frame.id = '1';
  frame2.id ='2';
  frame.buttonPressed = false;
  
}

// the loop function runs over and over again forever
void loop() {
  
  frame2.temp = smeHumidity.readTemperature();
  frame.x = smeAccelerometer.readX();
  frame.y = smeAccelerometer.readY();
  frame.z = smeAccelerometer.readZ();
  
  if (smeGps.ready()) {
    frame2.latitude   = smeGps.getLatitude();
    frame2.longitude  = smeGps.getLongitude();
  }
  else {
    frame2.latitude = 2.178047;
    frame2.longitude = 41.411451;
  }
  
  if (digitalRead(13) == HIGH) {
    frame.buttonPressed = true;
    last = millis();
  }
  if (millis() - last >= 60000*10 || frame.buttonPressed == true) {
    bool answer = sendSigfox(&frame, sizeof(data));
    /*SerialUSB.print("  X = ");
    SerialUSB.print(frame.x, DEC);
    SerialUSB.print("     Y = ");
    SerialUSB.print(frame.y, DEC);
    SerialUSB.print("     Z = ");
    SerialUSB.println(frame.z, DEC);
    delay(2000);*/
    answer = sendSigfox(&frame2, sizeof(data2));
    /*SerialUSB.print("temp ");
    SerialUSB.println(frame2.temp);
    SerialUSB.print("Latitude ");
    SerialUSB.print(frame2.latitude, DEC);
    SerialUSB.print("Longitude ");
    SerialUSB.print(frame2.longitude, DEC);*/
    last = millis();
    frame.buttonPressed = false;
  }
}

bool initSigfox() {
  SigFox.begin(19200);
  SigFox.print("+++");
  //SerialUSB.print("SigFox setup: ");
  while (!SigFox.available()) {
    delay(1000);
  }
  String status = "";
  while (SigFox.available()) {
    status += (char)SigFox.read();
  }
  //SerialUSB.println(status);
  status.toUpperCase();
  return status.indexOf("OK") >= 0;
}

void sigfoxCommandMode() {
  SigFox.print("+++");

  // Waiting for modem answer
  while (!SigFox.available()) {
    delay(100);
  }
  while (SigFox.available()) {
    char answer = (char)SigFox.read  ();
     // SerialUSB.print(answer);
  }

  //  SerialUSB.println("");
  delay(10000);
}

boolean canSend(long prev){
  if (prev==0){
    return true;
  }
  unsigned long diff = millis() - prev;
  return diff >= 600000;
}

bool sendSigfox(const void* data, uint8_t len) {
  uint8_t* bytes = (uint8_t*)data;

    //SerialUSB.println("Issuing AT command");
  SigFox.print('A');
  SigFox.print('T');
  SigFox.print('$');
  SigFox.print('S');
  SigFox.print('F');
  SigFox.print('=');
  
  SerialUSB.print('A');
  SerialUSB.print('T');
  SerialUSB.print('$');
  SerialUSB.print('S');
  SerialUSB.print('F');
  SerialUSB.print('=');
  
  //0-1 == 255 --> (0-1) > len
  for(uint8_t i = len-1; i < len; --i) {
    if (bytes[i] < 16) {
      SigFox.print("0");
      SerialUSB.print("0");
    }
    SigFox.print(bytes[i], HEX);
    SerialUSB.print(bytes[i], HEX);
  }
  SigFox.print('\r');

  SerialUSB.print('\r');
  SerialUSB.println("");


  bool error = false;
  // Waiting for modem answer
  while (!SigFox.available()) {
    delay(100);
  }
  bool firstChar = true;
  while (SigFox.available()) {
    char answer = (char)SigFox.read();
      //SerialUSB.print(answer);
    if (firstChar) {
      firstChar = false;
      if (answer == 'O') { // "OK" message
        error = false; 
      } else { // "ERROR" message 
        error = true;
      }
    }
  }
  return !error;
}
