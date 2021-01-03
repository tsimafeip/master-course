def prefix_func(s):
    n = len(s)
    prefix_res = [0, ]
    for i in range(1, n):
        j = prefix_res[i - 1]
        while j > 0 and s[i] != s[j]:
            j = prefix_res[j - 1]

        if s[i] == s[j]:
            j += 1
        prefix_res.append(j)

    return prefix_res


with open("coin.in", "r") as in_file, open("coin.out", "w") as out_file:
    input_str = in_file.read().replace(" ", "")

    n = len(input_str)
    pow = [1, ]
    for i in range(1, n + 1):
        pow.append(pow[i - 1] * 2)

    res = 0
    p = prefix_func(input_str)
    i = n
    while i > 0:
        res += pow[i]
        i = p[i - 1]

    out_file.write(str(res))
