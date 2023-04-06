#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <NewPing.h>

// define the pins
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define NOTE_OFFSET  20  // Offset for the note frequency calculation

// initialize the distance sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, 200);

// initialize the audio library
AudioClass& Audio = AudioClass::getInstance();
AudioSynthWaveformSine wave = AudioSynthWaveformSine();
AudioOutputI2S i2s = AudioOutputI2S();
AudioConnection patchCord1(wave, 0, i2s, 0);
AudioConnection patchCord2(wave, 0, i2s, 1);

// initialize the variables
int distance = 0;
int note = 0;
float amplitude = 0.0;

void setup() {
  // initialize the serial connection
  Serial.begin(9600);

  // initialize the audio library
  Audio.begin(44100, 16);

  // set the wave frequency to 440 Hz
  wave.frequency(440);

  // start playing the sound
  wave.begin();
}

void loop() {
  // get the distance from the sensor
  distance = sonar.ping_cm();

  // calculate the note frequency based on the distance
  note = map(distance, 0, 100, 0, 127);
  note = note + NOTE_OFFSET;

  // set the wave frequency to the note frequency
  wave.frequency(midiNoteToFrequency(note));

  // calculate the amplitude based on the distance
  amplitude = map(distance, 0, 100, 0, 1);

  // fade the amplitude in and out
  for (float i = 0; i <= 1; i += 0.01) {
    wave.amplitude(i * amplitude);
    delay(10);
  }
  for (float i = 1; i >= 0; i -= 0.01) {
    wave.amplitude(i * amplitude);
    delay(10);
  }

  // print the distance and note frequency to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Note: ");
  Serial.println(note);

  // delay for a short period
  delay(50);
}
