#include <Audio.h>
#include "AudioEnvelopeFollower.h"

AudioInputUSB          usbIn;
AudioEnvelopeFollower  follower;
AudioOutputUSB         usbOut;

AudioConnection        p1(usbIn, 0, follower, 0);
AudioConnection        p2(follower, 0, usbOut, 0); 
AudioConnection        p3(follower, 0, usbOut, 1); 

void setup() {
    AudioMemory(16);
    Serial.begin(9600);
    follower.setAttack(10.0f);
    follower.setRelease(100.0f);
}

void loop() {
    Serial.println(follower.getLevel());
    delay(100);
}