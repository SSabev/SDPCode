#include <Wire.h>

#define UTILITY_INDEX  4
#define KICKER_MASK    1
#define SPINNER_MASK   2

#define CHARGE_KICKER  1
#define CHARGE_TIME    50
#define DO_KICK        2
#define KICK_TIME      150
#define RETRACT_KICKER 3
#define RETRACT_TIME   100
#define KICKER_IDLE    0


const int frontM0 = 11;    // Left motor black wire
const int frontM1 = 10;    // Left motor white wire
const int backM0 = 5;    // Right motor black wire
const int backM1 = 6;    // Right motor white wire
const int spinner = 9;
const int kicker_a = 12;
const int kicker_b = 13;

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

long kicker_time;
byte kicker_operation = KICKER_IDLE;
byte b_spinners;


void setup(  ) {
    pinMode(frontM0, OUTPUT);
    pinMode(frontM1, OUTPUT);
    pinMode(backM0, OUTPUT);
    pinMode(backM1, OUTPUT);
    pinMode(spinner, OUTPUT);
    pinMode(kicker_a, OUTPUT);
    pinMode(kicker_b, OUTPUT);

    Wire.begin();
    Serial.begin(9600);
    Serial.print(B0);
    time = millis();

    //digitalWrite(spinner, HIGH);
    attachInterrupt(1, RPMPulse, CHANGE);
    attachInterrupt(0, RPMPulse2, CHANGE);
    kicker_operation = KICKER_IDLE;
    b_spinners = 0;
}

void RPMPulse() {
    revolutions_a++;
}

void RPMPulse2() {
    revolutions_b++;
}

void loop() {
    byte ctrlVals[5];    // Array to store control values
    int i = 0;          // Counter for control package validation

    if (Serial.available() > 0){
        delay(5);           // Timeout for serial buffer consistency
        while (i<5){
            if(Serial.available()) {
                ctrlVals[i] = Serial.read();
                i++;
            }
            else{
                delay(1);
            }
        }
        byte feedback;
        
        Wire.beginTransmission(4);
        Wire.write(ctrlVals[0]);
        Wire.write(ctrlVals[1]);
        Wire.endTransmission();
        Wire.requestFrom(4, 1, true);
        if (Wire.available()) {
            feedback = Wire.read();
            Serial.print(feedback);
        }
        processVals(ctrlVals);
        
        b_spinners = (ctrlVals[UTILITY_INDEX] & SPINNER_MASK) >> 1;
    } else {
        ctrlVals[UTILITY_INDEX] = 0;
    }

      
    if (((ctrlVals[UTILITY_INDEX] & KICKER_MASK)!= 0) && (kicker_operation == KICKER_IDLE)) {
          digitalWrite(kicker_a, HIGH);
          digitalWrite(kicker_b, LOW);
  
          kicker_operation = CHARGE_KICKER;
          kicker_time = millis();
    }
    else if ((kicker_operation == CHARGE_KICKER) && (millis() - kicker_time >= CHARGE_TIME)) {
          digitalWrite(kicker_a, LOW);
          digitalWrite(kicker_b, HIGH);
          digitalWrite(spinner, LOW); // disable the spinners
  
          kicker_operation = DO_KICK;
          kicker_time = millis();
    }
    else if ((kicker_operation == DO_KICK) && (millis() - kicker_time >= KICK_TIME)) {
          digitalWrite(kicker_a, HIGH);
          digitalWrite(kicker_b, LOW);
          // enable the spinners if required
          if (ctrlVals[UTILITY_INDEX] & SPINNER_MASK){
              digitalWrite(spinner, HIGH);
          }
  
          kicker_operation = RETRACT_KICKER;
          kicker_time = millis();
    }
    else if ((kicker_operation == RETRACT_KICKER) && (millis() - kicker_time >= RETRACT_TIME)) {
          digitalWrite(kicker_a, LOW);
          digitalWrite(kicker_b, LOW);
  
          kicker_operation = KICKER_IDLE;
    }
    else{
          // for a normal operation: fix the spinner
          if (b_spinners){
              // enable the spinners
              digitalWrite(spinner, HIGH);
          }
          else{
              // disable the spinners
              digitalWrite(spinner, LOW);
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

        if (i == 5) {
            setSpeeds();
        }

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

