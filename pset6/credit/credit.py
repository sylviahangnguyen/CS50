#! /usr/bin/env python3
# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file
from cs50 import get_int


def main():
    while True:
        card = get_int("Number: ")
        if card > 0:
            break
    card_string = str(card)
    length = len(card_string)

    # GET THE 1ST AND 2ND DIGIT CHECK FOR THE CARD'S TYPE

    power = pow(10, length - 1)  # get the default type of the power needed to extract the first digit
    # unsigned long long int power = temp_power; # convert the power to integer type to perform the division
    firstDigit = card // power  # get the first digit by round down the 1st quotient
    secondDigit = int((card % power) // (power / 10))  # get the second digit by round down the 2nd quotient

    # GET THE LUHN SUM

    luhn_sum = sumDigits(card, length, power)  # get the Luhn sum

    # CHECK THE INVALID LUHN SUM

    if luhn_sum % 10 != 0:  # check if the sum's last digit is equal to 0
        print("INVALID")

    # CHECK THE CARD'S TYPE

    elif length == 15 and firstDigit == 3 and (secondDigit == 4 or secondDigit == 7):  # AMEX's condition
        print("AMEX")
    elif length == 16 and firstDigit == 5 and (secondDigit >= 1 and secondDigit <= 5):  # MASTER's condition
        print("MASTERCARD")
    elif (length == 13 or length == 16) and firstDigit == 4:  # VISA's condition
        print("VISA")
    else:
        print("INVALID")

# FUNCTION: GET THE LUHN SUM


def sumDigits(card, length, power):  # take argument from input, countDigits() and calculated power
    # EXTRACT ALL THE DIGIT FROM CARD NUMBER
    digits= [0] * length  # initialise the list of card digits
    while card != 0:  # stop once all the digits are counted, use debug50 ./credit to investigate the loop
       # use debug50 ./credit to investigate the loop
        for i in range(length):  # set iteration from 0 to length-1 (or 1 to length)
            digits[i] = card // power  # get the 1st digit
            card = card % power  # remove/pop that 1st digit
            power /= 10  # decrease the power by 10 times

    # CALCULATE THE LUHN SUM
    sum = 0  # initialise the Luhn sum of digits
    for even in range(length - 2, -1, -2):  # set iteration for every other index from the 2nd-to-last digit backwards
        if digits[even] < 5:  # if the product of 2 times the digit has only 1 digit (or 2*digit < 10)
            sum += 2 * digits[even]  # add to the Luhn sum
        else:  # if the product of 2 times the digit has 2 digit (or 2*digit >= 10)
            firstRec = (2 * digits[even]) // 10  # extract the 1st digit of that product
            secondRec = (2 * digits[even]) % 10  # extract the 2nd digit of that product
            sum += firstRec + secondRec  # add the above 1st and 2nd digit of the product to the Luhn sum
    for odd in range(length - 1, -1, -2):  # set iteration for every other index from the last digit backwards
        sum += digits[odd]  # add to the Luhn sum
    return sum   # function returns the Luhn sum as an integer


if __name__ == "__main__":
    main()