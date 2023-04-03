#include "Arduino_FreeRTOS.h"
#include <util/delay.h>

#define led1 3
#define led2 5
void ledTask1(void* pvParam)
{
  for(;;)
  {
    //delay(3000);
    digitalWrite(led1, HIGH);
    _delay_ms(3000);
    digitalWrite(led1, LOW);  
    vTaskDelay(pdMS_TO_TICKS(1)); //fake delay to give the other task the possibility to run
  }
}

void ledTask2(void* pvParam)
{
  for(;;)
  {
    digitalWrite(led2, HIGH);
    _delay_ms(3000);
    digitalWrite(led2, LOW);
    vTaskDelay(pdMS_TO_TICKS(1)); //fake delay to give the other task the possibility to run
  }
}
void loop() {
  // put your main code here, to run repeatedly:

}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  xTaskCreate(ledTask1, "task1",  100, 0, 1, NULL);

  xTaskCreate(ledTask2, "task2",  100, 0, 1, NULL);

}

//förklaring till observerat beteende:
//1. om man ger lampa p3, task1 högre prio kommer bara den köras. Detta är
//eftersom vTaskDelay är så kort så task1 kommer avbryta task2 direkt när den
//körs igen. 
//2. samma sak gäller fast tvärtom för task2


