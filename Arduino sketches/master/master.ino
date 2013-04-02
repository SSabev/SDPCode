#include <Wire.h>

#define KICKER_INDEX   4
#define KICKER_MASK    1

#define CHARGE_KICKER  1
#define CHARGE_TIME    50
#define DO_KICK        2
#define KICK_TIME      150
#define RETRACT_KICKER 3
#define RETRACT_TIME   100
#define KICKER_IDLE    0

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

long kicker_time;
byte kicker_operation = KICKER_IDLE;


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
    Serial.println("Hello!");
    time = millis();

    digitalWrite(spinner, HIGH);
    attachInterrupt(1, RPMPulse, CHANGE);
    attachInterrupt(0, RPMPulse2, CHANGE);
    kicker_operation = KICKER_IDLE;
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
    delay(5);           // Timeout for serial buffer consistency
    if (Serial.available() > 0){
        Serial.println("Data avail");
        while (i<5){
            ctrlVals[i] = Serial.read();
            Serial.println(ctrlVals[i], HEX);
            i++;
        }
        processVals(ctrlVals);
        Wire.beginTransmission(4);
        Wire.write(ctrlVals[0]);
        Wire.write(ctrlVals[1]);
        Wire.endTransmission();
    }
    else{
        ctrlVals[KICKER_INDEX] = 0;
    }

    
    if (((ctrlVals[KICKER_INDEX] & KICKER_MASK)!= 0) && (kicker_operation == KICKER_IDLE)) {
        Serial.println("Phase 1");
        digitalWrite(kicker_a, HIGH);
        digitalWrite(kicker_b, LOW);

        kicker_operation = CHARGE_KICKER;
        kicker_time = millis();
        Serial.println(kicker_time);

    }
    else if ((kicker_operation == CHARGE_KICKER) && (millis() - kicker_time >= CHARGE_TIME)) {
        Serial.println("Phase 2");
        digitalWrite(kicker_a, LOW);
        digitalWrite(kicker_b, HIGH);

        kicker_operation = DO_KICK;
        kicker_time = millis();
        Serial.println(kicker_time);
    }
    else if ((kicker_operation == DO_KICK) && (millis() - kicker_time >= KICK_TIME)) {
        Serial.println("Phase 3");
        digitalWrite(kicker_a, HIGH);
        digitalWrite(kicker_b, LOW);

        kicker_operation = RETRACT_KICKER;
        kicker_time = millis();
        Serial.println(kicker_time);
    }
    else if ((kicker_operation == RETRACT_KICKER) && (millis() - kicker_time >= RETRACT_TIME)) {
        Serial.println("Final Phase");
        digitalWrite(kicker_a, LOW);
        digitalWrite(kicker_b, LOW);

        kicker_operation = KICKER_IDLE;
        Serial.println(millis());
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

