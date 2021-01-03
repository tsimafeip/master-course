def get_char_index(c):
    if c == '$':
        return 0

    return ord(c) - ord('a') + 1  # sentinel


def bw_matching(n, pattern, occurences, count):
    top = 0
    bottom = n - 1
    while top <= bottom:
        if pattern:
            symbol = pattern[-1]
            pattern = pattern[:-1]
            char_index = get_char_index(symbol)
            top = occurences[char_index] + count[top][char_index]
            bottom = occurences[char_index] + count[bottom + 1][char_index] - 1
        else:
            return bottom - top + 1
    return 0


if __name__ == "__main__":
    bwt_str = input()
    pattern = input()
    first_column = ''.join(sorted(bwt_str))
    n = len(bwt_str)
    alphabet_len = 27

    count_arr = [[0] * alphabet_len for _ in range(n + 1)]

    first_occurences = [-1 for _ in range(alphabet_len)]

    for index, (bwt_symbol, first_col_symbol) in enumerate(zip(bwt_str, first_column)):
        count_arr[index + 1] = count_arr[index][:]
        bwt_char_index = get_char_index(bwt_symbol)
        count_arr[index + 1][bwt_char_index] += 1
        first_col_char_index = get_char_index(first_col_symbol)
        if first_occurences[first_col_char_index] == -1:
            first_occurences[first_col_char_index] = index

    res = bw_matching(n, pattern, first_occurences, count_arr)
    print(res)
