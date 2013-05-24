//Dino Tinitigan
//Heart Rate Module


byte ID = 175;
byte header = 170;
byte dataLength = 1;
byte heartRate;

int bPin = 10;
int sPin = 11;
int ledPin = 13;
int sensorPin = 9;
int test;
int priority = 200;
int wait = 5000;
int pulseCount = 0;
int duration;
const int SAMPLE = 10;
unsigned long pulseTimeValues[SAMPLE];
bool timedOut;
//################################################################################################################
//################################################################################################################
void setup() 
{
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.println("Heart Rate Module");
}
//################################################################################################################
//################################################################################################################
void loop()
{
  duration = pulseIn(sensorPin, HIGH, 2000000);
  if((duration > 10000) && (duration < 20000))
  {
    //pulse detected within timeout value
    pulseTimeValues[pulseCount] = millis();
    pulseCount++;
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
  }
  else
  {
    //no/missed pulse -> reset calculations
    pulseCount = 0;
    Serial.println("*");
  }
  if(pulseCount == SAMPLE)
  {
    heartRate = calculateHR();
    if(heartRate)
    {
      Serial.print("Heart rate: ");
      Serial.println((int)heartRate);
    }
    else
    {
      Serial.println("Invalid HR");
    }
  }
}
//################################################################################################################
//################################################################################################################
int calculateHR()
{
  pulseCount = 0;
  int sum = 0;
  double varSum = 0;
  for(int i = 0; i < (SAMPLE - 1); i++)
  {
    sum += pulseTimeValues[i+1] - pulseTimeValues[i];
  }
  double mean = sum/(SAMPLE - 1);
  for(int i = 0; i < (SAMPLE - 1); i++)
  {
    varSum += (mean - (pulseTimeValues[i+1] - pulseTimeValues[i])*(mean - (pulseTimeValues[i+1] - pulseTimeValues[i])));
  }
  double stdDev = (varSum/(SAMPLE - 2))/1000.0;
  int HR = 60.0/(mean/1000.0);
  if(stdDev <= 10)
  { 
    return(HR);
  }
  else
  {
    return 0;
  }
}
//################################################################################################################
//################################################################################################################
