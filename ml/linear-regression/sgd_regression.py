import pandas as pd
import numpy as np

from sklearn.model_selection import train_test_split
from sklearn.datasets import make_regression
from sklearn.metrics import r2_score
from sklearn.linear_model import LinearRegression

import sys
EPS = sys.float_info.epsilon

n_features = 5
n_samples = 20_000
random_state = 42
noise = 50 
test_size = 0.3

X, y, coef = make_regression(n_samples = n_samples, n_features = n_features, random_state = random_state, noise = noise, coef = True)
data = pd.DataFrame(X, columns = [f'feature_{k}' for k in range(n_features)])
data['target'] = y

x_train, x_test, y_train, y_test = train_test_split(data.drop(['target'], axis = 1), data['target'], test_size = test_size, random_state = random_state)


rng = np.random.default_rng(seed = 42)

class sgd_linear:
  def __init__(self, rng, MAX_ITER = 100_000):
    self.MAX_ITER = MAX_ITER
    self.w = None
    self.w0 = rng.normal()
    return

  def _f(self, x):
    assert len(x) == len(self.w)
    return np.dot(self.w, x) + self.w0

  def _der_loss(self, x, y, loss):
    if loss == 'MSE':
      return -(y - self._f(x)) * x

  def fit(self, X_train, y_train, loss = 'MSE'):
    self.w = rng.normal(size=X_train.shape[1])
    step = 0.01
    for k in range(self.MAX_ITER):
      rand_index = rng.integers(0, X_train.shape[0] - 1)
      x = np.array(X_train)[rand_index]
      y = np.array(y_train)[rand_index]
      if k % 10000 == 0:
         step = step / 2
      self.partial_fit(x, y, step, loss)

  def partial_fit(self, x, y, step = 0.01, loss = 'MSE'):
    if not self.w.any():
      self.w = rng.normal(size=len(x))
    dl = self._der_loss(x, y, loss)
    self.w -= step * dl
    self.w0 -= - step * (y - self._f(x))



sgd = sgd_linear(rng)
sgd.fit(x_train, y_train)
print("My sgd:")
print(sgd.w)
print("")


lr = LinearRegression()
lr.fit(np.array(x_train), np.array(y_train))
print("Sklearn lr:")
print(lr.coef_)
print("")


print("GT:")
print(coef)
print("")






