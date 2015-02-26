/****************************************************************
Code to demonstrate how to control the microview to smile and 
frown base on values from a photoresistor SparkFun electronics.

24 Feb 2014 - Pamela - SparkFun Electronics @hello_techie
Jim - SparkFun Electronics - Graphics for the smile and frown code

Code used for the Desktop Buddy Demo for the SparkFun Inventors kit
for MicroView video 
****************************************************************/

#include <MicroView.h>
#include <Servo.h>
 
#define D_WIDTH uView.getLCDWidth()
#define D_HEIGHT uView.getLCDHeight()
 
#define SMILE 0
#define FROWN 1
#define SLEEP 2

int lightFrownThreshold = 0;      //This needs to be changed base on the photoresistor's values
int lightSmileMaxThreshold = 900;  //This needs to be changed base on the photoresistor's values
                                  
int RED = 6;			// declare RED LED pin 6
int GREEN = 5;			// declare GREEN LED pin 5
int BLUE = 3;			// declare BLUE LED pin 3
int buttonPin = A0;     	// push button pin
int buttonState = 0;		// variable to store the pushbutton status
int sensorPin = A2;		// select the input pin for the photo resistor
int sensorValue = 0;	// variable to store the value coming from the sensor
Servo servo;				// declare servo object

//Face Code

byte eyeRadius = D_WIDTH/8;
byte eyeY = D_HEIGHT/6; 
byte eyeLX = D_WIDTH/2 - eyeRadius*2;
byte eyeRX = D_WIDTH/2 + eyeRadius*2;
byte pupilRadius = eyeRadius/4;
 
byte noseLength = D_HEIGHT/4;
byte noseWidth = D_WIDTH/8;
byte noseATX = D_WIDTH/2;
byte noseATY = D_HEIGHT/4;
byte noseABX = noseATX - noseWidth;
byte noseABY = noseATY + noseLength;
byte noseBBX = noseATX;
byte noseBBY = noseABY + 1;
 
byte smileH = D_HEIGHT/6;
byte smileW = D_WIDTH/3;
byte smileX = D_WIDTH/2;
byte smileY = 3 * D_HEIGHT/4;
 
byte frownH = D_HEIGHT/5;
byte frownW = D_WIDTH/3;
byte frownX = D_WIDTH/2;
byte frownY = 3 * D_HEIGHT/4 + frownH/2;
 
byte mouthLX = D_WIDTH/4;
byte mouthLY = 3 * D_HEIGHT / 4;
byte mouthRX = D_WIDTH - mouthLX;
byte mouthRY = mouthLY;

void setup()
{
        pinMode(sensorPin,INPUT);		// set sensor pin as INPUT
	digitalWrite(sensorPin,HIGH);	// set Internal pull-up
	uView.begin();			// start MicroView 
	uView.clear(PAGE);		// clear page
	servo.attach(2);		// servo control pin at D2 	
        pinMode(buzzerPin, OUTPUT);
        pinMode(RED, OUTPUT);							// set RED LED pin as OUTPUT
	pinMode(GREEN, OUTPUT);							// set GREEN LED pin as OUTPUT
	pinMode(BLUE, OUTPUT);
        pinMode(buttonPin, INPUT);      // initialize the pushbutton pin as an input
        digitalWrite(buttonPin,HIGH);   // set Internal pull-up
}


void updateLight()
{

  int sensorValue= analogRead(sensorPin);
  if (sensorValue < lightFrownThreshold)
  {
      drawFace(FROWN, map(sensorValue, 0, lightFrownThreshold, 100, 0), map(sensorValue, 0, lightFrownThreshold, 100, 0));
      	digitalWrite(RED, HIGH);	// set LED pin HIGH voltage, LED will be on	
  }
  else
  {
      drawFace(SMILE, map(sensorValue, 0, lightSmileMaxThreshold, 100, 0), map(sensorValue, 0, lightSmileMaxThreshold, 100, 0));
          	digitalWrite(RED, HIGH);	// set LED pin HIGH voltage, LED will be on
          	
  uView.display();
  }
}
 
 
void loop() {
 buttonState = digitalRead(buttonPin);	// read the state of the pushbutton value

	// check if the pushbutton is pressed.
	// if it is not pressed, the buttonState is HIGH:
	if (buttonState == LOW) {     
	servo.write(70);	//	about 20 degree
	delay(1000);
	servo.write(90);	// about 90 degree
	delay(1000);
        servo.write(120);	// about 160 degree
	delay(1000);
 	} 
	else 
{
updateLight();
delay(50);
}
 }
//void loop() 
//{
 // if 
 // updateLight();
 // delay(50);
//}

 
void drawFace(byte mouth, byte mouthAngle, byte eyeOpenness)
{
  int eyeH = eyeRadius * eyeOpenness / 100;
  int mouthH;
  
  uView.clear(PAGE);
  // Draw eyes
  
  drawUpArc(eyeLX, eyeY, eyeRadius, eyeH);
  drawDownArc(eyeLX, eyeY, eyeRadius, eyeH);
  uView.circleFill(eyeLX, eyeY, min(pupilRadius, eyeH));
  drawUpArc(eyeRX, eyeY, eyeRadius, eyeH);
  drawDownArc(eyeRX, eyeY, eyeRadius, eyeH);
  uView.circleFill(eyeRX, eyeY, min(pupilRadius, eyeH));
  
  // Draw nose
  uView.line(noseATX, noseATY, noseABX, noseABY);
  uView.line(noseABX, noseABY, noseBBX, noseBBY);
  
  // Draw mouth:
  switch(mouth)
  {
  case SMILE:
    mouthH = smileH * mouthAngle / 100;
    drawUpArc(smileX, smileY, smileW, mouthH);
    break;
  case FROWN:
    mouthH = frownH * mouthAngle / 100;
    drawDownArc(frownX, frownY, frownW, mouthH);
    break;
  case SLEEP:
    uView.line(mouthLX, mouthLY, mouthRX, mouthRY);
    break;
  }
}
void drawUpArc(int center_x, int center_y, int xRadius, int yRadius)
{
  int radius = max(xRadius, yRadius);
  float angle_inc = 1.0/radius;
  int xpos, ypos;
  
  for (float angle = 0.0; angle <= PI; angle += angle_inc)
  {
    xpos = center_x + xRadius * cos(angle);
    ypos = center_y + yRadius * sin(angle);
    uView.pixel(xpos, ypos);
  }
}
 
void drawDownArc(int center_x, int center_y, int xRadius, int yRadius)
{
  int radius = max(xRadius, yRadius);
  float angle_inc = 1.0/radius;
  int xpos, ypos;
  
  for (float angle = PI; angle <= 2*PI; angle += angle_inc)
  {
    xpos = center_x + xRadius * cos(angle);
    ypos = center_y + yRadius * sin(angle);
    uView.pixel(xpos, ypos);
  }
}
