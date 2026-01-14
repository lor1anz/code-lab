import pandas as pd
import numpy as np

from sklearn.datasets import make_regression
from sklearn.model_selection import train_test_split

from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score


n_features = 10
n_samples = 20_000
random_state = 42
noise = 50

X, y = make_regression(n_samples = n_samples, n_features = n_features, random_state = random_state, noise = noise)

print("Data shape:")
print(X.shape, y.shape)
print ("")

data = pd.DataFrame(X, columns=[f'feature_{k}' for k in range(n_features)])
data['target'] = y
print("Dataset:")
print(data)
print ("")

test_size = 0.3

x_train, x_test, y_train, y_test = train_test_split(data.drop(['target'], axis = 1), data['target'], test_size = test_size, random_state = random_state)

lgr = LinearRegression()
lgr.fit(x_train, y_train)
print("Result:")
print("r2_score: " + f'{r2_score(y_test, lgr.predict(x_test)):0.2f}')

