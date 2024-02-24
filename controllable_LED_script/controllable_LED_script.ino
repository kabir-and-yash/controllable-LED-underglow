#include<FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 2
#define COLOUR_ORDER GRB 
#define CHIPSET WS2812B
#define VOLTS 5
#define MAX_AMPS 500

// Deadzone Settings

#define X_GREATER_DZ 600
#define X_LESS_DZ 420

#define Y_GREATER_DZ 600
#define Y_LESS_DZ 420

#define tone_step 3
#define photoresistor_threshold 300

#define joystick_pin 5

int transition_speed = 40;

const int photoresistor_pin = A4; 

const int Ypin = A0;
const int Xpin = A1;

int brightness = 50;

int glob_red = 255;
int glob_green = 0;
int glob_blue = 0;

// 1 - RGB Picker; 2 - Dynamic Transitions

// int j = 29;

int X;
int Y;
int phtoresistor_value;
int joystick_value;
int colour_mode = 1;


bool show_light = true;
bool turned_off = false;

CRGB leds[NUM_LEDS]; 

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOUR_ORDER> (leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();

  // Initializing White Colour
  change_all_colour(255, 0, 255);

  pinMode(photoresistor_pin, INPUT);
  pinMode(joystick_pin, INPUT_PULLUP);

}

void loop() {

  // X = analogRead(Xpin);
  // Y = analogRead(Ypin);

  light_switch();

  // Serial.print("\nLight amount: ");
  // Serial.print(photoresistor_value);
  // Serial.print(" JOYSTICK : ");
  // Serial.print(joystick_value);
  // Serial.print(" MODE : ");
  // Serial.print(colour_mode);

  // Serial.print("\nX: ");
  // Serial.print(X);
  // Serial.print("  Y: ");
  // Serial.print(Y);
  // Serial.print("  Red: ");
  // Serial.print(glob_red);
  // Serial.print("  Green: ");
  // Serial.print(glob_green);
  // Serial.print("  Blue: ");
  // Serial.print(glob_blue);
  if (show_light == true) {

    if (colour_mode == 1) {
    change_all_colour(glob_red, glob_green, glob_blue);
    change_brightness_analog();
    change_colour_tone_analog();
        // Button is clicked
      if (joystick_value == 0) {
        colour_mode = 2;
        delay(1000);
      }
    }

    else if (colour_mode == 2) {
      // Colour dynamic thing here
      if (joystick_value == 0) {
        colour_mode = 1;
        delay(1000);
      }
      colour_fader(255, 0, 0);
      colour_fader(255, 0, 255);
      colour_fader(0, 255, 255);
      colour_fader(255, 255, 0);
      colour_fader(0, 0, 255);
      colour_fader(0, 255, 0);
    }
  }
  else {
    for (int i = 0; i < 60; ++i){
      leds[i] = CRGB(0, 0, 0);
  }
    }
  delay(5);
  // Serial.print(brightness);
  // Serial.print("\n");
  FastLED.show();

}


void change_brightness_analog() {

  // Notice, brightness is a global variable

  X = analogRead(Xpin);
  Y = analogRead(Ypin);

  // Decrease Brightness
  if (Y > Y_GREATER_DZ && brightness >= 1 && (X < X_GREATER_DZ && X > X_LESS_DZ)){
    brightness = brightness - 1;
    FastLED.setBrightness(brightness);
  }

  // Increase Brightness
  else if (Y < Y_LESS_DZ &&  (X < X_GREATER_DZ && X > X_LESS_DZ) && brightness <= 49) {
    brightness = brightness + 1;
    FastLED.setBrightness(brightness);
  }
}


void change_colour_tone_analog() {

  X = analogRead(Xpin);
  Y = analogRead(Ypin);

  // Increase Red 
  if ((Y < Y_LESS_DZ) && (X < X_LESS_DZ) && glob_red < 252){
    change_all_colour(glob_red + tone_step, glob_green, glob_blue);
  }

  // Decrease Red
  else if (Y < Y_LESS_DZ && (X > X_GREATER_DZ) && glob_red > 3) {
    change_all_colour(glob_red - tone_step, glob_green, glob_blue);

  }

  // Increase Green 
  else if ((Y > Y_LESS_DZ && Y < Y_GREATER_DZ) && (X < X_LESS_DZ) && glob_green < 252){
    change_all_colour(glob_red, glob_green + tone_step, glob_blue);
  }

  // Decrease Green
  else if ((Y > Y_LESS_DZ && Y < Y_GREATER_DZ) && (X > X_GREATER_DZ) && glob_green > 3) {
    change_all_colour(glob_red, glob_green - tone_step, glob_blue);
  }

  // Increase Blue 
  else if ((Y > Y_GREATER_DZ) && (X < X_LESS_DZ) && glob_blue < 252){
    change_all_colour(glob_red, glob_green, glob_blue + tone_step);
  }

  // Decrease Blue
  else if ((Y > Y_GREATER_DZ) && (X > X_GREATER_DZ) && glob_blue > 3) {
    change_all_colour(glob_red, glob_green, glob_blue - tone_step);
  }

}

void change_all_colour (int red, int green, int blue) {
  glob_red = red;
  glob_green = green;
  glob_blue = blue;

  for (int i = 0; i < 60; ++i){
    leds[i] = CRGB(glob_red, glob_green, glob_blue);
  }
  FastLED.show();
}

bool change_transition_speed(){
  X = analogRead(Xpin);
  Y = analogRead(Ypin);
  
  // Increase Speed
  if ((Y > Y_LESS_DZ && Y < Y_GREATER_DZ) && (X < X_LESS_DZ) && transition_speed < 2000){
    transition_speed = transition_speed + 10;
    return true;
  }

  // Decrease Speed
  else if ((Y > Y_LESS_DZ && Y < Y_GREATER_DZ) && (X > X_GREATER_DZ) && transition_speed > 10) {
    transition_speed = transition_speed - 10;
    return true;
  }

  return false;
}

void colour_fader(int new_red, int new_green, int new_blue){
    while ((new_red != glob_red || new_green != glob_green || new_blue != glob_blue) && colour_mode == 2 && show_light == true) {
      // change_brightness_analog();
      // change_transition_speed();
      
      // Serial.print("\n1 ");
      // Serial.print(transition_speed);

      if (new_red > glob_red){
        ++glob_red;
      }
      else if (new_red < glob_red){
        --glob_red;
      }
      if (new_green > glob_green){
        ++glob_green;
      }
      else if (new_green < glob_green){
        --glob_green;
      }
      if (new_blue > glob_blue){
        ++glob_blue;
      }
      else if (new_blue < glob_blue){
        --glob_blue;
      }
      change_all_colour(glob_red, glob_green, glob_blue);
      for (int i = 0; i < transition_speed; i += 5){
        
        
        // change_transition_speed();

        // Serial.print("\n2  ");
        // Serial.print(transition_speed);

        light_switch();
        change_brightness_analog();
        if (change_transition_speed()) {
          break;
        }
        
        if (joystick_value == 0) {
          colour_mode = 1;
          delay(1000);
          return;
        }
        FastLED.show();
        delay(5);
      }
      // delay(transition_speed);
      joystick_value = digitalRead(joystick_pin);
      if (joystick_value == 0) {
        colour_mode = 1;
        delay(1000);
        return;
    }
    }
    // Button is clicked

}


void light_switch() {
  photoresistor_value = analogRead(photoresistor_pin);
  joystick_value = digitalRead(joystick_pin);
    // Light detects ON for the first time and light is not showing
  if (photoresistor_value > photoresistor_threshold && show_light == false && turned_off == true) {
    show_light = true;
    turned_off = false;
    Serial.println("\n1\n");
    delay(1000);
  }
  
  // Light detects OFF for the first time since on
  else if (photoresistor_value < photoresistor_threshold && turned_off == false) {
    turned_off = true;
    Serial.println("\n2\n");
    delay(1000);
  }

  // Light turns ON for the first time since off but there is already light showing
  else if (photoresistor_value > photoresistor_threshold && show_light == true && turned_off == true) {
    show_light = false;
    turned_off = false;
    Serial.println("\n3\n");
    delay(1000);
  }
}

