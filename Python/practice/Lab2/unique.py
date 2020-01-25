from collections import Counter


def compress(elements):
    return Counter(elements).items()


if __name__ == '__main__':
    expected_sorted = [(1, 2), (2, 1)]
    actual_sorted = sorted(compress([1, 2, 1]))
    assert expected_sorted == actual_sorted
