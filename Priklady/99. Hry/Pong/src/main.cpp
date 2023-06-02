#include <TFT_eSPI.h>
#include <SPI.h>
#include "console.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite frame_buffer = TFT_eSprite(&tft);

// velikost displeje
int16_t h = 128;
int16_t w = 160;

int16_t pad_width = 35;
int16_t pad_height = 6;
int pad_x = w/2;

int16_t ball_r = 2;
float ball_x = w/2-ball_r;
float ball_y = 20.0;
float ball_dx = 2.5;
float ball_dy = 2.5;

int skore = 0;
int highscore = 0;

// Takto se řekne kompilátoru, že tyto příkazy existují, dole jsou pak nadefinovány
void paddle();
void ball();

void setup(void) {
  // podsvícení
  pinMode(PB3, OUTPUT);
  digitalWrite(PB3, HIGH);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  frame_buffer.createSprite(w, h);
  frame_buffer.setTextColor(TFT_WHITE, TFT_BLACK);
  frame_buffer.setTextDatum(MC_DATUM); // "datum" = kde je nulový bod textu; 'MC_DATUM' = middle center

  // titulní obrazovka, na přibližně dvě sekundy
  for (int i = 0; i <= 60; i++){ // toto se stane 60x. Viz Arduino Reference
    frame_buffer.fillRect(0,0,160,128,TFT_BLACK);
    frame_buffer.drawString("Pong", w/2, h/2);
    paddle();
    frame_buffer.fillCircle(int(ball_x), int(ball_y), ball_r, TFT_YELLOW);
    frame_buffer.pushSprite(0,0);
    delay(33);
  }
}

void loop() {
  frame_buffer.fillRect(0,0,160,128,TFT_BLACK);
  paddle();
  ball();
  frame_buffer.setTextDatum(TR_DATUM);
  frame_buffer.drawString(String(skore) + "/" + String(highscore), 160, 0);
  frame_buffer.pushSprite(0,0);

  // delay(33);
  // delay není potřeba - ono to běží svou rychlostí, pushSprite trvá relativně dlouho
}

void paddle() {
  pad_x = analogRead(Console_Joy_X);
  pad_x = map(pad_x, 280, 750, 0, w-pad_width);
  frame_buffer.fillRect(pad_x, h-pad_height, pad_width, pad_height, TFT_WHITE);
}

void ball() {
  ball_x += ball_dx;
  ball_y += ball_dy;
  
  if(ball_x > w - ball_r || ball_x < ball_r){
    ball_dx *= -1;
    ball_x += ball_dx;
  }
  if(ball_y<ball_r){
    ball_dy *= -1;
    ball_y += ball_dy;
  }
  
  if(ball_y > h - ball_r - pad_height){
    if(ball_x >= pad_x - 1 && ball_x <= pad_x + pad_width + 1){
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
