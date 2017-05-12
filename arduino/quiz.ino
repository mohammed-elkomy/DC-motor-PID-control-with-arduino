#include <SimpleTimer.h>
// the timer object
SimpleTimer timer;

volatile long  pos=0;
int AA =2,BB= 3;
int resistor_refPin,PWM_pin =9,Dir_Pin1= 7,Dir_Pin2= 8 ;
int resistor_refVal=0,out=0,sign;

unsigned long newtime,oldtime = 0;

double PErr=0,PPErr=0,POut=0,PPOut=0,Err,measure;

int dir= 0;
void callback(){
  if(digitalRead(BB) == HIGH)
    dir =-1,pos++;
  else 
    dir =1,pos--; 

   if (pos == 2 || pos == -2) //12000 = 1000 * 12 = 1000 * (24 / 2) = time of 2 pulses 
  {
    pos = 0;
    newtime = millis();
    measure = dir*4920.0/(newtime-oldtime);
    oldtime = newtime;
  }
}



void speedMeasure()
{
      newtime = millis();
      measure = 2500.0*pos/(newtime-oldtime);  // 60 * 1000  * pos / (deltaTime * 24)
      oldtime = newtime; 
      pos = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(Dir_Pin1,OUTPUT);
  pinMode(Dir_Pin2,OUTPUT);
 
  pinMode(AA,INPUT_PULLUP);
  pinMode(BB,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(AA), callback, RISING);
  
   //timer.setInterval(65, speedMeasure);
}

int control_action(){    
   
   Err =  resistor_refVal - 2.1794 * measure; //speed mapping and then compute error 
   out = Err * .1559 -.3116 *PErr+.1557 * PPErr +1.996*POut - .9962*PPOut;
   PPOut = POut,POut=out; 
   out = (out > 255? 255:out);
   out = (out < -255? -255:out);

   Serial.println (resistor_refVal);
   Serial.println (2.1794 * measure);
   Serial.println(out);
   Serial.println("\n");
   return (int)out;
 }
void loop() {
   //timer.run();
   
   resistor_refVal = analogRead(resistor_refPin); // 0 => 1023 map => -511 to 512
   sign = resistor_refVal > 511 ? 1 : -1;
   resistor_refVal =

    //resistor_refVal =  (((resistor_refVal & 511) >> 1) + (sign == -1 ? -255 : 0)); //resistor mapping 
    resistor_refVal =  (sign==1?-1:0)+(resistor_refVal >>1) - 255; //another resistor mapping 


   //or use a controller
    out=control_action();
   
    //just map resistor value 
    out = resistor_refVal ;

 
    
    if(sign == 1 )
      { 
        analogWrite(PWM_pin,out);//+++++++++  0 255
        digitalWrite(Dir_Pin1,HIGH);
        digitalWrite(Dir_Pin2,LOW);
       
      }
      else 
      {
        analogWrite(PWM_pin,-out);//---------- 0 255
        digitalWrite(Dir_Pin1,LOW );
        digitalWrite(Dir_Pin2,HIGH );
      }

    //Serial.print("converted read :");
    //Serial.println (out);

    //Serial.print ("speed =========== ");
    //Serial.print (measure);
    //Serial.println ("<<<<<<<");
}

