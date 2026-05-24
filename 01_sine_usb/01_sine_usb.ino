#include <Audio.h>

AudioSynthWaveformSine sine;
AudioOutputUSB         usbOut;
AudioConnection        patchL(sine, 0, usbOut, 0);
AudioConnection        patchR(sine, 0, usbOut, 1);

void setup() {
    AudioMemory(12);
    sine.frequency(440);
    sine.amplitude(0.5);
}

void loop() {
    Serial.println(AudioProcessorUsage());
    delay(1000);
}