int enablePin = 3;
int in1Pin = 4;
int in2Pin = 2;

int currentSpeed = 125;
int speedChange = 1;
boolean rev = true;

void setup()
{
  Serial.begin(9600);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() 
{
  currentSpeed += speedChange;
  if(currentSpeed > 253) {
    speedChange *= -1;
  }
  if(currentSpeed < 1) {
    speedChange *= -1;
    rev = !rev;
  }
  setMotor(currentSpeed, rev);
  delay(10);
}

void setMotor(int speed, boolean reverse)
{
  Serial.print(speed); Serial.print(" ");
  Serial.print(reverse); Serial.println(" ");
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}
