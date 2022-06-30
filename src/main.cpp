// Forked form by @jaminNZx and modifications by @tekk
// Modification by @dedysoerya

// CONNECT THE RS485 MODULE.
// MAX485 module <-> ESP8266
//  - DI -> D10 / GPIO1 / TX
//  - RO -> D9 / GPIO3 / RX
//  - DE and RE are interconnected with a jumper and then connected do eighter pin D1 or D2
//  - VCC to +5V / VIN on ESP8266
//  - GNDs wired together
// -------------------------------------
// You do not need to disconnect the RS485 while uploading code.

// Tested on NodeMCU ESP8266 + MAX485 module
// RJ 45 cable: Green -> A, Blue -> B, Brown -> GND module + GND ESP8266
// MAX485: DE + RE interconnected with a jumper and connected to D1 or D2


#include <Arduino.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define MAX485_DE D1
#define MAX485_RE_NEG D2

#define RXD2 13
#define TXD2 15



#define p_ledtick LED_BUILTIN

int ledState = LOW; 
unsigned long previousMillis = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  
  Serial.begin(defaultBaudRate);
  mySerial.begin(defaultBaudRate, SWSERIAL_8N1, RXD2, TXD2, false);;
  
  // Modbus slave ID 1
  node.begin(1,mySerial);

  // callbacks to toggle DE + RE on MAX485
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  pinMode(p_ledtick, OUTPUT);


  Serial.println("Setup OK!");
  Serial.println("----------------------------");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  const long interval = 5000;
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    if (ledState == LOW) 
    {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(p_ledtick, ledState);
    digitalWrite(redePin, LOW);

    executeCurrentRegistryFunction();
    nextRegistryNumber();
  }
}