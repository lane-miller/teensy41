#include <Audio.h>

AudioInputUSB       usbIn;
AudioFilterBiquad   lpf;
AudioOutputUSB      usbOut;

AudioConnection     p1(usbIn, 0, lpf, 0);
AudioConnection     p2(lpf, 0, usbOut, 0);
AudioConnection     p3(lpf, 0, usbOut, 1);

void setup() {
    AudioMemory(16);
    Serial.begin(9600);
    lpf.setLowpass(0, 20000, 0.707);
}

void loop() {
    if (Serial.available()) {
        int freq = Serial.parseInt();
        if (freq > 20 && freq < 20000) {
            lpf.setLowpass(0, freq, 0.707);
            Serial.print("Cutoff → ");
            Serial.println(freq);
        }
    }
}