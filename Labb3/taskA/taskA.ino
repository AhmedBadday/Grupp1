#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>
#include <time.h>
#define arraysize 10


typedef struct monitor
{
  int counter; //counter för att veta hur många element vi har i arrayn och för att fixa till så att de är en queue
  int arr[arraysize];
  SemaphoreHandle_t mutex, untilFullSem, untilEmptySem;    //a mutex and 2 semaphore
  TaskHandle_t prodThrd, consThrd;

  void init()
  {
    untilFullSem = xSemaphoreCreateCounting(arraysize, arraysize); //max 10, 10 left to full
untilEmptySem = xSemaphoreCreateCounting(arraysize, 0); //max 10, 0 left to empty
    mutex = xSemaphoreCreateMutex();

  }

  void add()
  {
    int r = rand() % 99; //random number between 0-100
    if (counter < 10) //not going outside the array
    {
      arr[counter] = r;
      Serial.print(arr[counter]); //prints out what we added to the arr
      Serial.print(" <- lade till");
      Serial.print("\n");
      Serial.print("Nuvarande Array: ");
      for (int j = 0; j <= counter; j++)
    {
      Serial.print(arr[j]); //changed the queue to fifo
      Serial.print(" ");
    }
    Serial.print("\n");



      counter++;
    }
  }

  void read()
  {
    Serial.print(arr[0]); //reads the head, prints it
    Serial.print(" <- tog bort");
    Serial.print("\n");

    counter = counter - 1; // ett element mindre
     Serial.print("Nuvarande Array: ");
    for (int j = 0; j < counter; j++)
    {
      arr[j] = arr[j + 1];
      Serial.print(arr[j]); //changed the queue to fifo
      Serial.print(" ");
    }
    Serial.print("\n");
    arr[counter] = 0; //the last element becomes zero
  }
} monitor;



monitor k;



void producer(void* pvParam)
{
      for(;;)
      {
      if(k.counter <= arraysize) //we only add a number if we are in the range of the size, note that this seems that is needed because we have semaphores, but the problem is 
      //if we get to ten elements and then try to take the semaphore that is "full" it will wait for the program to get a semaphore but the
      // program never realises any so it will be in a deadlock
          {
            vTaskDelay(pdMS_TO_TICKS(6000));
            xSemaphoreTake(k.untilFullSem, portMAX_DELAY); //takes a "semaphore" so in the first run it will be 9 left, thus we can only add 9 more elements
            xSemaphoreTake(k.mutex, portMAX_DELAY);
            k.add();
            xSemaphoreGive(k.mutex);
            xSemaphoreGive(k.untilEmptySem); //gives to untilEmptySem so it will be 1 left to empty. thus we can only remove 1 element in the first run. 

          }
      

    }

  
}
void consumer(void* pvParam)
{
  for(;;)
  {
    if(k.counter >= 0)
    {
        vTaskDelay(pdMS_TO_TICKS(9000)); //wait 9 seconds until we can remove any element. 
        xSemaphoreTake(k.untilEmptySem, portMAX_DELAY);
        xSemaphoreTake(k.mutex, portMAX_DELAY);
        k.read();
        xSemaphoreGive(k.mutex);
        xSemaphoreGive(k.untilFullSem); //gives to untilEmptySem because we removed elements therefore we can add elements
        
        
    }

  }

}
void setup()
{
  srand(time(NULL));
  Serial.begin(9600);
  k.init();
  xTaskCreate(producer, "producer",  128, NULL, 1, NULL);
  xTaskCreate(producer, "producer2",  128, NULL, 1, NULL);
  xTaskCreate(consumer, "consumer",  128, NULL, 0, NULL);
  xTaskCreate(consumer, "consumer2",  128, NULL, 0, NULL);

}

void loop()
{
  
}
