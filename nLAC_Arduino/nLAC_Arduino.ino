
/*

*/
#include <Ethernet.h>
#include <ArduinoModbus.h>

#define DOUT  3
#define CLK  2

  // defines pins numbers
const int stepPin = 6; 
const int dirPin = 4; 
const int enaPin = 2;
const int LL = 8;
const int UL = 10;
//byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x71, 0x1F};
byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x62, 0x75};

IPAddress ip(10,0,0,4);
float calibration_factor = 8850; //-7050 worked for my 440lb max scale setup
float zero_factor;
EthernetServer ethernet(502);
ModbusTCPServer modbusTCPServer;

long coil_state = 0x0000;
long pos_steps;
bool isEnabled;

union
{
  float asFloat;
  uint16_t asInt[2];
} fpregister;

void motorEnable()
{
  digitalWrite(enaPin,LOW);
  isEnabled = 0x1;
}

void motorDisable()
{
  digitalWrite(enaPin,HIGH);
  isEnabled = 0x0;
}

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  delay(1000);

  ethernet.begin();
  // start the Modbus TCP server
	if (!modbusTCPServer.begin()) 
	{
		Serial.println("Failed to start Modbus TCP Server!");
	}
  modbusTCPServer.configureHoldingRegisters(0x00, 8);//this will hold the position
  modbusTCPServer.configureCoils(0x0a, 1); //motor direction 0=backward, 1=forward
  modbusTCPServer.configureCoils(0x0b, 1); //motor enable
  modbusTCPServer.configureCoils(0x0c, 1); //keep alive
  modbusTCPServer.configureCoils(0x0d, 1); //home status

  modbusTCPServer.coilWrite(0x0d, 0x00);//home status is false to start
  
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enaPin,OUTPUT);
  pinMode(LL,INPUT);
  pinMode(UL,INPUT);
  
  Serial.print("attempting to HOME the stage\n");

  home();
  
  Serial.print("setup complete\n");
}

void stepPosition(short int idir)
{
    short int inc = idir;
    if(!digitalRead(LL))
    {
      digitalWrite(dirPin,HIGH);//away from the motor
      inc = 1;
    }
    if(!digitalRead(UL))
    {
      digitalWrite(dirPin,LOW);//toward from the motor
      inc = -1;
    }
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(100);
    digitalWrite(stepPin,LOW);          
    delayMicroseconds(100);
    pos_steps += inc;
    modbusTCPServer.holdingRegisterWrite(0x00,pos_steps);
}

void seekPosition()
{
  uint8_t homestatus = modbusTCPServer.coilRead(0x0d);
  if( homestatus == 0x00 ) return;
  uint16_t desiredPosition = modbusTCPServer.holdingRegisterRead(0x00);
  motorEnable();
  while( pos_steps != desiredPosition )
  {
    if( pos_steps > desiredPosition )
    {
      digitalWrite(dirPin,LOW);//toward motor
      stepPosition(-1);
    }
    if( pos_steps < desiredPosition )
    {
      digitalWrite(dirPin,HIGH);//away from motor
      stepPosition(1);
    }
  }
  motorDisable();
}

void loop() 
{
  EthernetClient client = ethernet.available();
  //Serial.print(" I am in the loop\n");
  modbusTCPServer.accept(client);
  while( client.connected() )
  {
    Serial.print("a client connected\n");
    modbusTCPServer.poll();
    uint8_t homestatus = modbusTCPServer.coilRead(0x0d);
    Serial.print("Coil was read");
    Serial.print(homestatus);
    Serial.print("\n");
    seekPosition();
  }
}

void home()
{
  digitalWrite(dirPin,LOW);//toward the motor
  motorEnable();
  bool done = 0x0;
  while( !done )
  {
    if(!digitalRead(LL))
    {
      done = 0x1;
      pos_steps = 0;
      modbusTCPServer.holdingRegisterWrite(0x00,0x0000);
      modbusTCPServer.coilWrite(0x0d, 0xff);//home status is now true
      break;
    }
    if(!digitalRead(UL))
    {
      pos_steps = -1;
      modbusTCPServer.coilWrite(0x0d, 0x00);//home status is false
      break;
    }
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(100);
    digitalWrite(stepPin,LOW);          
    delayMicroseconds(100);
  }
  motorDisable();
}
