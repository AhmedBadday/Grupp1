void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
 

}

void loop() {
  for(int i = 1;i <= 255; i++)
  {
    delay(10);
    analogWrite(3, i);
  }

  for(int k = 255; k >= 0; k--)
  {
    delay(10);
    analogWrite(3, k);
  }


}
