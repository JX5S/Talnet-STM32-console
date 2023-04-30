#include <Arduino.h>
#include "console.h"
#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer = TFT_eSprite(&tft);

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

  frame_buffer.createSprite(160, 128);
  frame_buffer.fillRect(0,0,160,128,TFT_BLACK);
  frame_buffer.pushSprite(0,0);

  pinMode(Console_TFT_BLK, OUTPUT);
  analogWrite(Console_TFT_BLK, 255); // jas podsvícení 0-255
}

void loop() {
  // put your main code here, to run repeatedly:
  frame_buffer.fillRect(0,0,160,128,TFT_BLACK);
  frame_buffer.setCursor(0,0);
  frame_buffer.println("X: " + String(analogRead(Console_Joy_X)));
  frame_buffer.println("Y: " + String(analogRead(Console_Joy_Y)));
  
  frame_buffer.drawRect(109,0,51,51, TFT_PURPLE);
  frame_buffer.fillCircle(109 + analogRead(Console_Joy_X)/20, 0 + (1024-analogRead(Console_Joy_Y))/20, 3, TFT_GREEN);

  if(!digitalRead(Console_BTN_Up)){
    frame_buffer.println("Nahoru");
  }
  if(!digitalRead(Console_BTN_Down)){
    frame_buffer.println("Dolu");
  }
  if(!digitalRead(Console_BTN_Left)){
    frame_buffer.println("Doleva");
  }
  if(!digitalRead(Console_BTN_Right)){
    frame_buffer.println("Doprava");
  }
  if(!digitalRead(Console_BTN_LShoulder)){
    frame_buffer.println("Leve-rameno");
  }
  if(!digitalRead(Console_BTN_RShoulder)){
    frame_buffer.println("Prave-rameno");
  }
  if(!digitalRead(Console_Joy_BTN)){
    frame_buffer.println("Jostick-tlacitko");
  }
  frame_buffer.drawTriangle(100,100,120,110,100,126,TFT_YELLOW);
  frame_buffer.fillCircle(150, 110, 5, TFT_RED);
  frame_buffer.fillRectHGradient(0,90,90,20,TFT_GOLD, TFT_BLUE);

  frame_buffer.pushSprite(0,0);

  delay(10);
}