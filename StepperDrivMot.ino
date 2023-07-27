  // defines pins numbers
const int stepPin = 6; 
const int dirPin = 4; 
const int enaPin = 2;

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enaPin,OUTPUT);
  digitalWrite(enaPin,LOW);
  delay(10);
  digitalWrite(enaPin,HIGH);
  delay(10);
  Serial.print("setup complete\n");

}

void loop() 
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  delay(1);
  Serial.print("beginning test\n");
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 5000; x++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(100); 
    digitalWrite(stepPin,LOW);          
    delayMicroseconds(100); 

  }
  Serial.print("switching direction\n");
 
  delay(1000); // One second delay
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  delay(1);
  for(int x = 0; x < 3000; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(100);
  }
  delay(1000);
 
}
