import numpy as np
import matplotlib.pyplot as plt
import math
import copy
from collections import OrderedDict


def eucl_dist(a, b):
    return round(math.sqrt(math.pow((a[0]-b[0]), 2) + math.pow((a[1]-b[1]), 2)), 3)


def get_route_cost(route, distances):
    sum_without_last = sum([distances[city_num][route[i+1]] for i, city_num in enumerate(route) if i+1 < len(route)])
    return sum_without_last + distances[route[-1]][route[0]]


def get_next_route(route, swap=True):
    new_route = copy.deepcopy(route)
    swap_coords = [0, 0]
    while swap_coords[0] == swap_coords[1]:
        swap_coords = np.random.randint(0, len(route)-1, 2)
    i, j = swap_coords[0], swap_coords[1]
    if swap:
        new_route[i], new_route[j] = new_route[j], new_route[i]
    else:
        if i > j:
            i, j = j, i
        list_to_invert = route[i:j]
        list_to_invert.reverse()
        new_route[i:j] = list_to_invert
    return new_route


def metropolis(initial_route, distances, t, steps):
    x = copy.deepcopy(initial_route)
    changes_counter = 0
    for i in range(steps):
        y = get_next_route(x)
        delta = get_route_cost(y, distances) - get_route_cost(x, distances)
        if delta <= 0:
            x = y
            changes_counter += 1
        else:
            if np.random.rand() < math.exp(-delta/t):
                x = y
                changes_counter += 1

    return x, changes_counter


def plot_route(route, points, t=0, title=""):
    x_values = []
    y_values = []
    for city_num in route:
        x_values.append(points[city_num][0])
        y_values.append(points[city_num][1])

    x_values.append(points[route[0]][0])
    y_values.append(points[route[0]][1])

    plt.plot(x_values, y_values, '-o')
    if title:
        plt.title(title)
    else:
        plt.title(f"T={t}")
    plt.show()


def simulate_annealing(N):
    x_values = np.random.random(N)*N
    y_values = np.random.random(N)*N
    points = OrderedDict()
    for i, x_value in enumerate(x_values):
        points[i] = (round(x_value, 3), round(y_values[i], 3))

    distances = [[0 for x in range(N)] for y in range(N)]
    for i in range(N):
        for j in range(N):
            distances[i][j] = eucl_dist(points[i], points[j])

    route = list(points.keys())
    print(route)

    plot_route(route, points, title="Initial route.")
    metropolis_steps = 1000
    print(f"Initial route cost: {get_route_cost(route, distances)}.")
    t_start = 10000
    t_end = 0.1
    t = t_start
    for k in range(100000):
        route, amount_of_changes = metropolis(route, distances, t, metropolis_steps)
        t = t_start/(k+1)
        if t < t_end:
            break

    plot_route(route, points, title="Final route.")
    print(f"Final route cost: {get_route_cost(route, distances)}.")
    print(f"Final route: {route}")


if __name__ == "__main__":
    N1, N2, N3 = 10, 30, 100

    simulate_annealing(10)