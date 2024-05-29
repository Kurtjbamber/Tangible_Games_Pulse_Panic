#include <Adafruit_NeoPixel.h>




#include "pitches.h"




int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};




// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};




bool allOut = false;








//green
const int button1 = 22;
int button1State = 0;
bool pushed1 = false;
bool off1 = false;
bool out1 = false;
static int pixelNum1 = 0;
unsigned long time1 = micros();








//hm
//red
const int button2 = 27;
int button2State = 0;
bool pushed2 = false;
bool off2 = false;
bool out2 = false;
static int pixelNum2 = 0;
unsigned long time2 = micros();








//blue
const int button3 = 33;
int button3State = 0;
bool pushed3 = false;
bool off3 = false;
bool out3 = false;
static int pixelNum3 = 0;
unsigned long time3 = micros();








//yellow
//hm
const int button4 = 15;
int button4State = 0;
bool pushed4 = false;
bool off4 = false;
bool out4 = false;
static int pixelNum4 = 0;
unsigned long time4 = micros();




#define NUMPIXELS 144
unsigned long DELAYVAL = 20;




Adafruit_NeoPixel strip1(NUMPIXELS, 5, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUMPIXELS, 4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3(NUMPIXELS, 13, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip4(NUMPIXELS, 18, NEO_GRBW + NEO_KHZ800);




uint32_t black = strip1.Color(0, 0, 0, 0);




void setup() 
{
  Serial.begin(115200);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  strip1.begin();
  strip2.begin();
  strip1.clear();
  strip2.clear();
  strip1.show();
  strip2.show();
  strip3.begin();
  strip4.begin();
  strip3.clear();
  strip4.clear();
  strip3.show();
  strip4.show();
}




void loop() 
{
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);
  button4State = digitalRead(button4);




  if (button1State == LOW && !pushed1) 
  {
    pushed1 = true;
    pixelNum1 = 0;
    setPixelColours(strip1, out1);
  }








  if (button1State == HIGH && pushed1) 
  {
    pushed1 = false;
    off1 = true;
    time1 = micros();
  }








  if (off1 && !pushed1) 
  {
    turnOffPixels(strip1, pixelNum1, off1, out1, time1);
  }








  if (button2State == LOW && !pushed2) 
  {
    pushed2 = true;
    pixelNum2 = 0;
    setPixelColours(strip2, out2);
  }








  if (button2State == HIGH && pushed2) 
  {
    pushed2 = false;
    off2 = true;
    time2 = micros();
  }








  if (off2 && !pushed2) 
  {
    turnOffPixels(strip2, pixelNum2, off2, out2, time2);
  }












  if (button3State == LOW && !pushed3) 
  {
    pushed3 = true;
    pixelNum3 = 0;
    setPixelColours(strip3, out3);
  }








  if (button3State == HIGH && pushed3) 
  {
    pushed3 = false;
    off3 = true;
    time3 = micros();
  }








  if (off3 && !pushed3) 
  {
    turnOffPixels(strip3, pixelNum3, off3, out3, time3);
  }












  if (button4State == LOW && !pushed4) 
  {
    pushed4 = true;
    pixelNum4 = 0;
    setPixelColours(strip4, out4);
  }








  if (button4State == HIGH && pushed4) 
  {
    pushed4 = false;
    off4 = true;
    time4 = micros();
  }








  if (off4 && !pushed4) 
  {
    turnOffPixels(strip4, pixelNum4, off4, out4, time4);
  }




  if(out1 && out2 && out3 && out4)
  {
    allOut = true;
  }




  unsigned static long lastRoundTime = millis();
  unsigned static long nextRound = random(8000, 14000);




    if(millis() - lastRoundTime > nextRound)
    {
      playSound();
      Serial.println(DELAYVAL);
      if(DELAYVAL > 1)
      {
        DELAYVAL -= 2;
      }
      lastRoundTime = millis();
      nextRound = random(3000, 6000);




    }
  
}








void setPixelColours(Adafruit_NeoPixel &pixels, bool &out) 
{
  if(!out)
  {
    pixels.fill(pixels.Color(0, 200, 0), 0, 48);
    pixels.fill(pixels.Color(255, 70, 0), 49, 96);
    pixels.fill(pixels.Color(250, 0, 0), 97, 143);
    pixels.show();
  }
}








void turnOffPixels(Adafruit_NeoPixel &pixels, int &pixelNum, bool &off, bool &out, unsigned long &time)
{
  if (pixelNum < NUMPIXELS) 
  {
    uint32_t currentColour = pixels.getPixelColor(pixelNum);








      Serial.print("millis() = ");
      Serial.println(millis());
      Serial.print("time = ");
      Serial.println(time);








      if(micros() - time > DELAYVAL)
      {
        Serial.print("Blacken ");
        Serial.println(pixelNum);
        pixels.setPixelColor(pixelNum, black);
        pixels.show();
        time = micros();
        pixelNum++;
      }








  } else 
  {
    off = false;
    out = true;
  }
}




  unsigned static long soundTime = millis();




void playSound()
{




// iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {




    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(12, melody[thisNote], noteDuration);




    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    if(millis() - soundTime > pauseBetweenNotes)
    {
      noTone(12);
      soundTime = millis();
    }
  }
}