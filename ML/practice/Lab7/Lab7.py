import numpy as np
import pandas as pd
from libsvm.python.commonutil import svm_read_problem
from libsvm.python import svmutil
import os
from tqdm import tqdm
import json


def preprocess_and_scale(input_file, save_dir):
    os.makedirs(save_dir, exist_ok=True)
    data = pd.read_csv(input_file, header=None).values
    train_data, test_data = np.vsplit(data, [3450])

    train_data_path = os.path.join(save_dir, 'train.csv')
    test_data_path = os.path.join(save_dir, 'test.csv')

    preprocess_data(train_data, train_data_path)
    preprocess_data(test_data, test_data_path)

    scaled_train_path = os.path.join(save_dir, 'train_scaled.csv')
    scaled_test_path = os.path.join(save_dir, 'test_scaled.csv')
    svm_scale_script = r'.\libsvm\windows\svm-scale.exe'
    os.system(f'{svm_scale_script} -l 0 -u 1 -s params {train_data_path} > {scaled_train_path}')
    os.system(f'{svm_scale_script} -r params {test_data_path} > {scaled_test_path}')

    return scaled_train_path, scaled_test_path


def preprocess_data(raw_data, filename):
    x = raw_data[:, :-1]
    y = raw_data[:, -1].astype(int)
    lines = []
    for features, label in zip(x, y):
        sparse_features = [f"{i + 1}:{value}" for i, value in enumerate(features) if value != 0]
        line = ' '.join([str(label)] + sparse_features) + '\n'
        lines.append(line)
    with open(filename, 'w') as f:
        f.writelines(lines)


def train_predict(y_train: np.array, x_train: np.array, y_test: np.array, x_test: np.array,
                  degree: int = 3, c: int = 2):
    model = svmutil.svm_train(y_train, x_train, f'-r 2 -t 1 -d {degree} -c {c} -q')
    p_label, p_acc, p_val = svmutil.svm_predict(y_test, x_test, model, '-q')
    acc, mse, _ = p_acc
    sv_num = model.l
    return acc, mse, sv_num


def kfold(x_train: np.array, y_train: np.array, k: int = 10):
    x_split = np.vsplit(x_train, k)
    y_split = np.split(y_train, k)
    folds = []
    for i in range(k):
        x_val, y_val = x_split[i], y_split[i]
        x_t = np.vstack([x for j, x in enumerate(x_split) if j != i])
        y_t = np.hstack([x for j, x in enumerate(y_split) if j != i])
        folds.append((x_t, y_t, x_val, y_val))
    return folds


def search_parameters(x_train: np.array, y_train: np.array, degree_range: range, c_range: list, n_folds=10):
    results = []
    min_e = 100000
    min_c = 100
    min_d = 100
    folds = kfold(x_train, y_train, n_folds)
    for d in degree_range:
        res = {
            'd': d,
            'res': []
        }
        for c in tqdm(c_range):
            errors = []
            for x_t, y_t, x_val, y_val in folds:
                acc, _, _ = train_predict(y_t, x_t, y_val, x_val, degree=d, c=c)
                errors.append(100 - acc)
            e = np.mean(errors)
            if e < min_e:
                min_e = e
                min_c = c
                min_d = d
            res['res'].append({'c': c, 'err': e, 'std': np.std(errors)})
        results.append(res)
    print(f'min cv error: {min_e}, C: {min_c}, d: {min_d}')
    return min_c, min_d, results


def spam_task():
    save_dir = 'results'
    train_scaled_path, test_scaled_path = preprocess_and_scale('spambase.csv', save_dir)

    y_train, x_train = svm_read_problem(train_scaled_path, return_scipy=True)
    y_test, x_test = svm_read_problem(test_scaled_path, return_scipy=True)

    deg = 20
    c_range = [2 ** i for i in range(-deg, (deg + 1))]
    deg_range = range(1, 5)
    best_c, best_d, cv_results = search_parameters(x_train.todense(), y_train, degree_range=deg_range, c_range=c_range)

    with open(os.path.join(save_dir, 'cv_results.json'), 'w') as f:
        json.dump(cv_results, f, indent=2)

    test_res = []
    for d in deg_range:
        acc, _, num_sv = train_predict(y_train, x_train, y_test, x_test, degree=d, c=best_c)
        test_res.append({'d': d, 'c': best_c, 'err': 100 - acc, 'n_sv': num_sv})

    test_results_path = os.path.join(save_dir, 'test_results.json')
    with open(test_results_path, 'w') as f:
        json.dump(test_res, f, indent=2)


if __name__ == "__main__":
    spam_task()
