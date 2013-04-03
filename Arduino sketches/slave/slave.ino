#include <Wire.h>

#define high_raw 730
#define low_raw 530

// NOTE: swapped these two around, because of the final board's design
const int leftM0 = 10;    // Left motor black wire
const int leftM1 = 11;    // Left motor white wire
const int rightM0 = 6;    // Right motor black wire
const int rightM1 = 5;    // Right motor white wire

const int legoSensorPin0 = 15;
const int legoSensor0 = A1;
const int legoSensorPin1 = 16;
const int legoSensor1 = A2;
const int touchSensors = 9;

int revolutions_a = 0;
int revolutions_b = 0;

unsigned long time = 0;

int desired_speed_a = 0;
int desired_speed_b = 0;
int current_speed_a = 0;
int current_speed_b = 0;

boolean direction_left;
boolean direction_right;

int TIMECHECK = 20;

int THRESHOLD = 10;

int total_raw0 = 0; // Spinner0 sensor sum
boolean spinning0 = false;
int total_raw1 = 0; // Spinner1 sensor sum
boolean spinning1 = false;
int spinner_counter = 0;
boolean touching = false;


void setup()
{
    Wire.begin(4);
    Wire.onReceive(receiveEvent);    // join i2c bus with address #4
    Wire.onRequest(requestEvent);
    Serial.begin(9600);
    Serial.println("Hello!");    // start serial for output
    
    pinMode(12, INPUT); // fixing Iain's circuit's issues

    pinMode(leftM0, OUTPUT);
    pinMode(leftM1, OUTPUT);
    pinMode(rightM0, OUTPUT);
    pinMode(rightM1, OUTPUT);
    attachInterrupt(1, RPMPulse, CHANGE);
    attachInterrupt(0, RPMPulse2, CHANGE);
        
    pinMode(legoSensorPin0, OUTPUT); //rotation sensor0 setup
    digitalWrite(legoSensorPin0, HIGH);
    pinMode(legoSensorPin1, OUTPUT); //rotation sensor1 setup
    digitalWrite(legoSensorPin1, HIGH);
    pinMode(touchSensors, INPUT_PULLUP);
}

void loop()
{
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
    
    revolutions_a = 0;
    revolutions_b = 0;
    time = millis();
  }
}

void RPMPulse()
{
    revolutions_a++;
}

void RPMPulse2()
{
    revolutions_b++;
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
    byte left, right;    
    
    while(Wire.available()) // loop through all but the last
    {
        left = Wire.read(); // receive byte as a character
        right = Wire.read();
    }
    processVals(left, right);
    
}

void requestEvent() {
    if (spinner_counter < 20){
        total_raw0 = total_raw0 + ReadLegoSensor(0);
        total_raw1 = total_raw1 + ReadLegoSensor(1);
        spinner_counter++;
    } else {
        if (total_raw0 > 20 * low_raw && total_raw0 < 20 * high_raw){
            spinning0 = true; 
        } else {
            spinning0 = false;
        } 
        if (total_raw1 > 20 * low_raw && total_raw1 < 20 * high_raw){
            spinning1 = true; 
        } else {
            spinning1 = false;
        }
        total_raw0 = 0;
        total_raw1 = 0;
        spinner_counter = 0;
    }
    
    if (digitalRead(touchSensors) == LOW){
        touching = true;
    } else {
        touching = false;
    }
    
    byte feedback = B0;
    if (!spinning0 || !spinning1) {
         feedback = B1000;
    }
    if (touching) {
         feedback = feedback || B0101;
    }
    
    Wire.write(feedback);
}

int ReadLegoSensor(int sensor){
    int legoSensorPin;
    int legoSensor;
    if (sensor == 0) {
      legoSensorPin = legoSensorPin0;
      legoSensor = legoSensor0
    } else {
      legoSensorPin = legoSensorPin1;
      legoSensor = legoSensor1
    }
    pinMode(legoSensorPin, INPUT_PULLUP); //set pin as input
    delayMicroseconds(20);
    int value=analogRead(legoSensor); //read the input
    pinMode(legoSensorPin, OUTPUT); //set pin as output
    digitalWrite(legoSensorPin, HIGH); //set output 
    return value; //return the raw value
}

void processVals(byte left, byte right){
    direction_left =  bitRead(left, 7);
    bitClear(left, 7);
    current_speed_a = left<<1;
    desired_speed_a = left<<1;
    
    direction_right =  bitRead(right, 7);
    bitClear(right, 7);
    current_speed_b = right<<1;
    desired_speed_b = right<<1;
    
    setSpeeds();
}

void setSpeeds(){
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
        Serial.println("R_a");
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
        Serial.println("R_b");
      } 
    }
    
    
    if (direction_left) {    // Move left
        analogWrite(leftM1, 0);
        analogWrite(leftM0, current_speed_a);
    } else {
        analogWrite(leftM0, 0);
        analogWrite(leftM1, current_speed_a);
    }
    if (direction_right) {    // Move rigth
        analogWrite(rightM0, 0);
        analogWrite(rightM1, current_speed_b);
    } else {
        analogWrite(rightM1, 0);
        analogWrite(rightM0, current_speed_b);
    }
}
