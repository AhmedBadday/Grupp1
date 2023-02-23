#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define ledpin1 3
#define ledpin2 5

SemaphoreHandle_t mutex; // mutex
volatile float command = -1;
volatile bool new_command = false;
volatile float brightness = 0.0;

void task1(void *pvParameters) 
{
  while(1)
  {
    if(new_command == true && (int)command == 1)
    {
      xSemaphoreTake(mutex, portMAX_DELAY);
      brightness = command - int(command);
      new_command = false;
      command = -1;
      analogWrite(ledpin1, brightness*255);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task2(void *pvParameters) 
{
  while(1)
  {
    if(new_command == true && (int)command == 2)
    {
      xSemaphoreTake(mutex, portMAX_DELAY);
      brightness = command - int(command);
      new_command = false;
      command = -1;
      analogWrite(ledpin2, brightness*255);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task3(void *pvParameters) 
{
  while(1)
  {
    if(Serial.available() > 0)
    {
      command = Serial.parseFloat();
      if((int)command == 1 || (int)command == 2)
      {
        xSemaphoreTake(mutex, portMAX_DELAY);
        new_command = true;
        xSemaphoreGive(mutex);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(task1, "Task1", 128, NULL, 2, NULL);
  xTaskCreate(task2, "Task2", 128, NULL, 2, NULL);
  xTaskCreate(task3, "Task3", 128, NULL, 1, NULL);
}

void loop() {
  // The loop function is not used in FreeRTOS and can be left empty.
}
Footer
© 2023 GitHub, Inc.
Footer navigation
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
Grupp-/lab2_del_B.ino at main · AhmedBadday/Grupp-