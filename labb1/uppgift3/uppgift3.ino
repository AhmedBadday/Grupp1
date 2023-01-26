void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT_PULLUP);

}
int brightness = 0;
void loop() {
    if(digitalRead(2) == 0)
    {
      if(brightness == 255)
        brightness = 0;
      analogWrite(5, brightness);
      brightness += 51;
      
      while(digitalRead(2) == 0)
    {
      delay(1);
    }
    }
   

}
