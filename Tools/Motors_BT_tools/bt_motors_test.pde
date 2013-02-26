char MOTOR_LEFT;
char MOTOR_RIGHT;
char MOTOR_FRONT;
char MOTOR_REAR;
char KINCKER;

 int  LED = 13; // LED on pin 13
 int  PWM1 = 9;
 
 int dataIn = 0;

 void setup() {
   Serial.begin(9600); 
   pinMode(LED, OUTPUT);
   pinMode(PWM1, OUTPUT);
 }

 void loop() {
   if (Serial.available()){
     // there has to be 5 bytes
     dataIn = Serial.available();
     if (dataIn != 5){
        Serial.println("Received less than 5 bytes: ");
        Serial.println(dataIn);
     }
     else {
        MOTOR_LEFT = Serial.read();
        MOTOR_RIGHT = Serial.read();
        MOTOR_FRONT = Serial.read();
        MOTOR_REAR = Serial.read();
        KINCKER = Serial.read();
        
        Serial.print("Kicker: ");
        Serial.print(KINCKER, DEC);
        Serial.print(" Front: ");
        Serial.print(MOTOR_FRONT, DEC);
        Serial.print(" Rear: ");
        Serial.print(MOTOR_REAR, DEC);
        Serial.print(" Right: ");
        Serial.print(MOTOR_RIGHT, DEC);
        Serial.print(" Left: ");
        Serial.println(MOTOR_LEFT, DEC);
     }
   }
   analogWrite(PWM1, MOTOR_FRONT);
   
   delay(50);
 }

