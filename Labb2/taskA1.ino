#include "Arduino_FreeRTOS.h"
#define ledPin 5

void ledTask(void* pvParam)
{
  for(;;)
  {
    digitalWrite(ledPin, HIGH);
    //vTaskDelay(pdMS_TO_TICKS(3000));
    delay(3000);
    digitalWrite(ledPin, LOW);
    //vTaskDelay(pdMS_TO_TICKS(1000));
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  xTaskCreate(ledTask, "task1",  128, NULL, 0, NULL);

}

void loop() {
  // put your main code here, to run repeatedly:
  // put your main code here, to run repeatedly
}
