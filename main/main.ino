#define Hall_Sensor A0

const bool USE_MPH = true; 
const int WHEEL_DIAMETER = 18; 
const int RPM_SAMPLE_PERIOD = 10; 
const int MIN_KPH = 4;
const int MIN_HIEGHT = 14; 
const int MAX_HIEGHT = 50; 

#define echoPin1 2 
#define trigPin1 3

#define echoPin2 4 
#define trigPin2 5 

int motor1pin1 = 6;
int motor1pin2 = 7;

int motor2pin1 = 8;
int motor2pin2 = 9;

long duration1; 
int distance1; 

long duration2; 
int distance2; 

int get_distance1();
int get_distance2();

void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  
  Serial.begin(115200);
}

void loop()
{
  
    static bool firstTime = true; 
    static long thisMagValue = 0;
    static int rpm = 0;
    static long mph = 0;
    static long kph = 0;
    static float magnetCount = 0.00; 
    static unsigned long timeOld = 0; 

    long previousMagValue = thisMagValue; 
    thisMagValue = analogRead(Hall_Sensor);

    if(!firstTime) 
    {
        //delay(500);
        int change = abs(thisMagValue-previousMagValue);

        if (change > 1000)
        {
            ++magnetCount;
        }
        // Every RPM_SAMPLE_PERIOD seconds we reset the magnetData array to 0 
        if(floor((millis()-timeOld)/1000)==RPM_SAMPLE_PERIOD)
        {
            timeOld = millis();
            magnetCount = 0;
        }
        if(abs((((float)(millis()-timeOld)/1000)/60))>0.01)
        {
            rpm = (magnetCount/2) / (((float)(millis()-timeOld)/1000)/60);
        }
        delay(15);
        kph = ((WHEEL_DIAMETER * PI)* rpm) / 1666.67; 
        Serial.println("KPH :");
        Serial.print(kph);

        if(kph<MIN_KPH){
          int dist1 = get_distance1();
          int dist2 = get_distance2();
            while(dist1<MIN_HIEGHT | dist2<MIN_HIEGHT){
              if(dist1<MIN_HIEGHT){
              digitalWrite(motor1pin1, HIGH);
              digitalWrite(motor1pin2, LOW);
              delay(100);
              Serial.println("Distance1");
              Serial.print(dist1);
              dist1 = get_distance1();
              }
              if(distance2<MIN_HIEGHT){
                digitalWrite(motor2pin1, HIGH);
                digitalWrite(motor2pin2, LOW);
                delay(100);
                Serial.println("Distance2");
                Serial.print(dist2);
                dist2 = get_distance2();
              }
            }
        }
        else if(kph>MIN_KPH){
          int dist1 = get_distance1();
          int dist2 = get_distance2();
          if(dist1<MAX_HIEGHT){
            while(dist1<MAX_HIEGHT){
              digitalWrite(motor1pin1, LOW);
              digitalWrite(motor1pin2, HIGH);
              delay(100);
              Serial.println("Distance1");
              Serial.print(dist1);
              dist1 = get_distance1();
            }
          }
          if(distance2<MAX_HIEGHT){
            while(dist2<MAX_HIEGHT){
              digitalWrite(motor2pin1, LOW);
              digitalWrite(motor2pin2, HIGH);
              delay(100);
              Serial.println("Distance2");
              Serial.print(dist2);
              dist2 = get_distance2();
            }
          }
        }
    }

    firstTime = false;
}
int get_distance1(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  return distance1;
}
int get_distance2(){
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  return distance2;
}
