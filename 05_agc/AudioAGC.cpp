// AudioAGC.cpp

// Include header files
#include "AudioAGC.h"
#include <cmath>

// Constructor
AudioAGC::AudioAGC() : AudioStream(1, inputQueueArray) {
    targetLevel = 0.25f;
    maxGain = 8.0f;
    envelopePtr = nullptr;
}

// Set the target level
void AudioAGC::setTargetLevel(float target) {
    targetLevel = target;
}

// Set the maximum gain
void AudioAGC::setMaxGain(float maxGain_) {
    maxGain = maxGain_;
}

// Set the envelope source
void AudioAGC::setEnvelopeSource(float *ptr) {
    envelopePtr = ptr;
}

// Update the AGC
void AudioAGC::update(void) {
    audio_block_t *block = receiveReadOnly(0);
    if (!block) return; // If no block is received, release the block and return
    if (!envelopePtr) { release(block); return; } // If no envelope source is set, release the block and return

    audio_block_t *outBlock = allocate(); // Allocate a new block for the output
    if (!outBlock) { release(block); return; } // If no block is allocated, release the block and return

    float envelope = *envelopePtr; // Get the envelope level
    currentGain = (envelope > 0.0001f) ? targetLevel / envelope : maxGain;
    if (currentGain > maxGain) currentGain = maxGain; // If the current gain is greater than the maximum gain, set the current gain to the maximum gain
    
    // Apply the gain to the input block and store the result in the output block
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float sample = block->data[i] / 32768.0f;
        sample *= currentGain;
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
        outBlock->data[i] = (int16_t)(sample * 32767.0f);
    }

    transmit(outBlock);
    release(outBlock);
    release(block);
}

// Get the current gain
float AudioAGC::getCurrentGain() {
    return currentGain;
}