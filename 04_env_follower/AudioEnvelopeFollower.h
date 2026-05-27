// AudioEnvelopeFollower.h

// Define guard to prevent multiple inclusion
#ifndef AUDIO_ENVELOPE_FOLLOWER_H
#define AUDIO_ENVELOPE_FOLLOWER_H

// Include necessary headers
#include <Arduino.h>
#include <Audio.h>

// Define the AudioEnvelopeFollower class
class AudioEnvelopeFollower : public AudioStream {
public:
    AudioEnvelopeFollower();
    void setAttack(float ms);
    void setRelease(float ms);
    float getLevel();
    virtual void update(void);

private:
    audio_block_t *inputQueueArray[1];
    float envelope;
    float attackCoeff;
    float releaseCoeff;
};

#endif