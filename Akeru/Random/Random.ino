#include <SoftwareSerial.h>
#include <Akeru.h>

#define SERIAL_BAUD 9600
#define SIGFOX_LED_PIN 13
#define SIGFOX_POWER 3

#define BOOT_DELAY 500 // ms before initializing modem
#define LOOP_DELAY 600000 //milliseconds between each loop

int value; //2bytes

struct payload {
  short random;
};

payload p;

void setup() {
  Serial.begin (SERIAL_BAUD);
  pinMode(SIGFOX_LED_PIN, OUTPUT);
  
  //let the akeru board wake up gently
  delay(BOOT_DELAY);
  pinMode(A0, INPUT);

  Akeru.begin();
  Akeru.setPower(SIGFOX_POWER);
  
  digitalWrite(SIGFOX_LED_PIN, LOW);
}
void loop() {
  p.random = random(0, 101);
  
  if (!Akeru.isReady()){
      Serial.println("Cannot send Sigfox message right now"); 
      Serial.println("Probably due to the 1 msg per 10' limit enforced by the lib");
  }
  digitalWrite(SIGFOX_LED_PIN, HIGH);
  if (!Akeru.send(&p, sizeof(p))){
    Serial.println("An error occured while sending message");
    digitalWrite(SIGFOX_LED_PIN, LOW);
  }
  uint8_t* bytes = (uint8_t*)&p;
  
  //0-1 == 255 --> (0-1) > len
  for(uint8_t i = sizeof(p)-1; i < sizeof(p); --i) {
    if (bytes[i] < 16) {Serial.print("0");}
    Serial.print(bytes[i], HEX);
  }
  Serial.println("Message sent");
  digitalWrite(SIGFOX_LED_PIN, LOW);
  delay(LOOP_DELAY);
}

