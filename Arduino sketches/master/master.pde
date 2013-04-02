#include <Wire.h>

int frontM0 = 11;    // Left motor black wire
int frontM1 = 10;    // Left motor white wire
int backM0 = 5;    // Right motor black wire
int backM1 = 6;    // Right motor white wire
int spinner = 9;
int kicker_a = 12;
int kicker_b = 13;

int revolutions_a = 0;
int revolutions_b = 0;

unsigned long time = 0;

int desired_speed_a = 0;
int desired_speed_b = 0;
int current_speed_a = 0;
int current_speed_b = 0;

boolean direction_front;
boolean direction_back;

int TIMECHECK = 20;

int THRESHOLD = 10;

void setup() {
  Wire.begin();
  Serial.begin(9600); 
  Serial.println("Hello!");
  time = millis();
  pinMode(frontM0, OUTPUT);
  pinMode(frontM1, OUTPUT);
  pinMode(backM0, OUTPUT);
  pinMode(backM1, OUTPUT);
  pinMode(spinner, OUTPUT);
  pinMode(kicker_a, OUTPUT);
  pinMode(kicker_b, OUTPUT);
  digitalWrite(spinner, HIGH);
  attachInterrupt(1, RPMPulse, CHANGE);
  attachInterrupt(0, RPMPulse2, CHANGE);
}

void RPMPulse() {
    revolutions_a++;
}

void RPMPulse2() {
    revolutions_b++;
}

void kick() {
  digitalWrite(kicker_a, HIGH);
  digitalWrite(kicker_b, LOW);
  delay(50);
  
  digitalWrite(kicker_a, LOW);
  digitalWrite(kicker_b, HIGH);
  delay(150);
  
  digitalWrite(kicker_a, HIGH);
  digitalWrite(kicker_b, LOW);
  delay(100);
  
  digitalWrite(kicker_a, LOW);
  digitalWrite(kicker_b, LOW);
  
}

void loop() {
  if (Serial.available()){
    byte ctrlVals[5];    // Array to store control values
    int i = 0;          // Counter for control package validation
    delay(5);           // Timeout for serial buffer consistency
    while (Serial.available() && i<5){
      ctrlVals[i] = Serial.read();
      i++;
    }
    if (ctrlVals[4] != 0) {
      Serial.println("KICK!");
      kick();
    }
    if (i==5) {         // Control package is fine
      for (int i=0; i<5; i++){
        Serial.print(bitRead(ctrlVals[i], 7));
        Serial.print(" - ");
        // bitClear(ctrlVals[i], 7);
        Serial.print(ctrlVals[i]);
        Serial.print("; ");
      }
      Serial.println();
      processVals(ctrlVals);
      Wire.beginTransmission(4);
      Wire.write(ctrlVals[0]);
      Wire.write(ctrlVals[1]);
      Wire.endTransmission();
      Serial.println("Tuk!");
    } 
    else if (i>5){
      Serial.println("Control signal larger than 5 bytes");
    } 
    else {
      Serial.println("Control signal less than 5 bytes");
    }
  }
  if (millis() - time > TIMECHECK){
    int temp_speed_a = revolutions_a * 250/TIMECHECK; // calculate Degrees per Second and divide by 4, to equate to desired_speed
    int temp_speed_b = revolutions_b * 250/TIMECHECK;
    
    if (temp_speed_a > desired_speed_a){
      current_speed_a--;
    } else if (temp_speed_a < desired_speed_a) {
      current_speed_a++; 
    }
    if (temp_speed_b > desired_speed_b){
      current_speed_b--;
    } else if (temp_speed_b < desired_speed_b) {
      current_speed_b++;
    }
    
    setSpeeds();
    
    
    //Serial.print("t: ");
    //Serial.print(temp_speed_a);
//    Serial.print(" c: ");
//    Serial.print(current_speed_a);
//    Serial.print(" d: ");
//    Serial.println(desired_speed_a);
//    
    //Serial.print("t: ");
//    Serial.print(temp_speed_b);
    //Serial.print(" c: ");
//    Serial.print(current_speed_b);
//    Serial.print(" d: ");
//    Serial.println(desired_speed_b);
//    
//    Serial.println();
   
    revolutions_a = 0;
    revolutions_b = 0;
    time = millis();
  }
}

void processVals(byte ctrlVals[5]){
  direction_front = bitRead(ctrlVals[3], 7);
  bitClear(ctrlVals[3], 7);
  current_speed_a = ctrlVals[3]<<1;
  desired_speed_a = ctrlVals[3]<<1;

  direction_back = bitRead(ctrlVals[2], 7);
  bitClear(ctrlVals[2], 7);
  current_speed_b = ctrlVals[2]<<1;
  desired_speed_b = ctrlVals[2]<<1;

  setSpeeds();
}

void setSpeeds (){
      if (desired_speed_a == 0){
      current_speed_a = 0;
    } else {
      int delta_a;
      if ( desired_speed_a > current_speed_a){
        delta_a = desired_speed_a - current_speed_a;
      } else {
        delta_a = current_speed_a - desired_speed_a;
      }
      if (delta_a > THRESHOLD) {
        current_speed_a = desired_speed_a;
        //Serial.println("R_a");
      } 
    }
    
    if (desired_speed_b == 0){
      current_speed_b = 0;
    } else {
      int delta_b;
      if ( desired_speed_b > current_speed_b){
        delta_b = desired_speed_b - current_speed_b;
      } else {
        delta_b = current_speed_b - desired_speed_b;
      }  
      if (delta_b > THRESHOLD) {
        current_speed_b = desired_speed_b;
        //Serial.println("R_b");
      } 
    }
    
  if (direction_front) {    // Move forward
    analogWrite(frontM1, 0);
    analogWrite(frontM0, current_speed_a);
  } 
  else {
    analogWrite(frontM0, 0);
    analogWrite(frontM1, current_speed_a);
  }
  if (direction_back) {    // Move forward
    analogWrite(backM0, 0);
    analogWrite(backM1, current_speed_b);
  } 
  else {
    analogWrite(backM1, 0);
    analogWrite(backM0, current_speed_b);
  } 
}
