#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <NewPing.h>

// define the pins
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define NOTE_OFFSET  20


NewPing sonar(TRIGGER_PIN, ECHO_PIN, 200);


AudioClass& Audio = AudioClass::getInstance();
AudioSynthWaveformSine wave = AudioSynthWaveformSine();
AudioOutputI2S i2s = AudioOutputI2S();
AudioConnection patchCord1(wave, 0, i2s, 0);
AudioConnection patchCord2(wave, 0, i2s, 1);


int distance = 0;
int note = 0;
float amplitude = 0.0;

void setup() {

  Serial.begin(9600);
  Audio.begin(44100, 16);
  wave.frequency(440);

  wave.begin();
}

void loop() {
 
  distance = sonar.ping_cm();

  note = map(distance, 0, 100, 0, 127);
  note = note + NOTE_OFFSET;


  wave.frequency(midiNoteToFrequency(note));

  amplitude = map(distance, 0, 100, 0, 1);


  for (float i = 0; i <= 1; i += 0.01) {
    wave.amplitude(i * amplitude);
    delay(10);
  }
  for (float i = 1; i >= 0; i -= 0.01) {
    wave.amplitude(i * amplitude);
    delay(10);
  }

  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Note: ");
  Serial.println(note);

  delay(50);
}
