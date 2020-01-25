def check_lucky_ticket(n):
    numbers = [int(x) for x in list(str(n))]
    even_sum = sum(numbers[::2])
    odd_sum = sum(numbers[1::2])
    return even_sum == odd_sum


def get_nearest_lucky_ticket(number):
    if check_lucky_ticket(number):
        return number

    bigger_number = number + 1
    lower_number = number - 1
    while True:
        if check_lucky_ticket(bigger_number):
            return bigger_number
        elif check_lucky_ticket(lower_number):
            return lower_number
        else:
            lower_number -= 1
            bigger_number += 1


if __name__ == '__main__':
    assert get_nearest_lucky_ticket(111111) == 111111
    assert get_nearest_lucky_ticket(123321) == 123321
    assert get_nearest_lucky_ticket(123320) == 123321
    assert get_nearest_lucky_ticket(333999) == 334004
    assert get_nearest_lucky_ticket(902) == 891
