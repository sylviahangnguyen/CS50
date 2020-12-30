#include <math.h> // library for pow()
#include <cs50.h> // library for get_long()
#include <stdio.h> // library for almost all C functions

int countDigits(long card); // initialise the function to count the number of digits before the main function
int sumDigits(long card, int len, unsigned long long int power); // initialise the function to check the Luhn sum

int main(void)
{
    // PROMPT THE CARD NUMBER (INPUT)
    long card; // initialise the type of card (from 13 to 16 digits)
    do
    {
        card = get_long("Number: "); // prompt the card number
    }
    while (card <= 0); // avoid the negative long and 0

    // GET THE DIGIT COUNT TO CHECK FOR THE CARD'S LENGTH => CARD'S TYPE
    int len = countDigits(card); // count the number of digits from the card number

    // GET THE 1ST AND 2ND DIGIT CHECK FOR THE CARD'S TYPE
    double temp_power = pow(10, len - 1); // get the default type of the power needed to extract the first digit
    unsigned long long int power = temp_power; // convert the power to integer type to perform the division
    int firstDigit = card / power; // get the first digit by round down the 1st quotient
    int secondDigit = (card % power) / (power / 10); // get the second digit by round down the 2nd quotient

    // GET THE LUHN SUM
    int sum = sumDigits(card, len, power); // get the Luhn sum

    // CHECK THE INVALID LUHN SUM
    if (sum % 10 != 0) // check if the sum's last digit is equal to 0
    {
        printf("INVALID\n");
    }

    // CHECK THE CARD'S TYPE
    else if (len == 15 && firstDigit == 3 && (secondDigit == 4 || secondDigit == 7)) // AMEX's condition
    {
        printf("AMEX\n");
    }
    else if (len == 16 && firstDigit == 5 && (secondDigit >= 1 && secondDigit <= 5)) // MASTER's condition
    {
        printf("MASTERCARD\n");
    }
    else if ((len == 13 || len == 16) && firstDigit == 4) // VISA's condition
    {
        printf("VISA\n");
    }

    // THE UNQUALIFIED CARD'S TYPE
    else
    {
        printf("INVALID\n");
    }
}

// FUNCTION: GET THE DIGIT COUNT TO CHECK FOR THE CARD'S LENGTH => CARD'S TYPE
int countDigits(long card)
{
    int len = 0; // initialise the length (the number of digits) of the card number
    while(card != 0) // use debug50 ./credit to investigate the loop
    {
       card = card / 10; // remove/pop the last digit from the card number
       len ++; // increment the length by 1, as we just counted one last digit
    }
    return len; // function return the length as an integer
}

// FUNCTION: GET THE LUHN SUM
int sumDigits(long card, int len, unsigned long long int power) // take argument from input, countDigits() and calculated power
{
    // EXTRACT ALL THE DIGIT FROM CARD NUMBER
    int digits[len]; // initialise the list of card digits
    while (card != 0) // stop once all the digits are counted, use debug50 ./credit to investigate the loop
    {
       // use debug50 ./credit to investigate the loop
       for (int i = 0; i < len;  i++) // set iteration from 0 to len-1 (or 1 to len)
        {
            digits[i] = card / power; // get the 1st digit
            card = card % power; // remove/pop that 1st digit
            power /= 10; // decrease the power by 10 times
        }
    }

    // CALCULATE THE LUHN SUM
    int sum = 0; // initialise the Luhn sum of digits
    for (int even = len - 2; even >= 0; even -= 2) // set iteration for every other index from the 2nd-to-last digit backwards
    {
        if (digits[even] < 5) // if the product of 2 times the digit has only 1 digit (or 2*digit < 10)
        {
            sum+= 2 * digits[even]; // add to the Luhn sum
        }
        else // if the product of 2 times the digit has 2 digit (or 2*digit >= 10)
        {
            int firstRec = (2 * digits[even]) / 10; // extract the 1st digit of that product
            int secondRec = (2 * digits[even]) % 10; // extract the 2nd digit of that product
            sum += firstRec + secondRec; // add the above 1st and 2nd digit of the product to the Luhn sum
        }
    }

    for (int odd = len - 1; odd >= 0; odd -= 2) // set iteration for every other index from the last digit backwards
    {
        sum += digits[odd]; // add to the Luhn sum
    }
    return sum; // function returns the Luhn sum as an integer
}