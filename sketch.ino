#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Servo.h>


#define DHT22_PIN 2  // DHT22 D2
#define AO_PIN A0  // Rain Sensor AO
#define DO_PIN 7     // Rain sesnor digital pin 7


LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
DHT dht22(DHT22_PIN, DHT22);
Servo servo;  // create servo object to control a servo


void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  servo.attach(9);  // attaches the servo on pin 9 to the servo objectư
  servo.write(0);   // rotate slowly servo to 0 degrees immediately
  dht22.begin();  // initialize dht22 sensor
  pinMode(DO_PIN, INPUT);  // Rain sensor DO at digital pin 2
  Serial.begin(9600);
}

void loop()
{

  lcd.clear();                 // clear display

   // read humidity
  int humi  = dht22.readHumidity();
  // read temperature as Celsius
  int tempC = dht22.readTemperature();
  // read temperature as Fahrenheit
  int tempF = dht22.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT22 sensor!");
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("DHT22 Read Failed!");        // print message at (0, 0) for Humidity
  } else {
    Serial.print("DHT22# Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");

    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("H:");        // print message at (0, 0) for Humidity
    lcd.print(humi);
    lcd.print("%");

    lcd.print(" | "); 

    lcd.print("T: "); /// print temperature
    lcd.print(tempC);
    lcd.print("C");


  }

  int rain_state = digitalRead(DO_PIN);
  int rain_value = readRainSensor();
    Serial.print("Rain Value:");
    Serial.println(rain_value);

    lcd.setCursor(0, 1);         // move cursor to   (0, 1)
    lcd.print("RainVal:");        // print message at (0, 1) for Rain Value
    lcd.print(rain_value);

    lcd.print(" | "); 

  if (rain_state == HIGH){
      Serial.println("The rain is NOT detected");
      lcd.print("LOW"); 
      }
    else{
      Serial.println("The rain is detected");
      lcd.print("HIGH"); 
    }
    
    
    rotateServo();
}


void rotateServo(){
    for (int pos = 0; pos <= 180; pos += 1) {  // rotate slowly from 0 degrees to 180 degrees, one by one degree
    // in steps of 1 degree
    servo.write(pos);  // control servo to go to position in variable 'pos'
    delay(10);         // waits 10ms for the servo to reach the position
  }

  for (int pos = 180; pos >= 0; pos -= 1) {  // rotate from 180 degrees to 0 degrees, one by one degree
    servo.write(pos);                        // control servo to go to position in variable 'pos'
    delay(10);                               // waits 10ms for the servo to reach the position
  }
}


int readRainSensor() {
  int sensorValue = analogRead(AO_PIN);  // Read the analog value from sensor
  int outputValue = map(sensorValue, 0, 1023, 100, 0); // map the 10-bit data to 8-bit data , 0 to 100
  return outputValue;             // Return analog moisture value
}
