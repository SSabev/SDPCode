 char INBYTE;
 int  LED = 13; // LED on pin 13
 int counter =0;

 void setup() {
   Serial.begin(9600); 
   pinMode(LED, OUTPUT);
 }

 void loop() {
   Serial.println("Press 1 to turn Arduino pin 13 LED ON or 0 to turn it OFF:");
   if (Serial.available()){
     INBYTE = Serial.read();
     if( INBYTE == '0' ) digitalWrite(LED, LOW);
     if( INBYTE == '1' ) digitalWrite(LED, HIGH);
   }
   
   if (counter % 8 == 0){
     Serial.print("Arduino counter: ");
     Serial.println(counter);
     counter = 0;
   }
   counter++;
   
   delay(1000);
 }

