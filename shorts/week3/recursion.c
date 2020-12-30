#include <cs50.h>
#include <stdio.h>

int collatz1(int n);
int collatz2(int n);

int main(void)
{
    int n = get_int("Give me an positive integer: ");
    int option = get_int("Choose 1. Use while loop or 2. Recursion: ");
    if (option == 1)
    {
        int count = collatz1(n);
        printf("We need %i steps to get 1.\n", count);
    }
    else
    {
        int count = collatz2(n);
        printf("We need %i steps to get 1.\n", count);
    }
}

int collatz1(int n)
{
    int count = 0;
    if (n == 1)
        return 0;
    else
    {
        while (n != 1)
        {
            if (n % 2 == 0)
            {
                n /= 2;
                count++;
            }
            if (n % 2 == 1 && n != 1)
            {
                n = 3*n + 1;
                count++;
            }
        }
        return count;
    }
}

int collatz2(int n)
{
    // base case
    if (n == 1)
        return 0;
    // even numbers
    else if (n % 2 == 0)
    {
        return 1 + collatz2(n/2);
    }
    // odd numbers
    else
    {
        return 1 + collatz2(3*n + 1);
    }
}