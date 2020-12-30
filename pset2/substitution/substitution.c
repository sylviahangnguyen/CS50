#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

string getPlaintext(void); // initialise the 1st sub function to prompt the plain test (input)
string getCiphertext(string plain_text,
                     string key_string); // initialise the 2nd sub function convert from plain text to cipher text (output)

int main(int argc, string argv[])
{
    string key_string = argv[1]; // initial a global variable from the command argument as a string
    if (argc != 2) // make sure that the user give only 1 command argument
    {
        printf("Usage: ./substitution KEY\n");
        return 1; // exit the program
    }
    else if (strlen(key_string) != 26)
    {
        printf("Key must contain 26 characters\n"); // make sure that the user give enough 26 characters
        return 1; // exit the program
    }
    else
    {
        int key_len = strlen(key_string); // get the length (no. of chars) of the plain text
        for (int i = 0; i < key_len; i++) // set iteration for each char in the key string
        {
            if (isdigit(key_string[i]) != 0) // make sure that all the chars are digits
            {
                printf("Key must only contain alphabetic characters\n"); //
                return 1; // exit the program
            }
            int count = 1; // set the count (to count the no. of each character's appreance) for the current character to 1
            for (int j = i + 1; j < key_len; j++) // set iteration in the key string from the next char onward
            {
                if (key_string[i] == key_string[j]) // if the current char equal to any char after it in the key string
                {
                    count ++; // increment count by 1
                }
            }
            if (count > 1) // raise the warning when the count of the current char in key string > 1
            {
                printf("Key must not contain repeated characters\n");
                return 1; // exit the program
            }
        }
    }
    string plain_text = getPlaintext(); // sub function to get the plain text from user
    string cipher_text = getCiphertext(plain_text,
                                       key_string);  // sub function to convert the plain text to cipher text
    printf("ciphertext: %s\n", cipher_text); // print the cipher text
    free(cipher_text);
}

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

string getCiphertext(string plain_text,
                     string key_string) // the 2nd sub function convert from plain text to cipher text (output)
{
    int len = strlen(plain_text); // get the length (no. of chars) of the plain text
    /* From Lecture 4 > Notes > Compare and copy
    allocate the number of locale for each char in string cipher_text equal to the length of plain text */
    char *cipher_text = malloc(len + 1);
    cipher_text[len] = (char) 0;
    // initalize the array abc[26] = {0,1,...,25} (the alphabetic integer for ABCD....XYZ)
    // initalize the array int_key_string[26] = {x0, x1, ..., x25} (the alphabetic integer for the key string)
    // initalize the array key[26] = {x0-0,x1-1,...,x25-25} (the diff between the alphabetic integer for the key string and for ABC..XYZ)
    int abc[26], int_key_string[26], key[26];
    for (int i = 0; i < 26; i++)
    {
        abc[i] = i; // set the value for each item in the array abc[26] = {0,1,...,25} (This is the alphabetic integer array for {A, B, .., Z})
        // set the value for each item in the array int_key_string[26] = {x_0, x_1, ..., x_25} (the alphabetic integer for the key string)
        // by convert each char to upper form, take its ASCII integer and move 65 locales backward to receive the alphabetic integer
        int_key_string[i] = (int) toupper(key_string[i]) - 65;
        // calculate the diff (no. of locale) between the alphabetic integer for the key string and for ABC..XYZ
        key[i] = int_key_string[i] - abc[i];
    }
    // initalize the array int_plain_text[len] = {x_0, x_1, ..., x_len} (the alphabetic integer for the plain text)
    int int_plain_text[len];
    for (int j = 0; j < len; j++) // for each char in the plain text
    {
        if (isalpha(plain_text[j]) != 0) // check if the char is a alphabetic letter
        {
            int shift = 0; // initialise the no. of locale needed to be shifted from ASCII to Alphabet
            if (isupper(plain_text[j]) != 0) // if the char is in upper form, shift 65 locales back (A = 65)
            {
                shift = 65;
            }
            else // if the char is in lower form, shift 97 locales back (a = 97)
            {
                shift = 97;
            }
            int_plain_text[j] = (int) plain_text[j] - shift; // get the Alphabetic integer for the char in the plain text
            for (int i = 0; i < 26; i++) // match the alphabetic integer of the current char to that of the char in ABC...XYZ array
            {
                if (int_plain_text[j] == abc[i])
                {
                    // take the alphabetic integer
                    // plus the key (the diff (no. of locale) between the alphabetic integer for the key string and for ABC..XYZ)
                    // take their module for 26
                    // from alphabetic integer, shift forward to get the ASCII integer (of the cipher text)
                    int int_cipher_text = (int_plain_text[j] + key[i]) % 26 + shift;
                    // convert from ASCII integer to char
                    cipher_text[j] = (char) int_cipher_text;
                }
            }
        }
        else // if the char is not an alphabetic letter
        {
            cipher_text[j] = plain_text[j]; // remain the value as the same as in plain text
        }
    }
    return cipher_text; // return the cipher text as a string
}