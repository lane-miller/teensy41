# 06_tflite_sinx

TensorFlow Lite inference pipeline on Teensy 4.1
A small dense neural network trained in Python (Keras) to predict sin(2πx),
exported as a float32 TFLite model, converted to a C header via xxd, and
deployed to Teensy using the Arduino_TensorFlowLite_t4 library

Prints x, predicted sin(x), and actual sin(x) over Serial for comparison.

## Notes
- Int8 quantization was attempted but degraded this small model significantly;
  float32 TFLite used instead