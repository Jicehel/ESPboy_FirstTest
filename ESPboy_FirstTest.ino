#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7735.h> 
#include <Adafruit_NeoPixel.h>
#include "ESPboyLogo.h"


//PINS
#define LEDPIN         D4
#define SOUNDPIN       D3
#define PIN_CS_LCD_MCP23017 8

//RGB LED
//SPI for LCD
#define TFT_RST       -1
#define TFT_DC        D8
#define TFT_CS        -1

//FOR RGB LED
#define NUMPIXELS 1

//FOR OLED
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

Adafruit_MCP23017 mcp;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN);


void setup(){
//  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  Serial.begin(74880);
  pixels.begin();
  delay (150);
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); 
  pixels.show();
  pinMode(SOUNDPIN, OUTPUT);
  tone(SOUNDPIN, 200);
  delay(100);  
  tone(SOUNDPIN, 100);
  delay(100);
  noTone(SOUNDPIN);
  digitalWrite(SOUNDPIN, HIGH);
  u8x8.begin();
  delay (150);
  u8x8.setFont(u8x8_font_chroma48medium8_r);  
  mcp.begin(0); //actially i2c addrees of mcp23017 is 0x20 but library ises assdr = (0x20 || (parameter X of mcp.begin(X)));
  delay (150);
  for (int i=0;i<8;i++){  
     mcp.pinMode(i, INPUT);
     delay(10);
     mcp.pullUp(i, HIGH);
     delay(10);}
  mcp.pinMode(PIN_CS_LCD_MCP23017, OUTPUT);
  mcp.digitalWrite(PIN_CS_LCD_MCP23017, LOW);
  tft.initR(INITR_144GREENTAB);
  delay (200);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  //draw ESPboylogo  
  tft.drawXBitmap(30, 24, ESPboyLogo, 68, 64, ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(0,102);
  tft.print ("      First test");
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
}


void loop(){ 
 int8_t countled;
 int8_t static rnd=0;
 u8x8.setCursor(0,0);
 countled=0;
 
 for (int i=0;i<8;i++){
    if(mcp.digitalRead(i))u8x8.print("-");
    else {
       u8x8.print("*");
       countled++;}
 }

 if (countled){
    if (rnd==0) {
      pixels.setPixelColor(0, pixels.Color(countled*20, 0, 0)); 
      tft.setTextColor(ST77XX_RED); 
      tft.fillRect(0,100,128,28,ST77XX_RED);}
    if (rnd==1) {
      pixels.setPixelColor(0, pixels.Color(0, countled*20, 0)); 
      tft.setTextColor(ST77XX_GREEN); 
      tft.fillRect(0,100,128,28,ST77XX_GREEN);}
    if (rnd==2) {
      pixels.setPixelColor(0, pixels.Color(0, 0, countled*20)); 
      tft.setTextColor(ST77XX_BLUE); 
      tft.fillRect(0,100,128,28,ST77XX_BLUE);}
    tft.fillRect(0,0,128,16,ST77XX_BLACK);
    tft.fillRect(0,64,16,16,ST77XX_BLACK);
    tft.setCursor(0, 50);
    tft.setTextSize(2);
    tft.println("PRESSED");
    tft.fillRect(0,64,16,16,ST77XX_BLACK);
    tft.println(countled);
    tone(SOUNDPIN, countled*150);}
 else{ 
    rnd=random(0,3);
    noTone(SOUNDPIN);
    digitalWrite(SOUNDPIN, HIGH);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); 
    tft.setCursor(0, 0);
    tft.setTextSize(1);
    tft.print("Millis: "); 
    tft.fillRect(40,0,70,8,ST77XX_BLACK);
    tft.fillRect(0,48,120,32,ST77XX_BLACK);
    tft.println(millis());
 }
 pixels.show();
 delay(100);
}
