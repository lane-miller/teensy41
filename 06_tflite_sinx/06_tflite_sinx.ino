#include <Arduino.h>
#include "TensorFlowLite.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "sine_model.h"

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

// Tensor arena — working memory for TFLite Micro
constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

// TFLite Micro objects
const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    model = tflite::GetModel(sine_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        while (1);
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println("AllocateTensors failed!");
        while (1);
    }

    input  = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("Model ready.");
}

void loop() {
    for (int i = 0; i <= 100; i++) {
        float x = i / 100.0f;

        input->data.f[0] = x;

        if (interpreter->Invoke() != kTfLiteOk) {
            Serial.println("Invoke failed!");
            return;
        }

        float y_pred   = output->data.f[0];
        float y_actual = sinf(2.0f * M_PI * x);

        Serial.print(x, 3);
        Serial.print("\t");
        Serial.print(y_pred, 4);
        Serial.print("\t");
        Serial.println(y_actual, 4);

        delay(50);
    }
    delay(2000);
}
