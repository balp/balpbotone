int lightPin = 0;
int ledPin = 3;

void setup()
{
  Serial.begin(9600);
  pinMode( ledPin, OUTPUT);
}

void loop()
{
  int light = analogRead(lightPin);
  int out = 256 - ((light - 100)/2);
  if(out < 0) {
    out = 0;
  }
  if(out > 255) {
    out = 255;
  }
  Serial.print(light); Serial.print(" "); Serial.println(ledPin);
  analogWrite(ledPin, 256 - light/4);
  delay(10);
}
