// AUTHORS: Jennifer Cho, Aoi Yamamoto, Lily Westort
// FILE: ArduinoCode.ino
// DATE: 12/10/17

//Call library for NeoPixel Ring
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 5 // LED PIN

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16

// NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



///___VARIABLES___///
//  Pulse variables
int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.

// Array variables
const int numReadings = 100;   // number of elements in the array != index
int readings6[numReadings];
int readIndex = 0;
bool arrayA = true; // Used to indicate whether playback is finished

// LED variables
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
int two =  8;
int one = 9;

// Record pins
const int REC = 10, PLAYE = 11;

// Indicates wheter button one or two was pressed
bool wasOnePressed = false;
bool wasTwoPressed = false;

///___VARIABLES ENDS___///



///___AUDIO___///
// Record audio
void record() {
  Serial.println("Recording for 10 seconds...");
  digitalWrite(REC, HIGH);
  delay(10000);
  digitalWrite(REC, LOW);
}

// Plays back audio once
void play_e() {
  Serial.println("Now playing...");
  digitalWrite(PLAYE, HIGH);
  delay(500);
  digitalWrite(PLAYE, LOW);
}
///___AUDIO ENDS___///



///___SET UP___///
void setup() {
  // Set up buttons 1 and 2
  pinMode(one, INPUT_PULLUP);
  pinMode(two, INPUT_PULLUP);
  digitalWrite(one, HIGH);
  digitalWrite(two, HIGH);

  // Set up LED
  pinMode(PIN, OUTPUT);
  pixels.show();
  pixels.begin(); // This initializes the NeoPixel library.

  Serial.begin(9600);         // Set's up Serial Communication at certain speed.

}
///___SET UP ENDS___///



///___LOOP____///
void loop() {
  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.

  // If one is pressed
  if (digitalRead(one) == LOW) {
    wasOnePressed = true;
    Serial.println("One");
    record();

    // Begin recording audio

    // If two is pressed
  } else if (digitalRead(two) == LOW) {
    wasTwoPressed = true;
    Serial.println("Two");

    // Playback recording
    play_e();
  }


  ///___ LED___///
  // If array is not yet filled with data and one was pressed
  if (readIndex < numReadings & wasOnePressed == true) {

    // Save pulse data into the array
    readings6[readIndex] = Signal;
    readIndex++;
    Serial.println(Signal);
    Serial.println("\n");

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setBrightness(brightness);

    }

    // When Signal goes over threshold, PIN gets the most bright input which is 255
    // otherwise it is brightness
    if (Signal > Threshold) {
      pixels.setBrightness(255);
      for (int x = 0; x < NUMPIXELS; x++) { // Initialize all pixels to 'off'
        pixels.setPixelColor(x, pixels.Color(255, 255, 255));
      }
    }

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 70) {
      fadeAmount = -fadeAmount;
    }

    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  ///___LED ENDS___///


  ///___PULSE PLAYBACK___///
  // If array is filled with data and replay button was played, loop through array of
  // pulse readings and show pulsing effect
  if (readIndex == numReadings & wasTwoPressed == true && arrayA) {

    // Loop through array
    for (int i = 0; i < numReadings; i++) {

      // Set the brightness of the pixels
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setBrightness(brightness);

      }

      // Store individual pulse reading into this variable
      int pulseReading = readings6[i];

      // If pulse is high
      if (pulseReading > Threshold) {
        // Set to max brightness
        pixels.setBrightness(255);

        // Set pixels to read color (set to red for testing purposes, but change later)
        for (int x = 0; x < NUMPIXELS; x++) {
          pixels.setPixelColor(x, pixels.Color(255, 0, 0));
        }
      }

      // Change the brightness for next time through the loop:
      brightness = brightness + fadeAmount;

      // Reverse the direction of the fading at the ends of the fade:
      if (brightness <= 0 || brightness >= 70) {
        fadeAmount = -fadeAmount;
      }

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(100);  // delay ...?  1000 = 1 sec

    }

    // Indicates of array has been done being read
    arrayA = false;

  }

  // If audio and pulse was already once recorded and played back,
  // reset values so user can reading and playback again
  if (wasOnePressed == true && wasTwoPressed == true && arrayA == false) {
    wasOnePressed = false;
    wasTwoPressed = false;

    readIndex = 0;
    arrayA = true;

  }
  ///___PULSE ENDS___///

  delay(100);  // delay: 1000 = 1 sec

} ///___LOOP ENDS___///
