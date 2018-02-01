#include <Stepper.h>
const int lampu1 = 13;
const int lampu2 = 12;
const int lampu3 = 11;
const int lampu4 = 10;
const int ldr1 = A0;
const int ldr2 = A1;
const int ldr3 = A2;

const int stepsPerRevolution = 278;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

char data = 0;
void setup()
{
  myStepper.setSpeed(100);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
void loop()
{

//  if (Serial.available() > 0)
//  {
//    data = Serial.read();
//  }
//  switch (data) {
//
//    case '0':
//      Serial.flush();
//      break;
//
//    case '1':
//      digitalWrite(lampu1, HIGH);
//      delay(1);
//      Serial.flush();
//      break;
//      
//    case '2':
//      digitalWrite(lampu1, LOW);
//      delay(1);
//      Serial.flush();
//      break;
//    
//    case '3':
//      digitalWrite(lampu2, HIGH);
//      delay(1);
//      Serial.flush();
//      break;
//
//    case '4':
//      digitalWrite(lampu2, LOW);
//      delay(1);
//      Serial.flush();
//      break;
//    
//    case '5':
//      digitalWrite(lampu3, HIGH);
//      delay(1);
//      Serial.flush();
//      break;
//
//    case '6':
//      digitalWrite(lampu3, LOW);
//      delay(1);
//      Serial.flush();
//      break;
//
//    
//    case '7':
//      digitalWrite(lampu4, HIGH);
//      delay(1);
//      Serial.flush();
//      break;
//
//    case '8':
//      digitalWrite(lampu4, LOW);
//      delay(1);
//      Serial.flush();
//      break;
//      
//    case '9':
//      Serial.println("clockwise");
//      myStepper.step(stepsPerRevolution);
//      Serial.flush();
//
//    case '10':
//      Serial.println("counterclockwise");
//      myStepper.step(-stepsPerRevolution);
//      Serial.flush();
//      break;
//  }
  indikator();
}

void indikator(){
    int sensorldr  = analogRead(ldr1);
    if (sensorldr>=100){
      Serial.println("on1");
    }
    else{
      Serial.println("off1");
    }
}


