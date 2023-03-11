#include <Arduino_FreeRTOS.h>
#include <timers.h>



int arr[6] = {11, 10, 9, 6, 5, 3};
int toggle[6]= {0,0,0,0,0,0};


static int i = 0;
static bool forward = true;
static int k = 0;


void dancing(TimerHandle_t timerHandle)
{
if (i == 6)
{
forward  = false;
i = 5;
}
  if (forward == true)
  { 
  if(toggle[i] == 0)
      {
      digitalWrite(arr[i], HIGH);
      toggle[i] = 1;
      return;
      }
    else
    {
      digitalWrite(arr[i], LOW);
      toggle[i] = 0;
      i++;
      return;
    }

  }
else
{

  if(toggle[i] == 0)
      {
      digitalWrite(arr[i], HIGH);
      toggle[i] = 1;
      return;
      }
    else
    {
      digitalWrite(arr[i], LOW);
      toggle[i] = 0;
      i--;
      
    }
    if (i == -1)
    {
    forward = true;
    i++;
    }

}

  

}

void dancingAnalog(TimerHandle_t timerHandle)
{
  if (i == 6)
  {
  forward  = false;
  i = 5;
  }
  if (forward == true)
  { 
    if(toggle[i] == 0)
    {
        if(k == 255)
        {
          k = 0;
          toggle[i] = 1;
        }
        if(k != 255 && toggle[i] == 0)
        {
        analogWrite(arr[i], k);
        k += 17;
        return;
        }
        else
        {
          digitalWrite(arr[i], LOW);
          toggle[i] = 0;
          i++;
          return;
        }
    }
  }
  else
  {

    if(toggle[i] == 0)
    {
    digitalWrite(arr[i], HIGH);
    toggle[i] = 1;
    return;
    }
    else
    {
      analogWrite(arr[i], LOW);
      toggle[i] = 0;
      i--; 
    }
    if (i == -1)
    {
      forward = true;
      i++;
    }

  }

}


TimerHandle_t dancing1;
TimerHandle_t dancingA;
static bool pattern = false;
void isr()
{
    if(pattern == false)
    {
      xTimerStop(dancingA, portMAX_DELAY);
      xTimerStart(dancing1, 0);
      pattern = true;
      return;
    }
    xTimerStop(dancing1, portMAX_DELAY);
    xTimerStart(dancingA, 0);
    pattern = false;
  
}



void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  dancing1 = xTimerCreate("dancingled", pdMS_TO_TICKS(50), pdTRUE, NULL, dancing );
  dancingA = xTimerCreate("dancingtimer", pdMS_TO_TICKS(20), pdTRUE, NULL, dancingAnalog );
  xTimerStart(dancingA, 0);
  attachInterrupt(digitalPinToInterrupt(2), isr, FALLING);
}


void loop() {

  
  // put your main code here, to run repeatedly:

}
