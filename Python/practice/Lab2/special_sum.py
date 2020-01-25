def calculate_special_sum(n):
    return sum([pow(i, 2)*(i+1) for i in range(1, n)])


if __name__ == '__main__':
    assert calculate_special_sum(3) == 14
