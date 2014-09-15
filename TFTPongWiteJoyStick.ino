/*

 TFT Pong
 
 This example for the Arduino screen reads the values 
 of 2 potentiometers to move a rectangular platform
 on the x and y axes. The platform can intersect 
 with a ball causing it to bounce. 
 
 This example code is in the public domain.
 
 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald
 
 http://arduino.cc/en/Tutorial/TFTPong
 
 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  
#define speakerPin 3
int duration = 0;
int frequency = 0;
// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1 
#define motorPin 5

TFT TFTscreen = TFT(cs, dc, rst);
int paddleColor[3];
int paddleSizeX = 40;
int paddleSizeY = 5;

int ballColor[3] = {255,255,255};
int ballSizeX = 5;
int ballSizeY = 5;

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;

int ballSpeed = 5; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;
char ascii[4];

void setup() {

  // initialize the display
  TFTscreen.begin();
  // black background
  TFTscreen.background(0,0,0); 

}

void loop() {

  // save the width and height of the screen
  int myWidth = TFTscreen.width();
  int myHeight = TFTscreen.height();
  
  // map the paddle's location to the position of the potentiometers   
  paddleX = map(analogRead(A0), 0, 1023, 0, myWidth) - paddleSizeX/2; 
  paddleY = map(analogRead(A1), 1023, 0, 0, myHeight) - paddleSizeY/2; 
  
  // set the fill color to black and erase the previous 
  // position of the paddle if different from present
  TFTscreen.fill(0,0,0);

 if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    TFTscreen.rect(oldPaddleX, oldPaddleY, paddleSizeX, paddleSizeY);
    showMouse(paddleX, paddleY,oldPaddleX, oldPaddleY);
  }
  
  // draw the paddle on screen, save the current position
  // as the previous.
  TFTscreen.fill(255,255,255);

  TFTscreen.rect(paddleX, paddleY, paddleSizeX, paddleSizeY);
  oldPaddleX = paddleX;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2) {
  moveBall();
  }
}
void showMouse(int x, int y, int oldx, int oldy) {

  //TFTscreen.fill(0, 0, 0);
  //TFTscreen.rect(0,110, 60,40);
  //TFTscreen.text("X:", 0,110); TFTscreen.text(itoa(x, ascii,10),20, 110); 
  //TFTscreen.text("Y:", 0,120); TFTscreen.text(itoa(y, ascii,10),20, 120); 
  //delay(10);
  // erase the text previous position
  TFTscreen.text("X:", 0,110); TFTscreen.text(itoa(oldx, ascii,10),20, 110); 
  TFTscreen.text("Y:", 0,120); TFTscreen.text(itoa(oldy, ascii,10),20, 120);
  // draw the ball's current position
  TFTscreen.stroke(0, 255, 0);
  TFTscreen.text("X:", 0,110); TFTscreen.text(itoa(x, ascii,10),20, 110); 
  TFTscreen.text("Y:", 0,120); TFTscreen.text(itoa(y, ascii,10),20, 120);
  TFTscreen.stroke(0, 0, 0);
}


// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
 if (ballX > TFTscreen.width() || ballX < 0) {
   ballDirectionX = -ballDirectionX;
 }
 
  if (ballY > TFTscreen.height() || ballY < 0) {
   ballDirectionY = -ballDirectionY;
 }  
 
  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY, paddleSizeX, paddleSizeY)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
    
    //Beep...
    frequency = map(analogRead(A1), 0, 1023, 100, 5000); 
    duration = map(analogRead(A2), 0, 1023, 100, 1000); 
    tone(speakerPin, frequency, duration);
    //delay(duration);
    //Motor Run...
    analogWrite(motorPin, 100);
    
    delay(100);
    analogWrite(motorPin, 0);
 }

 // update the ball's position
 ballX += ballDirectionX;
 ballY += ballDirectionY;
 
 // erase the ball's previous position
 TFTscreen.fill(0,0,0);
 
  if (oldBallX != ballX || oldBallY != ballY) {
    TFTscreen.rect(oldBallX, oldBallY, ballSizeX, ballSizeY);
  }
  
  
  // draw the ball's current position
  TFTscreen.fill(255,255,255);
  TFTscreen.rect(ballX, ballY, ballSizeX, ballSizeY);
 
  oldBallX = ballX;
  oldBallY = ballY;
  
}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;
   
  if ((x >= rectX && x <= (rectX + rectWidth)) && 
      (y >= rectY && y <= (rectY + rectHeight))) {
       result = true; 
      }
 
return result;  
}


