def merge(a, b):
    c = []

    it_b = iter(b)
    b_el = next(it_b)

    for a_el in a:
        while b_el and b_el < a_el:
            c.append(b_el)
            b_el = next(it_b, None)
        c.append(a_el)

    if b_el:
        c.append(b_el)
    c.extend(it_b)

    if a.__class__ == tuple:
        return tuple(c)

    return c


if __name__ == '__main__':
    print(merge([1, 2, 7, 28, 235, 235], [3, 10, 11]))
    print(merge((3, 15), (7, 8)))
