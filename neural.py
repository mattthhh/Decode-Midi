import glob
from sklearn.model_selection import train_test_split
import numpy as np
import tensorflow as tf
from sklearn.preprocessing import LabelEncoder

# import data
data_path = "data/classique-*song.txt"
files = glob.glob(data_path)
data = []
for file in files:
    with open(file, "r") as f:
        data.append(f.read())

data_path = "data/pop-*song.txt"
files = glob.glob(data_path)
for file in files:
    with open(file, "r") as f:
        data.append(f.read())

data_path = "data/classique-*cat.txt"
files = glob.glob(data_path)
labels = []
for file in files:
    with open(file, "r") as f:
        labels.append(f.read())

data_path = "data/pop-*cat.txt"
files = glob.glob(data_path)
for file in files:
    with open(file, "r") as f:
        labels.append(f.read())

# lebels encoding with integers
label_encoder = LabelEncoder()
label_encoder.fit(labels)
labels = label_encoder.transform(labels)

# tokenization
tokenizer = tf.keras.preprocessing.text.Tokenizer()
tokenizer.fit_on_texts(data)
sequences = tokenizer.texts_to_sequences(data)
max_sequence_length = max([len(seq) for seq in sequences])
padded_sequences = tf.keras.preprocessing.sequence.pad_sequences(sequences, maxlen=max_sequence_length, padding="post")

# split data 80% train, 20% test
train_sequences, test_sequences, train_labels, test_labels = train_test_split(padded_sequences, labels, test_size=0.2)

print(train_sequences.shape)
print(test_sequences.shape)

# CNN Model
model = tf.keras.Sequential([
    tf.keras.layers.Embedding(input_dim=len(tokenizer.word_index) + 1, output_dim=128, input_length=max_sequence_length),
    tf.keras.layers.Conv1D(128, 5, activation="relu"),
    tf.keras.layers.GlobalMaxPooling1D(),
    tf.keras.layers.Conv1D(64, activation="relu"),
    tf.keras.layers.GlobalMaxPooling1D(),
    tf.keras.layers.Dense(32, activation="relu"),
    tf.keras.layers.Dense(1, activation="sigmoid")
])

model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])
model.fit(train_sequences, train_labels, epochs=30, batch_size=32, validation_data=(test_sequences, test_labels))
test_loss, test_acc = model.evaluate(test_sequences, test_labels)
print("Test Loss: ", test_loss)
print("Test Accuracy: ", test_acc)
