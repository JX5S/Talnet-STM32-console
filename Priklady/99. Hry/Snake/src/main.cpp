#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "console.h"
#include "graphics.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite frame_buffer = TFT_eSprite(&tft);

void upbtn();
void downbtn();
void leftbtn();
void rightbtn();
void end();

void setup(void) {
  pinMode(PB0, OUTPUT);
  pinMode(PB3, OUTPUT);
  analogWrite(Console_TFT_BLK, 200);

  pinMode(Console_BTN_Up, INPUT);
  pinMode(Console_BTN_Down, INPUT);
  pinMode(Console_BTN_Left, INPUT);
  pinMode(Console_BTN_Right, INPUT);

  attachInterrupt(Console_BTN_Up, upbtn, FALLING);
  attachInterrupt(Console_BTN_Down, downbtn, FALLING);
  attachInterrupt(Console_BTN_Left, leftbtn, FALLING);
  attachInterrupt(Console_BTN_Right, rightbtn, FALLING);

  randomSeed(analogRead(0)*analogRead(1));
   
  tft.init();

  tft.setRotation(3);

  tft.fillScreen(BLACK);

  frame_buffer.createSprite(160, 128);
  frame_buffer.fillRect(20,20,160,128,TFT_BLUE);
}

uint8_t snak_dir = 1; // snake head direction. Changes in interrupts
uint8_t cur_snak_dir = snak_dir; // copied value to prevent being changed during calculating tick

uint8_t apple_x = 3; // apple coordinates
uint8_t apple_y = 3;
uint8_t was_apple = 0; // if an apple was eaten last tick, show tile of snake that has eaten an apple

uint16_t len = 1; // snake length. 1=just head
uint8_t addlen = 0;

uint8_t isSnak[200] = { 0 }; // array of whether tile contains a snake or not, used for collision and apple spawning

int8_t body_xcord[200] = {5, 0 }; // coordinates of snake body parts
int8_t body_ycord[200] = {5, 0 };
uint8_t body_dirs[200] = { 0 }; // directions of snake body parts
uint8_t body_pic[200] = {1, 0 }; // 0 apple, 1-4 head, 5-8 tail, 9-10 straight, 11-12 straight W/apple, 13-16 bent, 17-20 bent W/apply

static const unsigned short* graphics[21] = {apple, Head0, Head1, Head2, Head3, Tail0, Tail1, Tail2, Tail3, Straight0, Straight1, Straight_apple0, Straight_apple1, Bent01, Bent12, Bent23, Bent03, Bent_apple01, Bent_apple12, Bent_apple23, Bent_apple03};

void loop() {

  // evaluate buttons
  if(digitalRead(Console_BTN_Up)+digitalRead(Console_BTN_Down)+digitalRead(Console_BTN_Left)+digitalRead(Console_BTN_Right) == 3){
    if (!digitalRead(Console_BTN_Up) && cur_snak_dir != 2) snak_dir = 0;
    if (!digitalRead(Console_BTN_Down) && cur_snak_dir != 0) snak_dir = 2;
    if (!digitalRead(Console_BTN_Left) && cur_snak_dir != 1) snak_dir = 3;
    if (!digitalRead(Console_BTN_Right) && cur_snak_dir != 3) snak_dir = 1;
  }
  
  cur_snak_dir = snak_dir; // copy so value doesn't change during code
  
  isSnak[body_xcord[len-1]+20*body_ycord[len-1]] = 0;  // tail of last tick no longer contains snake

  // move snake forward
  for (int i = len-1; i >= 1; i--){
    body_xcord[i] = body_xcord[i-1];
    body_ycord[i] = body_ycord[i-1];
    body_pic[i] = body_pic[i-1];
    body_dirs[i] = body_dirs[i-1];
  }
  
  body_dirs[0] = cur_snak_dir;
  
  // determining which tile the one after the head should be
  if(len>1){
    if(body_dirs[0] == body_dirs[1]){ // if both the first tile and head tile have the same direction, it is straight
      body_pic[1] = 9 + body_dirs[1]%2 + 2*was_apple; // 9 - offset for straight tile
    } else { // bent tile
      switch(((body_dirs[0]+2)%4+1)*(body_dirs[1]+1)){ // needed a comutative operation to check which sides of the bent tile should be connected
        case 12:
          body_pic[1] = 13 + 4*was_apple;
          break;
        case 4:
          body_pic[1] = 14 + 4*was_apple;
          break;
        case 2:
          body_pic[1] = 15 + 4*was_apple;
          break;
        case 6:
          body_pic[1] = 16 + 4*was_apple;
          break;
      }
    }
    body_pic[len-1] = 5 + body_dirs[len-2]; // tail
  }
  switch(cur_snak_dir) // move snake
  {
    case 0:
      body_ycord[0] -= 1;
      if(body_ycord[0]<0) end(); // check for out of bounds
      break;
    case 1:
      body_xcord[0] += 1;
      if(body_xcord[0]>=20) end();
      break;
    case 2:
      body_ycord[0] += 1;
      if(body_ycord[0]>=10) end();
      break;
    case 3:
      body_xcord[0] -= 1;
      if(body_xcord[0]<0) end();
      break;
  }

  if(isSnak[body_xcord[0]+20*body_ycord[0]]){ // check for collision with snake
    end();
  } else {
    isSnak[body_xcord[0]+20*body_ycord[0]] = 1; // mark head position as snake
  }

  was_apple = 0;
  if(body_xcord[0] == apple_x && body_ycord[0] == apple_y){
    addlen = 1;
    was_apple = 1;
    while(isSnak[apple_x+20*apple_y]){
      apple_x = random(20);
      apple_y = random(10);
    }
  }

  //drawing:
  frame_buffer.fillRect(0,0,160,24,0x34AD);
  frame_buffer.fillRect(0,24,160,80, 0x6DE6);
  frame_buffer.fillRect(0,104,160,24,0x34AD);
 
  frame_buffer.setSwapBytes(true); // endianness
  for (int i = 1; i < len; i++){ // for each body part draw corresponding picture. Except head
        frame_buffer.pushImage(body_xcord[i] * 8,body_ycord[i]*8+24,8,8,graphics[body_pic[i]]);
  }

  // draw head
  frame_buffer.pushImage(body_xcord[0]*8, body_ycord[0]*8+24, 8, 8, graphics[1 + (cur_snak_dir+2)%4]);

  // draw apple
  frame_buffer.pushImage(apple_x * 8, apple_y * 8 + 24, 8, 8, apple);

  // actually push out to the screen
  frame_buffer.pushSprite(0,0);

  if(addlen){
    len++;
    addlen = 0;
  }

  delay(200);
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void end(){
  tft.fillRect(0,0,160,128,TFT_RED);
  tft.setTextDatum(CC_DATUM);
  tft.setTextSize(2);
  tft.drawString("Konec",80,50);
  tft.drawString("Delka: " + String(len),80,78);
  while(true);
}

void upbtn(){
  if(cur_snak_dir != 2) snak_dir = 0;
}
void downbtn(){
  if(cur_snak_dir != 0) snak_dir = 2;
}
void leftbtn(){
  if(cur_snak_dir != 1) snak_dir = 3;
}
void rightbtn(){
  if(cur_snak_dir != 3) snak_dir = 1;
}
