#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht.h>
#define dataPin 13 // Defines pin number to which the sensor is connected
dht DHT; // Creates a DHT object 
Adafruit_SSD1306 display(1);
void setup()   
{       
    // initialize with the I2C addr 0x3C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    } 
 void loop()
 {
   int readData = DHT.read11(dataPin); // DHT11 or int readData = DHT.read22(dataPin); // DHT22/AM2302
    float t = DHT.temperature; // Gets the values of the temperature
    float h = DHT.humidity; // Gets the values of the humidity
    // Printing the results on the serial monitor
    
    // Clear the buffer.
    display.clearDisplay();
    // Display Text
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(t);
    display.println(" C");
    display.print(h);
    display.println(" %");
    display.display();
    
    display.clearDisplay();}
