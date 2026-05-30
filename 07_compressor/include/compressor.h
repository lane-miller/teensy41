#pragma once
#include <Arduino.h>
#include <Audio.h>

class AudioEffectCompressor : public AudioStream {
public:
    AudioEffectCompressor() : AudioStream(1, inputQueueArray) {
        setThreshold(-20.0f);
        setRatio(4.0f);
        setAttack(10.0f);
        setRelease(100.0f);
        _envelope = 0.0f;
        _gain = 1.0f;
    }

    void setThreshold(float dB) { _threshold = powf(10.0f, dB / 20.0f); }
    void setRatio(float ratio)  { _ratio = ratio; }

    void setAttack(float ms) {
        _attack = expf(-1.0f / (ms * 0.001f * AUDIO_SAMPLE_RATE));
    }
    void setRelease(float ms) {
        _release = expf(-1.0f / (ms * 0.001f * AUDIO_SAMPLE_RATE));
    }

    float getGainReductiondB() { return 20.0f * log10f(_gain + 1e-9f); }

    void update(void) override {
        audio_block_t *block = receiveWritable(0);
        if (!block) return;

        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            float sample = block->data[i] / 32768.0f;
            float rectified = fabsf(sample);

            // Envelope follower
            float coeff = (rectified > _envelope) ? _attack : _release;
            _envelope = coeff * _envelope + (1.0f - coeff) * rectified;

            // Gain computation
            if (_envelope > _threshold) {
                float overDb = 20.0f * log10f(_envelope / _threshold);
                float reducedDb = overDb - (overDb / _ratio);
                _gain = powf(10.0f, -reducedDb / 20.0f);
            } else {
                _gain = 1.0f;
            }

            // Apply gain
            float out = sample * _gain;
            out = fmaxf(-1.0f, fminf(1.0f, out));
            block->data[i] = (int16_t)(out * 32767.0f);
        }

        transmit(block);
        release(block);
    }

private:
    audio_block_t *inputQueueArray[1];
    float _threshold, _ratio, _attack, _release;
    float _envelope, _gain;
};