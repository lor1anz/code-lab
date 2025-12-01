import numpy as np

w = [1, 2, 3, 4]

def calculate_threshold(x):
    return np.dot(x, w)

def classify(x):
    if calculate_threshold(x) < 0:
        print("1 class")
    else:
        print("2 class")

x1 = [-1, 0, 1, -1]
x2 = [1, 0, 1, -1]
x3 = [1, 1, 1, 0]
x4 = [0, -1, -1, 1]
x5 = [1, 1, -1, -1]

classify(x1)
classify(x2)
classify(x3)
classify(x4)
classify(x5)
