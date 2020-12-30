#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

string getPlaintext(void); // initialise the 1st sub function to prompt the plain test (input)
string getCiphertext(string plain_text, int key); // initialise the 2nd sub function convert from plain text to cipher text (output)

int main(int argc, string argv[])
{
    int key = 0; // initialise the global variable key
    if (argc != 2) // make sure that the user give only 1 command argument
    {
        printf("Usage: ./caesar key\n");
        return 1; // exit the program
    }
    else
    {
        string key_string = argv[1]; // initial a local variable from the command argument as a string
        int key_len = strlen(key_string);
        for (int i = 0; i < key_len; i++) // set iteration for each char in the key string
        {
            if (isdigit(key_string[i]) == 0) // make sure that all the chars are digits
            {
                printf("Usage: ./caesar key\n"); //
                return 1; // exit the program
            }
            else
            {
                // convert the key_string from string to integer
                // let the global variable "key" point to location of that integer
                key = atoi(key_string);
            }
        }
    }
    string plain_text = getPlaintext(); // sub function to get the plain text from user
    string cipher_text = getCiphertext(plain_text, key); // sub function to convert the plain text to cipher text
    printf("ciphertext: %s\n", cipher_text); // print the cipher text
    free(cipher_text);
}

// GETSTRING FUNCTION
string getPlaintext(void) // the 1st sub function to prompt the string (input)
{
    string plain_text;
    do
    {
        plain_text = get_string("plaintext: ");
    }
    while (plain_text == NULL || !strcmp(plain_text, "")); // avoid the null input
    return plain_text;
}

string getCiphertext(string plain_text, int key) // the 2nd sub function convert from plain text to cipher text (output)
{
    int len = strlen(plain_text); // get the length (no. of chars) of the plain text
    /* From Lecture 4 > Notes > Compare and copy
    allocate the number of locale for each char in string cipher_text equal to the length of plain text */
    char *cipher_text = malloc(len + 1);
    cipher_text[len] = (char) 0;
    for (int i = 0; i < len; i++) // set the no. of iteration equal to the no. of char in string
    {
        if (isalpha(plain_text[i]) != 0) // check if the char is a alphabetic letter
        {
            int shift; // declare the no. of locale needed to be shifted from ASCII to Alphabet
            if (isupper(plain_text[i]) != 0) // if the char is in upper form, shift 65 locales back (A = 65)
            {
                shift = 65;
            }
            else // if the char is in lower form, shift 97 locales back (a = 97)
            {
                shift = 97;
            }
            // convert char plain_text[i] to the ASCII location number
            // shift that location back by shift number
            // the result is the locale of that char in Alphabet table
            int ascii_to_alpha = (int) plain_text[i] - shift; // convert char plain_text[i] to the ASCII location number
            // find the locale of the cipher char
            // shift forward to convert from Alphabet table back to ASCII table
            // get the real locale in ASCII of the cipher char
            int alpha_to_ascii = (ascii_to_alpha + key) % 26 + shift;
            // convert from the locale of that cipher char to the value in this locale (the char)
            cipher_text[i] = (char) alpha_to_ascii;
        }
        else // if the char is not an alphabetic letter
        {
            cipher_text[i] = plain_text[i]; // remain the value as the same as in plain text
        }
    }
    return cipher_text; // return the cipher text as a string
}