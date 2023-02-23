#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <util/delay.h>

#define ledpin1 3
#define ledpin2 5
#define buttonpin 7  // Interrupt pin
#define queue_size 5

SemaphoreHandle_t mutex; // mutex
volatile float command = 0.0;
volatile float brightness = 0.0;
QueueHandle_t first = xQueueCreate(queue_size, sizeof(float));
QueueHandle_t second = xQueueCreate(queue_size, sizeof(float));
volatile float recived = 0.0; 
volatile float recived2 = 0.0;



void reset_queues_isr() {
  // Call the function to clear the queues
  xQueueReset(first);
  xQueueReset(second);
}

void task1(void *pvParameters) 
{
  while(1)
  {
    if(xQueueReceive(first, &recived, portMAX_DELAY) == pdTRUE) // checks if there is any item in the queue
    {

      xSemaphoreTake(mutex, portMAX_DELAY);
      analogWrite(ledpin1, recived*255); //
      xSemaphoreGive(mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // waits with this task for 5 seconds, therefore we will not take anything from the queue until 5 seconds have gone
  }
}

void task2(void *pvParameters) 
{
  while(1)
  {
     if(xQueueReceive(second, &recived2, portMAX_DELAY) == pdTRUE) // checks from the second queue
    {

      xSemaphoreTake(mutex, portMAX_DELAY);
      analogWrite(ledpin2, recived2*255);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
    
  }
}

void task3(void *pvParameters) 
{
  while(1)
  {
    if(Serial.available() > 0)
    {
      command = Serial.parseFloat(); //reads from serial monitor
      if((int)command == 1)
      {
        xSemaphoreTake(mutex, portMAX_DELAY);
        brightness = command - int(command);
        xQueueSend(first, &brightness, portMAX_DELAY); //send to the first que if it is to the task1 that controlls the first led.
        xSemaphoreGive(mutex);
      }
      if((int)command == 2)
      {
        xSemaphoreTake(mutex, portMAX_DELAY);
        brightness = command - int(command);
        xQueueSend(second, &brightness, portMAX_DELAY);
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
  pinMode(buttonpin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonpin), reset_queues_isr, FALLING); //we say that when the button pin is falling, we call the function  reset queues.
  //digitalPinToInterrupt(buttonpin) converts digtialpin to interupt number
  //interupt has higher prio then any task, so when a interupt happens the isr will be called and stop the whole program until the isr is done, then it will resume were it
  //ended.

  
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(task1, "Task1", 128, NULL, 2, NULL);
  xTaskCreate(task2, "Task2", 128, NULL, 2, NULL);
  xTaskCreate(task3, "Task3", 128, NULL, 1, NULL); //lowest prio
  
}

void loop()  
{
  // The loop function is not used in FreeRTOS and can be left empty.
}