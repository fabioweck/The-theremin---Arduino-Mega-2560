/*** 

Fabio Augusto Weck - #441977 - f.weck977@mybvc.ca 
TECH 1102 - Internet of Things - 22SEPMNTR3
Project - Make Something Cool.

-------------------------------------------------

REFERENCES: 

https://create.arduino.cc/projecthub/pollux-labs/arduino-theremin-with-a-minor-pentatonic-b56de4

https://www.youtube.com/watch?v=XIGS-wa4nHE

https://www.theengineeringprojects.com/2015/02/interfacing-multiple-ultrasonic-sensor-arduino.html

https://www.programmingelectronics.com/how-to-use-an-external-reset-button-with-arduino/

-------------------------------------------------


***/


/*******************
*** DC MOTOR Pins***
********************/

#define ENABLE 4
#define DIRA 2

/*****************************
*** Ultrasonic Sensors Pins***
******************************/

#define trigger1 6
#define echo1 7
#define trigger2 8
#define echo2 9

int potentiometer = A2;

/***************
*** LEDs Pins***
****************/

const int rgbBlue = 10;
const int rgbGreen = 11;
const int rgbRed = 12; 

/*****************
*** Speaker Pin***
******************/

const int speaker = 5;

/******************************
*** Calibration - Variables ***
*******************************/

int distance = 0;
int distanceHigh = 0;
int lengthOfScale = 0;

/*************
*** Scales ***
**************/

int note = 0;

//E major scale
int scaleOne[] = 
{
  165, 185, 207, 220, 247, 277, 311, 330,
  370, 415, 440, 494, 554, 622, 659, 740, 
  831, 880, 988, 1109, 1244, 1319
};

//G major scale
int scaleTwo[] = 
{
  196, 220, 247, 262, 294, 330, 370, 392, 440,
  494, 523, 587, 659, 740, 784, 880, 988, 1047,
  1175, 1319, 1480, 1568
};

//C minor pentatonic scale
int scaleThree[] = 
{
   233, 262, 311, 349, 370, 392, 466, 523, 622,
   698, 740, 784, 932, 1047, 1245, 1397, 1480, 1568,
   1865, 2093, 2489, 2794, 3136
};

//D minor pentatonic scale

int scaleFour[] =
{
  262, 294, 349, 392, 415, 440, 523, 587, 698, 784,
  831, 880, 1047, 1175, 1397, 1568, 1661, 1760, 2093,
  2349, 2794, 3136, 3520
};


/************
*** Setup ***
*************/

void setup() 
{
  Serial.begin(9600);

//Sensors

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

//DC motor 

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);

//LEDs
  
  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGreen, OUTPUT);
  pinMode(rgbBlue, OUTPUT);

//Potentiometer

  pinMode(potentiometer, INPUT);

/*Function to calibrate the distance 
between right hand and the theremin */

  defineDistance(distance, distanceHigh, lengthOfScale);
  
}

void loop() 
{ 

/**********************************
 *** Main function - play notes ***
 **********************************/

//Potentiometer value converted into scales
  
  int value = analogRead(potentiometer);
  int convertIntoScale = map(value, 0, 1023, 0, 3);

//Switch to play with selected scale

  switch(convertIntoScale)
  {
    case 0:
    playNotes(scaleOne);
    break;
    case 1:
    playNotes(scaleTwo);
    break;
    case 2:
    playNotes(scaleThree);
    break;
    case 3:
    playNotes(scaleFour);
    break;
  }

//Function for tremolo effect

  tremolo();
  
}

/*** Function "defineDistance" calibrates *** 
 *** the right sensor and its range. ******** 
 *** 5 seconds to get maximum distance. *****/

void defineDistance(int &distance, int &distanceHigh, int &lengthOfScale)
{

//LED goes on to indicate that calibration has started

  digitalWrite(rgbBlue, HIGH);
  digitalWrite(rgbGreen, HIGH);

  delay(1000);

//5 seconds allowed to get the highest distance
  
  while (millis() < 5000) 
  {
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);

  distance = pulseIn(echo2, HIGH);

    if (distance > distanceHigh) 
    {
      distanceHigh = distance;
    }  
  }

  for (int i = 0; i < (sizeof(scaleOne) / sizeof(scaleOne[0])); i++) 
    {
      lengthOfScale += 1;
    }

//LED goes off to indicate that calibration has finished

  delay(1000);
  digitalWrite(rgbBlue, LOW);
  digitalWrite(rgbGreen, LOW);
  
}

//Function "tremolo" constrols DC motor rotation

void tremolo()
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(1);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigger1, LOW);
  delayMicroseconds(1);
  
  distance = pulseIn(echo1, HIGH);

  int spin = map(distance, 250, 1500, 90, 220);
  
  analogWrite(ENABLE, abs(spin)); //enable on
  digitalWrite(DIRA,HIGH);
}


/************ MAIN FUNCTION **************
 *** Plays the array of notes selected ***
 *** by user through potentiometer. ******/

void playNotes(int scale[])
{
  
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  
  distance = pulseIn(echo2, HIGH);
  
  note = map(distance, 240, distanceHigh, scale[0], scale[lengthOfScale - 1]);

  for (int j = 0; j < (lengthOfScale); j++) 
  {

//LEDs function

    rgb(j);
          
    if (note == scale[j]) 
    {
      tone(speaker, note);
      break;
    }  
    else if (note > scale[j] && note < scale[j + 1]) 
    {
      note = scale[j];
      tone(speaker, note);
      break;
    }
    
  }
  delay(10);
}

/*** "rgb" function defines the ***
 *** RGB led colors based on ******
 *** low to high pitch notes ******/

void rgb(int j)

{
  if(j <= 4)
  {
  digitalWrite(rgbRed, LOW);
  digitalWrite(rgbGreen, LOW);
  digitalWrite(rgbBlue, HIGH);
  }
  
  if(j > 4 && j <= 8)
  {
  digitalWrite(rgbRed, LOW);
  digitalWrite(rgbGreen, HIGH);
  digitalWrite(rgbBlue, HIGH);
  }
  if(j > 8 && j <= 12)
  {
  digitalWrite(rgbRed, LOW);
  digitalWrite(rgbGreen, HIGH);
  digitalWrite(rgbBlue, LOW);
  }
  if(j > 12 && j <= 16)
  {
  digitalWrite(rgbRed, HIGH);
  digitalWrite(rgbGreen, HIGH);
  digitalWrite(rgbBlue, LOW);
  }
  if(j > 16)
  {
  digitalWrite(rgbRed, HIGH);
  digitalWrite(rgbGreen, LOW);
  digitalWrite(rgbBlue, LOW);
  }
}
