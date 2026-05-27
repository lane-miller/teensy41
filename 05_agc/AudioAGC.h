// AudioAGC.h

// Define guard to prevent multiple inclusion
#ifndef AUDIO_AGC_H
#define AUDIO_AGC_H

// Include necessary headers
#include <Arduino.h>
#include <Audio.h>

// Define the AudioAGC class
class AudioAGC : public AudioStream {
public:
    AudioAGC();
    void setTargetLevel(float target);
    void setMaxGain(float maxGain);
    void setEnvelopeSource(float *envelopePtr);
    virtual void update(void);
    float getCurrentGain();

private:
    audio_block_t *inputQueueArray[1];
    float targetLevel;
    float maxGain;
    float *envelopePtr;
    float currentGain;
};

#endif