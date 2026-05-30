# 07_compressor

Dynamic range compressor implemented as a custom `AudioStream` subclass on Teensy 4.1.

Receives audio over USB, applies compression, returns audio over USB. Parameters:
threshold (-20 dB), ratio (4:1), attack (10 ms), release (100 ms).

Prints gain reduction (dB), CPU usage (%), and peak memory usage (blocks) over Serial
at 115200 baud every 100ms.

## Notes
- USB mode: USB_MIDI_AUDIO_SERIAL (required for simultaneous Serial + Audio in PlatformIO)
- AudioMemory(16) allocated; peak observed usage 4 blocks