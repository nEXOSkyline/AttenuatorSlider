
/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX7                                                                               11
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/
#include <Ethernet.h>
#include <ArduinoModbus.h>

#define DOUT  3
#define CLK  2


byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x62, 0x75};
IPAddress ip(10,0,0,4);
float calibration_factor = 8850; //-7050 worked for my 440lb max scale setup
float zero_factor;
EthernetServer ethernet(502);
ModbusTCPServer modbusTCPServer;

long coil_state = 0x0000;

union
{
  float asFloat;
  uint16_t asInt[2];
} fpregister;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");



  Ethernet.begin(mac, ip);

  //if( Ethernet.begin(mac) == 0 )
  //{
  
    //Serial.println("Failed to use DHCP");
    //Ethernet.begin(mac,ip);
  //}
  delay(1000);

  ethernet.begin();
  // start the Modbus TCP server
	if (!modbusTCPServer.begin()) 
	{
		Serial.println("Failed to start Modbus TCP Server!");
	}
  modbusTCPServer.configureHoldingRegisters(0x00, 8);//this will hold the mass reading
  //modbusTCPServer.configureHoldingRegisters(0x02, 2);//this will hold the calibration
  //modbusTCPServer.configureHoldingRegisters(0x04, 2);//this will hold the zero

  modbusTCPServer.configureCoils(0x0a, 1); //Force this coil to tare
  
 
  //client.connect("134.79.120.210",5020); 
}


void loop() {
  
  EthernetClient client = ethernet.available();
  //Serial.print(" I am in the loop\n");

  modbusTCPServer.accept(client);
  while( client.connected() )
  {
    Serial.print("a client connected\n");
    modbusTCPServer.poll();


  
  long c = modbusTCPServer.coilRead(0x0a);
  Serial.print("Coil was read");
  Serial.print(c);
  Serial.print("\n");
  
   
    }
    
  }
