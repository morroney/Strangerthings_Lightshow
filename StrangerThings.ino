// By Dan Morrone
// Based on the work previously done by: https://gist.github.com/steveosoule/f9d98b58ae666bce6a6752cefa2d11aa
// Wiring: http://www.instructables.com/id/Arduino-Based-Stranger-Things-Lights/ 
// The wire diagram shows four wires in the band. My model of WS8211 has 3 bands, and I found the one in the middle was the signal wire.)

// use fast led library and define the color order as RGB
#include <FastLED.h>
#define COLOR_ORDER RGB

// more config... 
#define DATA_PIN 6 // edit if using a different pin out
#define NUM_LEDS 100 // the total leds in your setup, summing all connected strands. 
CRGB leds[NUM_LEDS];

// overall speed control 
#define SPEED_RATE 9 // 1=FAST/TEST, 5=NORMAL, 10=SLOW.

// reusable time definitions
#define DURATION_XXXSMALL SPEED_RATE * 2
#define DURATION_XXSMALL SPEED_RATE * 5
#define DURATION_XSMALL SPEED_RATE * 7
#define DURATION_SMALL SPEED_RATE * 50
#define DURATION_MEDIUM SPEED_RATE * 100
#define DURATION_MEDIUM1 SPEED_RATE * 155
#define DURATION_XMEDIUM2 SPEED_RATE * 300
#define DURATION_LARGE1 SPEED_RATE * 800
#define DURATION_LARGE2 SPEED_RATE * 1000

// definitions for light message function
#define DURATION_CHAR DURATION_MEDIUM
#define DURATION_SPACE DURATION_MEDIUM * 3
#define DURATION_MESSAGE DURATION_LARGE2

// for random delay function
#define RANDOM_VARIANCE 0 // decimal in range 0-1

// for red glow function
#define GLOW_STEP_RGB DURATION_XXSMALL
#define GLOW_STEP_DELAY DURATION_XXSMALL
#define GLOW_STEP_DELAY2 DURATION_XSMALL

// for flicker
int fadeAmount = 5;
int brightness = 0; 

// to randomize the leds during enter/exit
int randNumber=0;

// stopping piont of the fade step 1
int fadeStepDown = 25;


// not in use but saved from original sketch for posterity.
// #define ALPHABET_CHAR_COUNT 26
// #define BYTE_ALPHA_OFFSET 97; // byte("a") = 97

/**
 * ONE TIME SETUP
 **/

void setup() {
  // roll up a fresh FastLED instance
  FastLED.addLeds < WS2811, DATA_PIN, COLOR_ORDER > (leds, NUM_LEDS);
  // wipe slate
  FastLED.clear(); 
}

/**
 * THE LOOP
 */

void loop() {
  // play the stranger things light show repeatedly. 
  play_stranger_things_lightshow();
}

/**
 * CUSTOM: STRANGER THINGS LIGHTSHOW TIMELINE
 */

void play_stranger_things_lightshow()
{
  // start of show, peaceful white lights. hold a few beats.
  // symbolizes the moment Will first speaks to his mother through the white christmas lights. 
  openiniglights();
  delay(DURATION_MEDIUM1);

  // sudden flickering disturbs the peace. hold a few beats again.
  short_flicker();
  flicker2();
  flicker2();
  delay(DURATION_MEDIUM);

  // will byers affects the lights
  enter_will();
  fill_gaps();
  delay(DURATION_LARGE1);
  blackout();

  // will spells out a message. edit this to whatever...
  light_message("right here");
  delay(DURATION_MEDIUM);

  // sudden flickering disturbs the peace again, followed by blackout
  distressed_will();
  distressed_will();
  blackout();
  delay(DURATION_XMEDIUM2);

  // will spells out a warning. also edit this to whatever...
  light_message("run");
  
  // glow red for urgency / stranger things branding
  glow_red();
  blackout();
  
  // hold the blackout for a long beat.
  delay(DURATION_LARGE2);

}


/**
 * CUSTOM: HELPER FUNCTIONS
 */

void light_message(String message) {

  message.toLowerCase();
  
  for(int i = 0; i < message.length(); i++) {
    char letter = message[i];    

    if( isSpace(letter) ) {
      FastLED.clear();
      FastLED.show();
      delay(DURATION_MEDIUM);
      continue;
    }
    
    if( !isAlpha(letter) ) continue;

    if(message == "run"){
      random_delay(DURATION_MEDIUM1);
    }else{
      random_delay(DURATION_CHAR);
    }
    FastLED.clear();

    // retrieve led that is mapped to this letter
    int letter_led_index = get_letter_led_index(letter);
    
    // prepare to highlight the letter by setting color. 
    // random looks nice but white is brightest and helps with visibility.

    leds[letter_led_index] = CRGB::White; // highlight letter with white
    //set_led_random_color(letter_led_index); // highlight letter with random color

    
    FastLED.show();

    // custom timing for run .. a little more delayed and dramatic. 
    if(message == "run"){
      random_delay(DURATION_MEDIUM);
    }else{
      random_delay(DURATION_CHAR);
    }
  }
  FastLED.clear();
  FastLED.show();

  if(message=="run"){
    delay(DURATION_SMALL);
  }else{
    delay(DURATION_MEDIUM);
  }
  
}

int get_letter_led_index(char letter) { 

  /*
  float letter_index = byte(letter) - BYTE_ALPHA_OFFSET;
  float led_index = floor((letter_index / ALPHABET_CHAR_COUNT) * 100);  
  return led_index;
  */ 

  int led_index = 0;

  // MAP LIGHTS TO LETTERS. DEPENDENT UPON EACH INDIVIDUAL PHYSICAL SETUP.
  if 	    (letter == 'a') { led_index = 54; }
  else if (letter == 'b') { led_index = 56; }
  else if (letter == 'c') { led_index = 58; }
  else if (letter == 'd') { led_index = 59; }
  else if (letter == 'e') { led_index = 61; }
  else if (letter == 'f') { led_index = 62; }
  else if (letter == 'g') { led_index = 64; }
  else if (letter == 'h') { led_index = 65; }
  else if (letter == 'i') { led_index = 82; }
  else if (letter == 'j') { led_index = 80; }
  else if (letter == 'k') { led_index = 79; }
  else if (letter == 'l') { led_index = 76; }
  else if (letter == 'm') { led_index = 74; }
  else if (letter == 'n') { led_index = 75; }
  else if (letter == 'o') { led_index = 72; }
  else if (letter == 'p') { led_index = 71; }
  else if (letter == 'q') { led_index = 70; }
  else if (letter == 'r') { led_index = 88; }
  else if (letter == 's') { led_index = 89; }
  else if (letter == 't') { led_index = 91; }
  else if (letter == 'u') { led_index = 92; }
  else if (letter == 'v') { led_index = 94; }
  else if (letter == 'w') { led_index = 96; }
  else if (letter == 'x') { led_index = 97; }
  else if (letter == 'y') { led_index = 98; }
  else if (letter == 'z') { led_index = 99; }

  return led_index;
}

void random_delay(int duration) {
  float random_low = (1-RANDOM_VARIANCE) * duration;
  float random_high = (1+RANDOM_VARIANCE) * duration;
  delay( random(random_low, random_high) );
}

/**
 * CUSTOM: SECONDARY ANIMATIONS
 */

void enter_will() {
  // for (int i = 0; i < NUM_LEDS; i++) { //change direction
  for (int i = 200; i >= 0; i--) 
  {
    randNumber = random(100);
   
    delay(DURATION_XXSMALL); 
    //set_led_random_color(i);    
    set_led_random_color(randNumber);    
    
    FastLED.show();
    
  }
}

void fill_gaps(){
  // for (int i = 0; i < NUM_LEDS; i++) { //change direction
  for (int i = NUM_LEDS-1; i >= 0; i--) 
  {
    // see if this bulb is lit already
    if( !leds[i] ) 
    {
      // FastLED.clear();
      delay(DURATION_XXXSMALL); 
      set_led_random_color(i);    
      FastLED.show();
    }
  }
}


void distressed_will() {

  for (int i = 0; i < 70; i++) {
  //for (int i = NUM_LEDS-1; i >= 0; i--) {
    // FastLED.clear();

    randNumber = random(100);
    
    delay(DURATION_XXSMALL);    
    
    //set_led_random_color(i);    
    set_led_random_color(randNumber);    

    FastLED.show();

    for(int i = 0; i < NUM_LEDS; i++ )
   {
   //leds[i].setRGB(0,255,250);  // Set Color HERE!!!
   leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade: 
  if(brightness == 0 || brightness == 255)
  {
    fadeAmount = -fadeAmount ; 
  }    
  delay(18);  // This delay sets speed of the fade. I usually do from 5-75 but you can always go higher.
    
  }
}

void glow_red(){
  glow_red_up();
  fade_step1();
  fade_step2();
}

void glow_red_up() {
  FastLED.clear();
  
  for( int i = 0; i < 255; i = i + GLOW_STEP_RGB ) {

      int r = i;
      int b = 0;
      int g = 0;

      for(int led_index = 0; led_index < NUM_LEDS; led_index++){
          leds[led_index] = CRGB(r,g,b);
      }

      FastLED.show();
      delay(GLOW_STEP_DELAY);
    }
    random_delay(DURATION_MESSAGE);
}

void fade_step1() {
  for (int i = 255; i > fadeStepDown; i = i - GLOW_STEP_RGB){

      int r = i;
      int b = 0;
      int g = 0;

      for(int led_index = 0; led_index < NUM_LEDS; led_index++){
          leds[led_index] = CRGB(r,g,b);
      }

      FastLED.show();
      delay(GLOW_STEP_DELAY);
    }
        random_delay(DURATION_MESSAGE);

}

void fade_step2(){
  FastLED.clear();
  
  for (int i = fadeStepDown; i > 0; i = i - GLOW_STEP_RGB){

      int r = i;
      int b = 0;
      int g = 0;

      for(int led_index = 0; led_index < NUM_LEDS; led_index++){
          leds[led_index] = CRGB(r,g,b);
      }

      FastLED.show();
      delay(GLOW_STEP_DELAY);
    }
}


void blackout(){
      int r = 0;  
      int b = 0;  
      int g = 0;   

      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }
          
      FastLED.show();
      delay(DURATION_MEDIUM); 
      
}


void openiniglights(){
  FastLED.clear();
 
  for( int i = 0; i < 255; i = i + GLOW_STEP_RGB ) {

      int r = i;
      int b = i;
      int g = i;

      for(int led_index = 0; led_index < NUM_LEDS; led_index++){
          leds[led_index] = CRGB(r,g,b);
      }

      FastLED.show();
      delay(GLOW_STEP_DELAY2);
    }
  delay(9500);  
}


void short_flicker(){

  for (int ii=0; ii <2; ii++)
  {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;  //set color
  }
  
    FastLED.show();       //start the leds
    delay(10);          //Wait 1 sec =1000
  

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;  //set the all leds to Black (off)
  }
  FastLED.show();       //start the leds
  delay(100);           //Wait 1/2 second = 500
  }
}

void flicker(){

  for (int ii=0; ii <3; ii++)
  {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;  //set color
  }
  
    FastLED.show();       //start the leds
    delay(10);          //Wait 1 sec =1000
  

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;  //set the all leds to Black (off)
  }
  FastLED.show();       //start the leds
  delay(100);           //Wait 1/2 second = 500
  }
}

void flicker2(){

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;  //set color
  }

  
    FastLED.show();       //start the leds
    delay(33);          //Wait 1 sec =1000
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;  //set the all leds to Black (off)
  }
  FastLED.show();       //start the leds

    delay(444);           //Wait 1/2 second = 500

}


/**
 * CUSTOM: HELPER FOR RETURNING A RANDOM COLOR
 */

void set_led_random_color(int led_index) {
  leds[led_index] = CRGB(random(0,255),random(0,255),random(0,255));
}