import operator


def transpose(iterables):
    return zip(*iterables)


def get_number(iter):
    if isinstance(iter, int) or isinstance(iter, float):
        casting_result = iter
    else:
        try:
            casting_result = int(iter)
        except ValueError:
            return None

    return casting_result


def scalar_product(iterable1, iterable2):
    iterable1 = list(map(get_number, iterable1))
    iterable2 = list(map(get_number, iterable2))

    if all(iterable1) and all(iterable2):
        return sum(map(operator.mul, iterable1, iterable2))

    return None


if __name__ == '__main__':
    expected = [[1, 2, 3], [-1, 3, 2]]
    actual = transpose([[1, -1], [2, 3], [3, 2]])
    assert expected == list(map(list, actual))

    expected = 1
    actual = scalar_product([1, 2], [-1, 1])
    assert expected == actual

    actual = scalar_product([1, 'xyz'], [-1, 1])
    assert actual is None
