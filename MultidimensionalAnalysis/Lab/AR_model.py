from statistics import mean
import numpy
import matplotlib.pyplot as plt


def get_partial_covariance(y_s, n):
    ro_vector = numpy.array([y_s[i]/y_s[0] for i in range(0, n+1)])  # n+1 elements
    matrix = numpy.array([[ro_vector[abs(j-i)] for i in range(0, n)] for j in range(0, n)])
    answer_vector = numpy.linalg.solve(matrix, ro_vector[1:])
    return answer_vector[n-1]


def generate_x_axe(y):
    return [x+1 for x in range(0, len(y))]


if __name__ == '__main__':
    N = 10
    p = 1
    a = 0.99
    white_noise = numpy.random.normal(0, 1, size=100)  # N elements in the white_noise
    data = [0]
    for i in range(1, N):  # N elements in the data
        data.append(white_noise[i] - a*data[i-1])
    # print(data)
    s_max = int(N/3)
    sigma_2 = mean([x*x for x in white_noise])
    covariance = [sigma_2]
    for i in range(1, s_max):  # N/3 elements in the covariance vector
        covariance.append(-a*covariance[i-1])
    print(covariance)

    covariance_est = []
    for s in range(0, s_max):
        covariance_est.append((1/(N-s))*(sum([data[t]*data[t-s] for t in range(s+1, N)])))
    print(covariance_est)

    partial_covariance = [get_partial_covariance(covariance, n) for n in range(1, N-1)]
    # print(partial_covariance)
    partial_covariance_est = [get_partial_covariance(covariance_est, n) for n in range(1, N-1)]
    # print(partial_covariance_est)

    fig, ax = plt.subplots()
    ax.plot(generate_x_axe(covariance), covariance, label='Covariance', color='blue')
    ax.set_xlabel('time (t)')
    ax.set_ylabel('value')

    ax.plot(generate_x_axe(partial_covariance), partial_covariance, label='Partial covariance', color='orange')
    legend = ax.legend(loc='lower center', shadow=True, fontsize='x-large')

    plt.show()
