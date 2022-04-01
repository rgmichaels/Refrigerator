#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

int ThermistorPin = 0;
int Vo;
int LowTemp = 67;
int HighTemp = 70;
float R1 = 100000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float temp;


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); //LED as output
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

float CalculateTemp(){
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 
  return T;
}

void loop() {
  T = CalculateTemp();

if (T > HighTemp) //If (T)emperature is > than the HighTemp setting
{
  while (T > LowTemp) {
      // Turn on light/fan
      digitalWrite(LED_BUILTIN, HIGH); //Turn LED ON

      T = CalculateTemp();     
      
      // Display current temperature on screen
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,0);
      display.println("COOLING!");
      display.print("CURRENT TEMP: ");
      display.print(T);
      display.display();
      delay(5000);
      display.clearDisplay();
}
}

  // Display Temperature
  digitalWrite(LED_BUILTIN, LOW); //Turn LED ON
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Hello Rob!\n");
  display.println("The current temp is: ");
  display.print(T);
  display.println(" degrees F.");
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  delay(5000);
  display.clearDisplay();
  display.display();
}
