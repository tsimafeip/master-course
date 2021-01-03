def get_char_index(c):
    if c == '$':
        return 0

    return ord(c) - ord('a') + 1  # sentinel


if __name__ == "__main__":
    #bwt_str = input()
    bwt_str = 'abc$baaa'
    n = len(bwt_str)
    alphabet_len = 27

    first_occurences = [-1 for _ in range(alphabet_len)]

    for index, first_col_symbol in enumerate(sorted(bwt_str)):
        char_index = get_char_index(first_col_symbol)
        if first_occurences[char_index] == -1:
            first_occurences[char_index] = index

    # t, in fact, hold the sort order of the bwt_str indices
    t = [-1 for _ in range(n)]
    sentinel_pos = -1
    for i, bwt_char in enumerate(bwt_str):
        char_index = get_char_index(bwt_char)
        if char_index == 0:
            sentinel_pos = i
        t[first_occurences[char_index]] = i
        first_occurences[char_index] += 1

    j = t[sentinel_pos]
    answer = ['' for _ in range(n)]
    for i in range(n):
        answer[i] = bwt_str[j]
        j = t[j]

    print("".join(answer))
