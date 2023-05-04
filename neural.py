import glob
from sklearn.model_selection import train_test_split

data_path = "data/*song.txt"
files = glob.glob(data_path)
data = []
for file in files:
    with open(file, "r") as f:
        data.append(f.read())

data_path = "data/*cat.txt"
files = glob.glob(data_path)
labels = []
for file in files:
    with open(file, "r") as f:
        labels.append(f.read())

X_train_val, X_test, y_train_val, y_test = train_test_split(data, labels, test_size=0.2, random_state=42, stratify=labels)
X_train, X_val, y_train, y_val = train_test_split(X_train_val, y_train_val, test_size=0.2, random_state=42, stratify=y_train_val)
