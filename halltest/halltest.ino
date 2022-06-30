#define Hall_Sensor A0

const bool USE_MPH = true; 
const int WHEEL_DIAMETER = 18; 
const int RPM_SAMPLE_PERIOD = 10; 
const int MIN_KPH = 4;
const int MIN_HIEGHT = 14; 
const int MAX_HIEGHT = 50; 

int read_speed(){
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
        //Serial.println("KPH :");
        //Serial.print(kph);
        }

    firstTime = false;
    return(kph);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


void loop() {
  // put your main code here, to run repeatedly:
  int kph = read_speed();
  Serial.println(kph);
}
