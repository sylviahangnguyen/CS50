#include <math.h>
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user the change
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars <= 0);

    // Convert dollars into cents
    int cents = round(dollars * 100);
    int coins = 0;
    int quarter = 0;
    int dime = 0;
    int nickel = 0;
    int penny = 0;

    // Can I use quarters?
    while (cents >= 25)
    {
        coins ++;
        quarter ++;
        cents -= 25;
    }

    // Can I use dimes?
    while (cents >= 10)
    {
        coins ++;
        dime ++;
        cents -= 10;
    }

    // Can I use nickels?
    while (cents >= 5)
    {
        coins ++;
        nickel ++;
        cents -= 5;
    }

    // Can I use pennies?
    while (cents >= 1)
    {
        coins ++;
        penny ++;
        cents -= 1;
    }

    printf("I have %i coin(s)\n", coins);
    printf("%i quarter(s)\n", quarter);
    printf("%i dime(s)\n", dime);
    printf("%i nickel(s)\n", nickel);
    printf("%i penny(pennies)\n", penny);
}