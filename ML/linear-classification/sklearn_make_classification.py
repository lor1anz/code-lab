import pandas as pd
import numpy as np

from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split

from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_auc_score


n_features = 10
n_samples = 20_000
random_state = 42

X, y = make_classification(n_samples = n_samples, n_features = n_features, random_state = random_state, n_redundant = 0)

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

lgr = LogisticRegression()
lgr.fit(x_train, y_train)
print("Result:")
print("roc_auc: " + f'{roc_auc_score(y_test, lgr.predict(x_test)):0.2f}')

