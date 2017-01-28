// ABP v0.151 arduino control program
// @date 12/01/16
// @author niko9797
// Lee DHT22 PIN D53
// Lee YL-69/YL-38 PIN A9 & D49
// Lee LDR PIN A8
// While is taking lectures, onboard led turns on
// Connects to pc while usb serial

// DHT Temperature & Humidity Sensor
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <AFMotor.h>


#define DHTPIN            53         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;



// YL-39 + YL-69 humidity sensor
byte humidity_sensor_pin = A9;
byte humidity_sensor_vcc = 49;

//LDR
int LDR_Pin = A8; //analog pin 0

//Motor shield mtoor assignment
AF_DCMotor topfan1(1, MOTOR12_64KHZ);  //Define topfan 64KHZ actualiz.
AF_DCMotor topfan2(2, MOTOR12_64KHZ);  //Define topfan 64KHZ actualiz.
AF_DCMotor ledfan(4, MOTOR34_1KHZ);  //Define topfan 64KHZ actualiz.

//Relays pin definition

#define RELAY1 23
#define RELAY2 25

//Onboard LED
int defaultLed = 13;



void setup() {

  // Setup Serial
  Serial.begin(9600);
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  //Serial.println("------------------------------------");
  //Serial.println("-ABP first test program, 12/01/2017-");
  //Serial.println("          -Made by niko9797-        ");
  //Serial.println("------------------------------------");
  //Serial.println("Temperature");
  //Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  //Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  //Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  //Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  //Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  //Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  //Serial.println("------------------------------------");
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  //Serial.println("------------------------------------");
  //Serial.println("Humidity");
  //Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  //Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  //Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  //Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  //Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  //Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  //Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  delay(1000);
  
  //set fan speed
  ledfan.setSpeed(200);
  topfan1.setSpeed(255);
  topfan2.setSpeed(255);
  
  // Init the onboard LED
  pinMode(defaultLed, OUTPUT);
  
  // Init the humidity sensor board
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);

  //Handshake
  Serial.println("c");
  
  //Init relay pins
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);
  
}




void loop() {
	//while (Serial.available()) {
		int c = Serial.read();

		switch (c)
		{
  			case 48:
				readall();
				break;
			case 49:
				light1ON();
                                break;

			case 50:
				light2ON();
                                break;

			case 51:
				light1OFF;
				break;
			case 52:
				light2OFF();
				break;
                        case 53:
                                fan1ON();
   				break;
                        case 54:
                                fan2ON();
				break;
                        case 55:
                                fan1OFF();
				break;
                        case 56:
                                fan2OFF();
				break;
                        case 57:
                                ledfanON();
				break;
                        case 58:
                                ledfanOFF();
				break;
                }
                delay(1000);
	//}

}

void light1ON() {
   digitalWrite(RELAY1,LOW);
   Serial.println("-Lights1 turned ON");
   
}

void light2ON() {
   digitalWrite(RELAY2,LOW);
   Serial.println("-Lights2 turned ON");
   
}

void light1OFF() {
   digitalWrite(RELAY1,LOW);
   Serial.println("-Lights1 turned OFF");
}


void light2OFF() {
   digitalWrite(RELAY2,LOW);
   Serial.println("-Lights2 turned OFF");
}


void fan1ON() {
        topfan1.setSpeed(255);
	topfan1.run(FORWARD);
        Serial.println("-Fan 1 turned on");
}

void fan2ON() {
        topfan2.setSpeed(255);
	topfan2.run(FORWARD);
        Serial.println("-Fan 2 turned on");
}

void fan1OFF() {
	topfan1.run(RELEASE);
        Serial.println("-Fan 1 turned off");
}

void fan2OFF() {
	topfan2.run(RELEASE);
        Serial.println("-Fan 2 turned off");
}

void ledfanON() {
        ledfan.setSpeed(200);
	ledfan.run(FORWARD);
        Serial.println("-Led fan turned on");
}

void ledfanOFF() {
	ledfan.run(RELEASE);
        Serial.println("-Led fan turned off");
}


void readall() {
  
  Serial.println(read_temp());
  delay(1000);
  
  Serial.println(read_humidity_DHT());
  delay(1000);
  
  Serial.println(read_humidity_soil()); 
  delay(1000);
  
  Serial.println(read_light());
  delay(2000);
}

int read_humidity_soil() {
  // Gets Soil humidity (0-1024) first turns on sensor
  ledOn();
  digitalWrite(humidity_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(humidity_sensor_pin);
  digitalWrite(humidity_sensor_vcc, LOW);
  ledOff();
  return 1023 - value;
}

double read_humidity_DHT() {
  // Get humidity event and print its value.
  ledOn();
  delay(500);
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
	//Serial.println("Error reading humidity!");
	ledOff();
	return 0;
  }
  else {
	ledOff();
	return event.relative_humidity;
  }

}

double read_temp() {
 sensors_event_t event;
  // Get temperature event and print its value.
  ledOn();
  delay(500);
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
	//Serial.println("Error reading temperature!");
	ledOff();
	return 0;
  }
  else {
	ledOff();
	return event.temperature;
  }
}

int read_light() {
  // Reads a LDR connected to analog port with 10K resistor
  ledOn();
  delay(500);
  ledOff();
  delay(500);
  return analogRead(LDR_Pin);
}

void ledOn() {
  digitalWrite(defaultLed, HIGH);
}

void ledOff() {
  digitalWrite(defaultLed, LOW);
}

