#include "Arduino_FreeRTOS.h"
#include <util/delay.h>
#include <stdio.h>
#include <IRremote.h>

#define RECV_PIN 6
#define power 0x46
#define A 0x44
#define B 0x40
#define C 0x43
#define up 0x19
#define right 0x5E
#define down 0x1C
#define left 0xC
#define select 0x18

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.command, HEX); // Print "old" raw data
    IrReceiver.resume(); // Enable receiving of the next value
  }

}
