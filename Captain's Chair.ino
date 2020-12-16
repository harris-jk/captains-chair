
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"
#include <Adafruit_NeoPixel.h>


#define HIGH 0x1
#define LOW  0x0


Adafruit_NeoPixel blinkStrip(9, 30, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel alertStrip(5, 31, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel commStrip(1, 34, NEO_RGBW + NEO_KHZ800);


// soundboard pins and setup
#define SFX_RST 7
#define SFX_RX 0
#define SFX_TX 1
const int ACT = 6;    // this allows us to know if the audio is playing

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard( &ss, NULL, SFX_RST);

uint32_t white = blinkStrip.Color(0, 0, 0, 255);
uint32_t off = blinkStrip.Color(0, 0, 0, 0);



unsigned long startTime [9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long duration [9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
unsigned long previousMillisyel = 0;
unsigned long previousMillisred = 0;
unsigned long computeStatemillis = 0;
unsigned long commStatemillis = 0;
unsigned long prevCommMillis = 0;
unsigned long notifyMillis = 0;
long commDelay = 0;

const byte commDelays [] = {3000, 5000, 4000, 3500, 4500};
int randNumber;
const long alertInterval = 1000;
const int computeStateWaitTime = 3000;
const int yellowAlertbtn = 2;
int yellowAlertbuttonstatelast = 0;
int yellowpushCounter = 0;
const int redAlertbtn = 3;
int redAlertbuttonstatelast = 0;
int redpushCounter = 0;
int cardReaderbtn = A1;
int computerData = 0;
int computerOutput = 1;
int commOutput = 1;
int commSwitch = 32;
int commActivate = 28;
int commState = 0;
int commActivated = 0;
int commOn = 0;
int commButtonstatelast = 0;
int commpushCounter = 0;
int commLight = 0;
int speakerLight = 0;
int redLight = 3;
int yellowLight = 4;
int phaserButton = 33;
int torpButton = 35;
int phaserFire = 0;
int torpFire = 0;
int phaserLight = 2;
int torpLight = 1;
int lightFlashsw = 37;
int lightFlash = 0;
int ambientSoundsw = 41;
int ambientSound = 0;
int transportersw = 43;
int transporterSound = 0;
int warpSoundsw = 45;
int warpSound = 0;

//random blinking maximum and minimum values
const int randMax = 8000;
const int randMin = 2000;
const int commMax = 15000;
const int commMin = 8000;

bool bootUp = false;
bool light1state = false;
bool light2state = false;
bool light3state = false;
bool light4state = false;
bool light5state = false;
bool light6state = false;
bool light7state = false;
bool light8state = false;
bool light9state = false;
bool yellowAlerton = false;
bool yellowAlert = false;
bool redAlerton = false;
bool redAlert = false;
bool computingState = false;
bool computeDone = false;
bool beep = false;
bool shouldCompute = false;
bool redalertplaying = false;
bool notifyOn = true;
bool shouldComm = false;
bool playComm = false;
bool loopSound = true;
bool stopLoop = false;
bool transported = false;
bool warpspeed = false;

// audio track names on soundboard
char ambientTrack[] =     "T00     OGG";
char dropCard[] =         "T01     OGG";
char pullCard[] =         "T02     OGG";
char kirkData[] =         "T03     OGG";
char spockData[] =        "T04     OGG";
char logentry1 [] =       "T05     OGG";
char music1 [] =          "T06     OGG";
char logentry2 [] =       "T07     OGG";
char logentry3 [] =       "T08     OGG";
char redAlertaudio [] =   "T09     OGG";
char commAudio1 [] =      "T10     OGG";
char commAudio2 [] =      "T11     OGG";
char commAudio3 [] =      "T12     OGG";
char commAudio4 [] =      "T13     OGG";
char commAudio5 [] =      "T14     OGG";
char commAudio6 [] =      "T15     OGG";
char commAudio7 [] =      "T16     OGG";
char commAudio8 [] =      "T17     OGG";
char commAudio9 [] =      "T18     OGG";
char commAudio10 [] =     "T19     OGG";
char commAudio11 [] =     "T20     OGG";
char commHail [] =        "T21     OGG";
char phaserAudio [] =     "T22     OGG";
char torpAudio [] =       "T23     OGG";
char transpAudio [] =     "T29     OGG";
char warpAudio [] =       "T30     OGG";



void setup() {

  ss.begin(9600);
  // if (!sfx.reset()) {
  //while (1);
  //}
  pinMode(ACT, INPUT);
  pinMode(yellowAlertbtn, INPUT);
  digitalWrite (yellowAlertbtn, HIGH);
  pinMode (redAlertbtn, INPUT);
  digitalWrite (redAlertbtn, HIGH);
  pinMode (cardReaderbtn, INPUT);
  digitalWrite (cardReaderbtn, HIGH);
  pinMode (commSwitch, INPUT);
  digitalWrite (commSwitch, HIGH);
  pinMode (commActivate, INPUT);
  digitalWrite (commActivate, HIGH);
  pinMode (phaserButton, INPUT);
  digitalWrite (phaserButton, HIGH);
  pinMode (torpButton, INPUT);
  digitalWrite (torpButton, HIGH);
  pinMode (lightFlashsw, INPUT);
  digitalWrite (lightFlashsw, HIGH);
  pinMode (ambientSoundsw, INPUT);
  digitalWrite (ambientSoundsw, HIGH);
  pinMode (transportersw, INPUT);
  digitalWrite (transportersw, HIGH);
  pinMode (warpSoundsw, INPUT);
  digitalWrite (warpSoundsw, HIGH);


  blinkStrip.begin();
  blinkStrip.show(); // Initialize all pixels to 'off'
  alertStrip.begin();
  alertStrip.show(); // Initialize all pixels to 'off'
  commStrip.begin();
  commStrip.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(0));

}

void loop() {

  randNumber = random(1, 10); //get random number between 1 and 10 for blink sequence
  unsigned long currentMillis = millis();
  int playing = digitalRead(ACT);
  computingState = digitalRead (cardReaderbtn);
  commOn = digitalRead (commSwitch);
  phaserFire = digitalRead (phaserButton);
  torpFire = digitalRead (torpButton);
  lightFlash = digitalRead(lightFlashsw);
  ambientSound = digitalRead(ambientSoundsw);
  transporterSound = digitalRead(transportersw);
  warpSound = digitalRead(warpSoundsw);

  if (ambientSound == LOW) {
    loopSound = true;
  } else {
    if (stopLoop == true) {
      stopLoop = false;
      loopSound = false;
      sfx.stop ();
    }
  }
  if (playing == 1 && shouldCompute == false && playComm == false && loopSound == true) //ambient bridge noise playing when nothing is happening
  {
    playAudio(ambientTrack, playing);
    stopLoop = true;
  }
  if (bootUp == false) // Left Control Panel flashing light function call
  {
    if (lightFlash == HIGH) {
      callLight(randNumber);
    } else {
      blinkStrip.fill (white, 0, 9);
      blinkStrip.show ();
    }
  }

  if (phaserFire == LOW) {
    alertStrip.setPixelColor(phaserLight, white);
    alertStrip.show();
    playAudio (phaserAudio, playing);

  } else {
    alertStrip.setPixelColor(phaserLight, off);
    alertStrip.show();
  }

  if (torpFire == LOW) {
    alertStrip.setPixelColor(torpLight, white);
    alertStrip.show();
    playAudio (torpAudio, playing);
  } else {
    alertStrip.setPixelColor(torpLight, off);
    alertStrip.show();
  }

  if (transporterSound == LOW)
  {
    if (transported == false) {
      transported = true;
      playAudio (transpAudio, playing);
    }
  } else {
    transported = false;
  }
  
 if (warpSound == LOW)
  {
    if (warpspeed == false) {
      warpspeed = true;
      playAudio (warpAudio, playing);
    }
  } else {
    warpspeed = false;
  }


  //Comm Panel and Sound Function *******************
  if (commOn == LOW) {
    chairComm();
  } else {
    alertStrip.setPixelColor(commLight, off);
    commStrip.setPixelColor(speakerLight, off);
    alertStrip.show();
    commStrip.show();
  }
  // Computer and Micro Tape functions *********************
  if (computingState == LOW)
  {

    if (beep == false)
    {
      beep = true;
      //shouldCompute = false;
      computeStatemillis = millis();
      playAudio (dropCard, playing);
    }

    long diff = millis() - computeStatemillis;
    if  (diff > computeStateWaitTime) {
      if (shouldCompute == false)
      {
        shouldCompute = true;
        playComputerTrack(playing);
        computeDone = true;
      }
    }
  }

  if (computingState == HIGH)  {
    if (shouldCompute == true) {
      beep = false;
      shouldCompute = false;
      computeDone = false;
      playAudio (pullCard, playing);
      computerOutput ++;

    }
  }

  //Red Alert Functions********

  if (redAlert) { //function for red alert
    redAlertSeq ();
    if (redalertplaying == false) {
      redalertplaying = true;
      playAudio(redAlertaudio, playing);

    }
  } else {
    alertStrip.setPixelColor(redLight, off);
    alertStrip.show();
    redalertplaying = false;

  }
  int redAlertbuttonstate = digitalRead (redAlertbtn);
  if (redAlertbuttonstate != redAlertbuttonstatelast) {
    if (redAlertbuttonstate == 1)
      redpushCounter ++;
    if (redpushCounter > 1) redpushCounter = 0;
  }
  redAlertbuttonstatelast = redAlertbuttonstate;
  if (redpushCounter == 0)
  { redAlert = true;

  } else {
    redAlert = false;
    redalertplaying = false;
  }


  //Yellow Alert Functions********

  if (yellowAlert) { //function for yellow alert

    yellowAlertSeq ();
  } else {
    alertStrip.setPixelColor(yellowLight, off);
    alertStrip.show();
  }
  int yellowAlertbuttonstate = digitalRead (yellowAlertbtn);
  if (yellowAlertbuttonstate != yellowAlertbuttonstatelast) {
    if (yellowAlertbuttonstate == 1)
      yellowpushCounter ++;
    if (yellowpushCounter > 1) yellowpushCounter = 0;
  }
  yellowAlertbuttonstatelast = yellowAlertbuttonstate;
  if (yellowpushCounter == 0)
  { yellowAlert = true;
  } else {
    yellowAlert = false;
  }
}


/* ************* Audio Board Helper Functions ************* */
// helper function to play a track by name on the audio board
void playAudio( char * trackname, int playing ) {
  // stop track if one is going
  if (playing == 0) {
    sfx.stop();
  }

  // now go play
  if (sfx.playTrack(trackname)) {
    sfx.unpause();
  }
}

void chairComm() {

  unsigned long currentMillis = millis ();
  int commActivated = digitalRead (commActivate);
  int commButtonstate = digitalRead (commActivate);
  int notifyInt = 750;
  commDelay = random(commMin, commMax);
  int playing = digitalRead(ACT);

  switch (commState) {

    case (0):   //RESET
      commState = 1;
      prevCommMillis = millis ();
      break;

    case (1): //TIMER
      if (currentMillis - prevCommMillis >= commDelay) {

        playAudio(commHail, playing);
        commState = 2;
      }
      break;

    case (2): //NOTIFICATION

      if (commActivated == HIGH) {
        if (currentMillis - notifyMillis >= notifyInt) { // enough time passed yet?
          notifyMillis = millis(); // sets the time we wait "from"

          if (notifyOn == true)
          {

            alertStrip.setPixelColor(commLight, white);
            notifyOn = false;
          } else {
            alertStrip.setPixelColor(commLight, off);
            notifyOn = true;
          }
        }
        alertStrip.show();
      }

      if (commActivated == LOW)  {
        sfx.stop();
        playComm = true;
        alertStrip.setPixelColor(commLight, white);
        commStrip.setPixelColor(speakerLight, white);
        commState = 3;
      }
      break;

    case (3)://PLAY SOUND

      playCommTrack (commOutput);
      playComm = false;
      commState = 4;
      break;

    case (4):
      if (commActivated == LOW) {
        alertStrip.setPixelColor(commLight, off);
        commStrip.setPixelColor(speakerLight, off);
        commState = 0;
      }
      break;
  }
  alertStrip.show();
  commStrip.show();
}



void playComputerTrack( int playing ) {
  if ( computerOutput <= 5) {
    switch (computerOutput) {
      case (1):
        playAudio(kirkData, playing);
        break;
      case (2):
        playAudio(spockData, playing);
        break;
      case (3):
        playAudio(logentry1, playing);
        break;
      case (4):
        playAudio(logentry2, playing);
        break;
      case (5):
        playAudio(logentry3, playing);
        break;
      case (6):
        playAudio(music1, playing);
        break;
        // case (7):
        //   playAudio(toolsTrack, playing);
        //   break;
        // default:
        //  playAudio(endTrack, playing);
        //  break;
    }

  } else {
    computerOutput = 1;

  }
}

void playCommTrack( int playing ) {
  if ( commOutput <= 11) {

    switch (commOutput) {
      case (1):
        playAudio(commAudio1, playing);
        break;
      case (2):
        playAudio(commAudio2, playing);
        break;
      case (3):
        playAudio(commAudio3, playing);
        break;
      case (4):
        playAudio(commAudio4, playing);
        break;
      case (5):
        playAudio(commAudio5, playing);
        break;
      case (6):
        playAudio(commAudio6, playing);
        break;
      case (7):
        playAudio(commAudio7, playing);
        break;
      case (8):
        playAudio(commAudio8, playing);
        break;
      case (9):
        playAudio(commAudio9, playing);
        break;
      case (10):
        playAudio(commAudio10, playing);
        break;
      case (11):
        playAudio(commAudio11, playing);
        break;

        // default:
        //  playAudio(endTrack, playing);
        //  break;
    }
    commOutput ++;
  } else {
    commOutput = 1;

  }
}



void redAlertSeq () {
  if (millis() - previousMillisred >= alertInterval) { // enough time passed yet?
    previousMillisred = millis(); // sets the time we wait "from"
    if (redAlerton == true)
    {
      alertStrip.setPixelColor(redLight, white);
      redAlerton = false;
    } else {
      alertStrip.setPixelColor(redLight, off);
      redAlerton = true;

    }
  }
  alertStrip.show();
}




void yellowAlertSeq () {
  if (millis() - previousMillisyel >= alertInterval) { // enough time passed yet?
    previousMillisyel = millis(); // sets the time we wait "from"
    if (yellowAlerton == true)
    {
      alertStrip.setPixelColor(yellowLight, white);
      yellowAlerton = false;
    } else {
      alertStrip.setPixelColor(yellowLight, off);
      yellowAlerton = true;

    }
  }
  alertStrip.show();
}





//Generate random function calls for random lights
void callLight(int randNumber) {

  if (randNumber == 1)  {
    light1();
  }
  if (randNumber == 2) {
    light2();
  }
  if (randNumber == 3) {
    light3();
  }
  if (randNumber == 4) {
    light4();
  }
  if (randNumber == 5) {
    light5();
  }
  if (randNumber == 6) {
    light6();
  }
  if (randNumber == 7) {
    light7();
  }
  if (randNumber == 8) {
    light8();
  }
  if (randNumber == 9) {
    light9();
  }
}

// Functions to control random flashing lights, using led matrix and flashing groups of three together
void light1() {
  if (millis() - startTime[1] >= duration[1])
  { startTime[1] = millis();
    duration[1] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light1state == true) {
      blinkStrip.setPixelColor(0, white);
      light1state = false;
    } else {
      blinkStrip.setPixelColor(0, off);
      light1state = true;
    }
  }
  blinkStrip.show();
}

void light2() {
  if (millis() - startTime[2] >= duration[2])
  { startTime[2] = millis();
    duration[2] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light2state == true) {
      blinkStrip.setPixelColor(1, white);
      light2state = false;
    } else {
      blinkStrip.setPixelColor(1, off);
      light2state = true;
    }
  }
  blinkStrip.show();
}
void light3() {
  if (millis() - startTime[3] >= duration[3])
  { startTime[3] = millis();
    duration[3] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light3state == true) {
      blinkStrip.setPixelColor(2, white);
      light3state = false;
    } else {
      blinkStrip.setPixelColor(2, off);
      light3state = true;
    }
  }
  blinkStrip.show();
}
void light4() {
  if (millis() - startTime[4] >= duration[4])
  { startTime[4] = millis();
    duration[4] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light4state == true) {
      blinkStrip.setPixelColor(3, white);
      light4state = false;
    } else {
      blinkStrip.setPixelColor(3, off);
      light4state = true;
    }
  }
  blinkStrip.show();
}
void light5() {
  if (millis() - startTime[5] >= duration[5])
  { startTime[5] = millis();
    duration[5] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light5state == true) {
      blinkStrip.setPixelColor(4, white);
      light5state = false;
    } else {
      blinkStrip.setPixelColor(4, off);
      light5state = true;
    }
  }
  blinkStrip.show();
}
void light6() {
  if (millis() - startTime[6] >= duration[6])
  { startTime[6] = millis();
    duration[6] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light6state == true) {
      blinkStrip.setPixelColor(5, white);
      light6state = false;
    } else {
      blinkStrip.setPixelColor(5, off);
      light6state = true;
    }
  }
  blinkStrip.show();
}
void light7() {
  if (millis() - startTime[7] >= duration[7])
  { startTime[7] = millis();
    duration[7] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light7state == true) {

      blinkStrip.setPixelColor(6, white);
      light7state = false;
    } else {
      blinkStrip.setPixelColor(6, off);
      light7state = true;
    }
  }
  blinkStrip.show();
}
void light8() {
  if (millis() - startTime[8] >= duration[8])
  { startTime[8] = millis();
    duration[8] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light8state == true) {
      blinkStrip.setPixelColor(7, white);
      light8state = false;
    } else {
      blinkStrip.setPixelColor(7, off);
      light8state = true;
    }
  }
  blinkStrip.show();
}
void light9() {
  if (millis() - startTime[0] >= duration[0]) //using zero because numbering system is 0-8 rather than 1-9
  { startTime[0] = millis();
    duration[0] = random(randMin, randMax);  //smallest and longest ON/OFF times for reandom blinking
    if (light9state == true) {
      blinkStrip.setPixelColor(8, white);
      light9state = false;
    } else {

      blinkStrip.setPixelColor(8, off);
      light9state = true;
    }
  }
  blinkStrip.show();
}
