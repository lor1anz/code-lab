import numpy as np

w = np.array([1.0, 2.0, 3.0, 2.0, 1.0])

x_train = np.array([
    [1, 2, 3, 4, 5],
    [5, 4, 3, 2, 1],
    [1, 1, 1, 1, 1],
    [1, 0, 1, 0, 1],
    [0, 1, 0, 1, 0]
])

y_train = np.array([1, -1, 1, -1, 1])


def f(x):
    return 1 if np.dot(x, w) > 0 else -1

def loss(x, y):
    return max(0, (1 - y * f(x)))

def der_loss(x, y):
    return -1.0 if 1 - y * f(x) > 0 else 0.0


print("Before train:")

y_before_train = []
for x in x_train:
    y_before_train.append(f(x))

print(y_before_train)

print("Weight vector before:")
print(w)


def fit(x_train_, y_train_):
    global w
    for k in range(100):
        step = 0.01
        term = np.array([0.0, 0.0, 0.0, 0.0, 0.0])
        for i in range(len(x_train_)):
            term += y_train_[i] * x_train_[i] * der_loss(x_train_[i], y_train_[i])
        w -= step * term


fit(x_train, y_train)

print("After train:")

y_after_train = []
for x in x_train:
    y_after_train.append(f(x))

print(y_after_train)

print("Ground truth:")
print(y_train)

print("Weight vector after:")
print(w)






