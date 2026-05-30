#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <compressor.h>

AudioInputUSB          usbIn;
AudioEffectCompressor  compressor;
AudioOutputUSB         usbOut;

AudioConnection patchIn0(usbIn, 0, compressor, 0);
AudioConnection patchOut0(compressor, 0, usbOut, 0);
AudioConnection patchOut1(compressor, 0, usbOut, 1);

void setup() {
    Serial.begin(115200);
    AudioMemory(16);
}

void loop() {
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 250) {
        lastPrint = millis();
        Serial.print("GR: ");
        Serial.print(compressor.getGainReductiondB(), 1);
        Serial.print(" dB  CPU: ");
        Serial.print(AudioProcessorUsage(), 1);
        Serial.print("%  Mem: ");
        Serial.println(AudioMemoryUsageMax());
    }
}