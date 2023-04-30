/*
 * Pong
 * Original Code from https://github.com/rparrett/pongclock
 *
 */

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "console.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

int16_t h = 128;
int16_t w = 160;
int16_t pad_width = 35;
int16_t pad_height = 6;
int pad_x = 50;

int16_t ball_r = 2;
float ball_x = w/2-ball_r;
float ball_y = 20.0;
//float ball_dx = 2.5;
//float ball_dy = 2.5;
float ball_dx = 2.5;
float ball_dy = 2.5;

int skore = 0;
int highscore = 0;

TFT_eSprite frame_buffer = TFT_eSprite(&tft);

void paddle();
void ball();

void setup(void) {
  pinMode(PB0, OUTPUT);
  pinMode(PB3, OUTPUT);
  digitalWrite(PB3, HIGH);
  randomSeed(analogRead(0)*analogRead(1));
   
  tft.init();

  tft.setRotation(3);

  tft.fillScreen(BLACK);
  
  frame_buffer.createSprite(w, h);

  frame_buffer.setTextColor(WHITE, BLACK);
  frame_buffer.setTextDatum(MC_DATUM);

  for (int i = 0; i <= 60; i++){
    frame_buffer.fillRect(0,0,160,128,BLACK);
    frame_buffer.drawString("Pong", w/2, h/2);
    paddle();
    frame_buffer.fillCircle(int(ball_x), int(ball_y), ball_r, TFT_YELLOW);
    frame_buffer.pushSprite(0,0);
    delay(33);
  }
  
}

void loop() {
  frame_buffer.fillRect(0,0,160,128,BLACK);
  paddle();
  ball();
  frame_buffer.setTextDatum(TR_DATUM);
  frame_buffer.drawString(String(skore) + "/" + String(highscore), 160, 0);
  frame_buffer.pushSprite(0,0);
//  delay(33);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void paddle() {
  pad_x = analogRead(Console_Joy_X);
  pad_x = map(pad_x, 280, 750, 0, w-pad_width);
  frame_buffer.fillRect(pad_x, h-pad_height, pad_width, pad_height, WHITE);
}

void ball() {
  ball_x += ball_dx;
  ball_y += ball_dy;
  if(ball_x > w-ball_r || ball_x < ball_r){
    ball_dx *= -1;
    ball_x += ball_dx;
  }
  if(ball_y<ball_r){
    ball_dy *= -1;
    ball_y += ball_dy;
  }
  if(ball_y>h-ball_r-pad_height){
    if(abs(ball_x-(pad_x+pad_width/2)) <= pad_width/2+2){
      ball_dy *= -1;
      ball_y += ball_dy;
      skore += 1;
      if (skore > highscore){
        highscore = skore;
      }
    } else {
      tone(Console_Buzzer, 500, 500);
      skore = 0;
      ball_x = w/2;
      ball_y = 20;
    }
  }
  frame_buffer.fillCircle(int(ball_x), int(ball_y), ball_r, TFT_YELLOW);
}