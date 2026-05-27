// 05_agc.ino


// Include necessary headers
#include <Audio.h>
#include "AudioEnvelopeFollower.h"
#include "AudioAGC.h"

// Define the audio components
AudioInputUSB          usbIn;
AudioEnvelopeFollower  follower;
AudioAGC               agc;
AudioOutputUSB         usbOut;

// Define the audio connections
AudioConnection        p1(usbIn, 0, follower, 0);
AudioConnection        p2(follower, 0, agc, 0);
AudioConnection        p3(agc, 0, usbOut, 0);
AudioConnection        p4(agc, 0, usbOut, 1);

// Setup the audio system
void setup() {
    AudioMemory(16);
    Serial.begin(9600);
    follower.setAttack(10.0f); // Set the attack time
    follower.setRelease(100.0f); // Set the release time
    agc.setTargetLevel(0.25f); // Set the target level
    agc.setMaxGain(8.0f); // Set the maximum gain
    agc.setEnvelopeSource(follower.getEnvelopePtr()); // Set the envelope source
}

// Main loop
void loop() {
    Serial.print("Envelope: ");
    Serial.print(follower.getLevel());
    Serial.print("  Gain: ");
    Serial.println(agc.getCurrentGain());
    delay(100);
}