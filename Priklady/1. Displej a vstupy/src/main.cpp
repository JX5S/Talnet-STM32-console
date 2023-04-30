#include <Arduino.h>
#include "console.h"
#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  // put your setup code here, to run once:
  pinMode(Console_BTN_Up, INPUT);
  pinMode(Console_BTN_Down, INPUT);
  pinMode(Console_BTN_Left, INPUT);
  pinMode(Console_BTN_Right, INPUT);
  pinMode(Console_BTN_LShoulder, INPUT);
  pinMode(Console_BTN_RShoulder, INPUT);
  pinMode(Console_Joy_BTN, INPUT);
  pinMode(Console_Joy_X, INPUT);
  pinMode(Console_Joy_Y, INPUT);
  
  tft.init();

  tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);

  pinMode(Console_TFT_BLK, OUTPUT);
  analogWrite(Console_TFT_BLK, 255); // jas podsvícení
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,0);
  tft.println("X: " + String(analogRead(Console_Joy_X)));
  tft.println("Y: " + String(analogRead(Console_Joy_Y)));
  
  tft.drawRect(109,0,51,51, TFT_PURPLE);
  tft.fillCircle(109 + analogRead(Console_Joy_X)/20, 0 + (1024-analogRead(Console_Joy_Y))/20, 3, TFT_GREEN);

  if(!digitalRead(Console_BTN_Up)){
    tft.println("Nahoru");
  }
  if(!digitalRead(Console_BTN_Down)){
    tft.println("Dolu");
  }
  if(!digitalRead(Console_BTN_Left)){
    tft.println("Doleva");
  }
  if(!digitalRead(Console_BTN_Right)){
    tft.println("Doprava");
  }
  if(!digitalRead(Console_BTN_LShoulder)){
    tft.println("Leve-rameno");
  }
  if(!digitalRead(Console_BTN_RShoulder)){
    tft.println("Prave-rameno");
  }
  if(!digitalRead(Console_Joy_BTN)){
    tft.println("Jostick-tlacitko");
  }
  tft.drawTriangle(100,100,120,110,100,126,TFT_YELLOW);
  tft.fillCircle(150, 110, 5, TFT_RED);
  tft.fillRectHGradient(0,90,90,20,TFT_GOLD, TFT_BLUE);
  delay(100);
}