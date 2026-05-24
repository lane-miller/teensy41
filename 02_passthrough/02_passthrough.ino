#include <Audio.h>

AudioInputUSB     usbIn;
AudioOutputUSB    usbOut;

AudioConnection   patchL(usbIn, 0, usbOut, 0);
AudioConnection   patchR(usbIn, 0, usbOut, 1);

void setup() {
    AudioMemory(12);
}

void loop() {
    Serial.println(AudioProcessorUsage());
    delay(1000);
}