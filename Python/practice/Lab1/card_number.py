import random


def check_card_number(num):
    numbers = []

    while num:
        digit = num % 10
        numbers.append(digit)
        num = num//10

    numbers.reverse()
    num_sum = find_luhn_sum(numbers)

    return num_sum % 10 == 0


def check_card_number_str(num_str):
    numbers = list(num_str)
    num_sum = find_luhn_sum(numbers)

    return num_sum % 10 == 0


def find_luhn_sum(numbers):
    num_sum = 0
    for index, number in enumerate(numbers):
        if index % 2 == 0:
            doubled_number = int(number) * 2
            if doubled_number > 9:
                doubled_number -= 9
            num_sum += doubled_number
        else:
            num_sum += int(number)

    return num_sum


def generate_card_number(card_type):
    if card_type == 'visa':
        card_number = '4'
    elif card_type == 'mastercard':
        card_number = '5'
    else:
        raise Exception("Unsupported card type: {0}. Only 'visa' and 'mastercard' are supported".format(card_type))

    for i in range(14):
        card_number += str(random.randint(0, 9))

    num_sum = find_luhn_sum(card_number)
    last_number = (10 - (num_sum % 10))
    card_number += str(last_number)

    return card_number


if __name__ == '__main__':
    assert check_card_number(5082337440657928)  # valid Mastercard card number
    assert not check_card_number(5082337440657929)  # valid Mastercard card number
    assert not check_card_number_str('4601496706376197')  # invalid Visa card number
    assert check_card_number_str(generate_card_number('visa'))
    assert check_card_number_str(generate_card_number('mastercard'))
