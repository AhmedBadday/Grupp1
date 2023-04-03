#include "Arduino_FreeRTOS.h"
#include <util/delay.h>
#include <stdio.h>
#include <timers.h>


#define sensorTrigPin 3
#define sensorEchoPin 2
static volatile double distance;
static volatile double time;

void distancePulse(void *pvParameter)
{
  for(;;)
  {
    digitalWrite(sensorTrigPin, HIGH);
    delayMicroseconds(15);
    
    time =  pulseIn(2, HIGH);
    distance = time * 1 / (1000000) * 340;

    digitalWrite(sensorTrigPin, LOW);
    Serial.print(distance);
    Serial.print("\n");
    }
}

void setup() {
  
    Serial.begin(9600);
    pinMode(sensorTrigPin, OUTPUT);
    pinMode(sensorEchoPin, INPUT);
    /*TimerHandle_t distanceTimer = xTimerCreate("distancePulse", pdMS_TO_TICKS(3000), pdTRUE, NULL, distancePulse);
    xTimerStart(distanceTimer, 0);*/
    xTaskCreate(distancePulse, "distancePulse",  100, 0, 1, NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
