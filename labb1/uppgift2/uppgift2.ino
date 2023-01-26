void setup() {
  
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

}
int j = 0;
int prev_out = 1;
int cur_out;

void loop() {
  if(digitalRead(2) == 0 && digitalRead(5) == LOW)
  {
    digitalWrite(5, HIGH);
    while(digitalRead(2) == 0)
    {
      delay(1);
    }
    
  }
  else if (digitalRead(5) == HIGH && digitalRead(2) == 0)
  {
    digitalWrite(5, LOW);
     while(digitalRead(2) == 0)
    {
      delay(1);
    }

  }
  delay(100);

  


 
    
}
