// Include CS50 and stdio libraries
#include <cs50.h>
#include <stdio.h>


// main function
int main(void)
{
    // declare the height
    int n;
    // prompt height from the user
    do
    {
        n = get_int("Height: ");
    }
    // the height must be between 1 and 8, inclusive
    while (n < 1 || n > 8);

    // print iterations for n rows
    for (int i = 1; i <= n; i++)
    {
        for (int j = n - i; j > 0; j --)
        {
            printf(" ");
        }

        for (int k = 1; k <= i; k ++)
        {
            printf("#");
        }

        printf("\n");

    }
}
