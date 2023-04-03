#include "Arduino_FreeRTOS.h"
#include <util/delay.h>
#include <stdio.h>
#include <IRremote.h>
#include <semphr.h>

//IR-remote
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

//Motor
#define motorPin1 9
#define dirPin11 5
#define dirPin12 11
#define motorPin2 10
#define dirPin21 13
#define dirPin22 12

//Sensor
#define sensorTrigPin 4
#define sensorEchoPin 2

//Speed settings
#define Aspeed 77
#define Bspeed 153
#define Cspeed 255
static volatile int speed;

QueueHandle_t irQueue; //queue for commands from the remote

//Used for the sensor
static volatile double distance;
static volatile double distance1;
static volatile double time;
static volatile uint16_t commandOut;
static volatile uint16_t commandIn;
static volatile bool lock;
 

void sensor(void* pvParameter)
{
  for(;;)
  { 
    //Send pulse and read from Echo pin
    digitalWrite(sensorTrigPin, HIGH); 
    delayMicroseconds(10);
    
    time =  pulseIn(sensorEchoPin, HIGH);
    distance = time * 0.034 / 2; //in cm
    digitalWrite(sensorTrigPin, LOW);
    
    if(distance < 20) //check if close to object, if so lock the car from moving forward
    {
      lock = true;
    }
    else
    {
      lock = false;
    }
    vTaskDelay(pdMS_TO_TICKS(15)); //allow other tasks to run
  }
}

void remote(void* pvParameter)
{
  for(;;)
  {
    if(IrReceiver.decode()) //if any button is pressed
    {
      commandOut = IrReceiver.decodedIRData.command; //get the command
      //change speed if needed, else send it to the queue used later
      if(commandOut == A)
        speed = Aspeed;
      if(commandOut == B)
        speed = Bspeed;
      if(commandOut == C)
        speed = Cspeed;
      else
      {
        xQueueSend(irQueue, &commandOut, 10);
      }
      IrReceiver.resume(); // Enable receiving of the next value
    }
    vTaskDelay(pdMS_TO_TICKS(17)); //allow other tasks to run
  }
}

void reset() //function to reset all motor pins, setting them to low 
{
  analogWrite(motorPin1, 0);
  digitalWrite(dirPin11, LOW);
  digitalWrite(dirPin12, LOW);
  analogWrite(motorPin2, 0);
  digitalWrite(dirPin21, LOW);
  digitalWrite(dirPin22, LOW);
}

void motor(void* pvParameter)
{
  for(;;)
  {
    xQueueReceive(irQueue, &commandIn, 10); //get command from queue which comes from IR-remote
    if(commandIn == up && lock == false) //if up (forward) and we're not locked
    {

      analogWrite(motorPin1, speed);
      digitalWrite(dirPin11, LOW);
      digitalWrite(dirPin12, HIGH);
      analogWrite(motorPin2, speed);
      digitalWrite(dirPin21, LOW);
      digitalWrite(dirPin22, HIGH);
      delay(100);
      commandIn = 0;
      reset();
    }
    else if(commandIn == down) //if backward
    {
      analogWrite(motorPin1, speed);
      digitalWrite(dirPin11, HIGH);
      digitalWrite(dirPin12, LOW);
      analogWrite(motorPin2, speed);
      digitalWrite(dirPin21, HIGH);
      digitalWrite(dirPin22, LOW);
      delay(100);             //delay so the motor has some time to run, without this we don't notice the motor running since all pins get set to LOW
                              //immediately after this line
      commandIn = 0;          //reset the variable commandIn so that it doesn't set off an if even though we haven't sent anything new to the queue
      reset();                //use of function above, reset all pins
    }
    else if(commandIn == right) 
    {
      analogWrite(motorPin1, speed);
      digitalWrite(dirPin11, LOW);
      digitalWrite(dirPin12, HIGH);
      delay(100);
      commandIn = 0;
      reset();
    }
    else if(commandIn == left)
    {
      analogWrite(motorPin2, speed);
      digitalWrite(dirPin21, LOW);
      digitalWrite(dirPin22, HIGH);
      delay(100);
      commandIn = 0;
      reset();
    }
    vTaskDelay(pdMS_TO_TICKS(17));
  }

}


void setup() {
  Serial.begin(9600);

  irQueue = xQueueCreate(5, sizeof(int)); //create the queue
  speed = 255;                            //set base speed to max

  //Begin receiver and set modes of all pins
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(dirPin11, OUTPUT);
  pinMode(dirPin12, OUTPUT);
  pinMode(dirPin21, OUTPUT);
  pinMode(dirPin22, OUTPUT);

  pinMode(sensorTrigPin, OUTPUT);
  pinMode(sensorEchoPin, INPUT);

  //create the three tasks and start the scheduler
  xTaskCreate(sensor, "sensor", 64, NULL, 6, NULL);
   
  xTaskCreate(remote, "remote", 128, NULL, 5, NULL); 

  xTaskCreate(motor, "motor", 64, NULL, 5, NULL);
  vTaskStartScheduler();
}

void loop() {
}
