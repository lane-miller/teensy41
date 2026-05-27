// AudioEnvelopeFollower.cpp

// Include header files
#include "AudioEnvelopeFollower.h"
#include <cmath>

// Constructor
AudioEnvelopeFollower::AudioEnvelopeFollower() : AudioStream(1, inputQueueArray) {
    envelope = 0.0f;
    setAttack(10.0f);
    setRelease(100.0f);
}

// Set attack time in milliseconds
void AudioEnvelopeFollower::setAttack(float ms) {
    attackCoeff = 1.0f - expf(-1.0f / (AUDIO_SAMPLE_RATE_EXACT * ms / 1000.0f));
}

// Set release time in milliseconds
void AudioEnvelopeFollower::setRelease(float ms) {
    releaseCoeff = 1.0f - expf(-1.0f / (AUDIO_SAMPLE_RATE_EXACT * ms / 1000.0f));
}

// Get the current envelope level
float AudioEnvelopeFollower::getLevel() {
    return envelope;
}

// Update the envelope level
void AudioEnvelopeFollower::update(void) {
    audio_block_t *block = receiveReadOnly(0);
    if (!block) return;

    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float sample = block->data[i] / 32768.0f;
        float magnitude = fabsf(sample);
        float coeff = (magnitude > envelope) ? attackCoeff : releaseCoeff;
        envelope += coeff * (magnitude - envelope);
    }

    transmit(block);
    release(block);
}