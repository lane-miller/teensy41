# train_sinx_model.py

# Import packages
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

# Training data: x in [0, 1] maps to one full sine period
N = 1000
x = np.linspace(0, 1, N)
y = np.sin(2 * np.pi * x)

# Shuffle
idx = np.random.permutation(N)
x = x[idx]
y = y[idx]

# Split 80/10/10 for train/val/test
N_train = int(N * 0.8)
N_val = int(N * 0.1)
N_test = N - N_train - N_val
x_train = x[:N_train].reshape(-1, 1)
y_train = y[:N_train]
x_val = x[N_train:N_train + N_val].reshape(-1, 1)
y_val = y[N_train:N_train + N_val]
x_test = x[N_train + N_val:].reshape(-1, 1)
y_test = y[N_train + N_val:]

# Define model
model = tf.keras.Sequential([
    tf.keras.layers.Dense(32, input_shape=(1,), activation='relu'),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(1),
])

# Compile model
model.compile(
    optimizer=tf.keras.optimizers.Adam(learning_rate=1e-3),
    loss='mae',
)

# Train model
history = model.fit(
    x_train, y_train,
    epochs=500,
    validation_data=(x_val, y_val),
    batch_size=32,
    callbacks=[
        tf.keras.callbacks.EarlyStopping(
            monitor='val_loss',
            patience=30,
            restore_best_weights=True,
        ),
    ],
    verbose=1,
)

# Plot training and validation loss
fig, ax = plt.subplots(figsize=(10, 5))
ax.plot(history.history['loss'], label='Training Loss')
ax.plot(history.history['val_loss'], label='Validation Loss')
ax.legend()
ax.set_title('Training and Validation Loss')
ax.set_xlabel('Epoch')
ax.set_ylabel('Loss')

# Evaluate model on test data
loss = model.evaluate(x_test, y_test, verbose=0)
print(f'Test MAE: {loss:.5f}')

# Plot comparing model predictions vs actual test data
order = np.argsort(x_test.flatten())
x_sorted = x_test.flatten()[order]
y_sorted = y_test[order]
pred_sorted = model.predict(x_test, verbose=0).flatten()[order]

fig, ax = plt.subplots(figsize=(10, 5))
ax.plot(x_sorted, pred_sorted, label='Predicted')
ax.plot(x_sorted, y_sorted, label='Actual')
ax.set_title('Model Predictions vs Actual Test Data')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.legend()
plt.show()

converter = tf.lite.TFLiteConverter.from_keras_model(model)
# No optimizations — pure float32
tflite_model = converter.convert()

# Save .tflite file
with open('sine_model.tflite', 'wb') as f:
    f.write(tflite_model)

print(f'Model size: {len(tflite_model)} bytes')

# Convert to C header
import subprocess
subprocess.run(['xxd', '-i', 'sine_model.tflite', 'sine_model.h'])
print('sine_model.h generated')
