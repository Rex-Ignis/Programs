
// Ultrasonic library definitions
#include <NewPing.h>
#define SONAR_NUM 3 // Number of sensors.
NewPing sonar[SONAR_NUM] = { // Sensor object array.
  NewPing(6, 7, 400), //Left sensor
  NewPing(8, 9, 400), //Right sensor
  NewPing(10, 11, 400), // Front sensor: trigger pin, echo pin, max distance to ping
};

unsigned long randNumber; //random number generator from noise on pin 0

// Motor pinout definitions
unsigned char PWM_2 = 2, DIR_2 = 3, PWM_1 = 4, DIR_1 = 5, pwm_value = 6;
unsigned int a = 7;

// setup stuff here---------------------------------------------------------------------------------
void setup() {
   Serial.begin(115200); //start serial output at baudrate
   
   pinMode(PWM_2,OUTPUT) ; //Logic pins are set as output
   pinMode(DIR_2,OUTPUT) ;
   pinMode(PWM_1,OUTPUT) ;
   pinMode(DIR_1,OUTPUT) ;
}

//Loops Stuff Here ---------------------------------------------------------------------------------
void loop() {
 
  for (uint8_t i = 0; i < SONAR_NUM; i++) {

    serialP(i);
    
    if((sonar[i].ping_cm() >= 30) && (a == 0) ) // Beginning Distance Measurement
    {   //if distance > __cm or distance < __cm, then go forward
      
      forwardG();
      forwardC();
      delay(10);
      a = 1;
    }
  
    else if((sonar[i].ping_cm() >= 30) && (a != 0) ) // Main Distance Measurement
    {   //if distance > __cm or distance < __cm, then go forward
      
      forwardC();
      delay(20);
    }
  
   else if((sonar[2].ping_cm() < 30) && (sonar[2].ping_cm() != 0)) //Front Distance Measurement
   {  //if distance <= __cm and distance >= __cm, then stop
    
      stopG();
    
      stopS();
     
      backwardG();
    
      backwardC();
  
      stopGB();
  
      stopS();
      
       if (sonar[0].ping_cm() == sonar[1].ping_cm()){ //start of clockwise OR counter-clockwise turn
    
        randNumber = random(1, 2);
        
          if (randNumber == 1){
            CWM();
          }
          else if (randNumber == 2){
           CCWM();
          }
        }
      
       else if (sonar[1].ping_cm() < sonar[0].ping_cm()){ 
        CWM();
        }
      
        else if (sonar[0].ping_cm() < sonar[1].ping_cm()){
        CCWM();
        }
      
      a = 0;
   }
  
  //  else if((sonar[2].ping_cm() > 300)) //Edge Detection by Front Sensor (WIP)
  // { 
  //    
  //    stopG();
  //  
  //    stopS();
  //
  //    CWM();
  //    delay(850);
  //    a = 0;
  //  }
   
   else if((sonar[0].ping_cm() <= 30) && (sonar[0].ping_cm() != 0)) //Left Sensor Distance Measurement
   { //if distance <= __cm and distance >= __cm, then turn clockwise
      
      stopG();
    
      stopS();
  
      CCWM();
      a = 0;
    }
    
   else if((sonar[1].ping_cm() <= 30) && (sonar[1].ping_cm() != 0)) //Right Sensor Distance Measurement
   { //if distance <= __cm and distance >= __cm, then turn counter-clockwise
      
      stopG();
    
      stopS();
    
      CWM();
      a = 0;
      }
      
      else if(sonar[i].ping_cm() == 0 ) // check for noise
      {  
        forwardC();
        delay(5);
      
        i = i - 1; //rerun last sensor
      }
    } // end sonar loop
    
 Serial.println();
 
}//end loop

//Functions ----------------------------------------------------------------------------------------------------
void serialP(int i){
    //serial print of i sensor
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");
}


void forwardC(){
    //constant forward motion
    digitalWrite(DIR_1,HIGH) ;
    digitalWrite(DIR_2,HIGH) ;
    analogWrite(PWM_1, 240);
    analogWrite(PWM_2, 255);
}

void forwardG(){
    // gradual Forward motion
    digitalWrite(DIR_1,HIGH) ;
    digitalWrite(DIR_2,HIGH) ;
      for(pwm_value = 0; pwm_value < 255; pwm_value++){
        analogWrite(PWM_1, pwm_value);                 //increase speed every __ sec
        analogWrite(PWM_2, pwm_value);
        delay(2);
      }
}

void stopG(){
    //gradual stop motion
    digitalWrite(DIR_1,HIGH) ;
    digitalWrite(DIR_2,HIGH) ;
      for(pwm_value = 0; pwm_value < 255; pwm_value++){
        analogWrite(PWM_1, 255 - pwm_value);           //decrease speed every __ sec
        analogWrite(PWM_2, 255 - pwm_value);
        delay(2);
      }
}

void stopGB(){
    //gradual stop motion
    digitalWrite(DIR_1,LOW) ;
    digitalWrite(DIR_2,LOW) ;
      for(pwm_value = 0; pwm_value < 255; pwm_value++){
        analogWrite(PWM_1, 255 - pwm_value);           //decrease speed every __ sec
        analogWrite(PWM_2, 255 - pwm_value);
        delay(2);
      }
}

void backwardG(){
    // gradual Backward motion
    digitalWrite(DIR_1,LOW) ;
    digitalWrite(DIR_2,LOW) ;
      for(pwm_value = 0; pwm_value < 255; pwm_value++){
        analogWrite(PWM_1, pwm_value);                 //increase speed every __ sec
        analogWrite(PWM_2, pwm_value);
        delay(10);
        }
}

void backwardC(){
    //Continue backward at max speed
    analogWrite(PWM_1, 255);
    analogWrite(PWM_2, 240);
    delay(40);
}

void CCWM(){
    // Counter-Clockwise Turn motion
    digitalWrite(PWM_2,255/2) ;
    digitalWrite(DIR_2,LOW) ;
    digitalWrite(PWM_1,255/2) ;
    digitalWrite(DIR_1,HIGH) ;
    delay(350); //end loop delay for robustness
}

void CWM(){
    // Clockwise Turn motion
    digitalWrite(PWM_2,255/2) ;
    digitalWrite(DIR_2,HIGH) ;
    digitalWrite(PWM_1,255/2) ;
    digitalWrite(DIR_1,LOW) ;
    delay(350); //end loop delay for robustness
}

void stopS(){
    // Stay Stopped
    analogWrite(PWM_1, 0);
    analogWrite(PWM_2, 0);
    delay(40);
}
