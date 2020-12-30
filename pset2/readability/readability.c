// INCLUDE THE NEEDED LIBRARIES
#include <cs50.h> // library for get_string()
#include <stdio.h> // main C library
#include <ctype.h> // library for isalpha(), isblank()
#include <string.h> // library for strlen(), strcmp()
#include <math.h> // library for round()

// INITIALISE THE SUB FUNCTIONS BEFORE THE MAIN FUNCTION
string getString(void); // initialise the 1st sub function to prompt the string (input)
int countLetter(string s); // initialise the 2nd sub function to count the number of letters
int countWord(string s); // initialise the 3rd sub function to count the number of words
int countSentence(string s); // initialise the 4th sub function to count the number of sentences
int calGrade(int count_letter, int count_word, int count_sentence); // initialise the 5th sub function to calculate the grade

// MAIN FUNCTION
int main(void)
{
    // GET STRING
    string s = getString(); // call the 1st sub function to get the string

    //uncomment 3 lines below when writing function by function
    //printf("%i letter(s)\n", countLetter(s));
    //printf("%i word(s)\n", countWord(s));
    //printf("%i sentence(s)\n", countSentence(s));

    // COUNT THE NO. OF LETTER, WORD AND SENTENCE TO CALCULATE THE INDEX NUMBER
    int index = calGrade(countLetter(s), countWord(s), countSentence(s)); // call other sub functions to pass the index

    // PRINT THE GRADE
    if (index >= 16) // when a grade is equivalent to or greater than a senior undergraduate reading level
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// GETSTRING FUNCTION
string getString(void) // the 1st sub function to prompt the string (input)
{
    string s;
    do
    {
        s = get_string("Text: ");
    }
    while (s == NULL || !strcmp(s, "")); // avoid the null input
    return s;
}

// COUNTLETTER FUNCTION
int countLetter(string s) // the 2nd sub function to count the number of letters
{
    int count_letter = 0; // initialise the letter total count
    int len = strlen(s);
    for (int i = 0; i < len; i++) // set the no. of iteration equal to the no. of char in string
    {
        if (isalpha(s[i]) != 0) // check if the char is a alphabetic letter
        {
            count_letter ++; // increment the letter total count by 1
        }

    }
    return count_letter; // return the result
}

// COUNTWORD FUNCTION
int countWord(string s) // the 3rd sub function to count the number of words
{
    int count_word = 0; // initialise the word total count
    int len = strlen(s);
    for (int i = 0; i < len; i++) // set the no. of iteration equal to the no. of char in string
    {
        if (isblank(s[i]) != 0) // check if the char is a blank space
        {
            count_word ++; // increment the word total count by 1
        }
    }
    count_word ++; // as the total no. of blank space + 1 = the total no. of word
    return count_word; // return the result
}

// COUNTSENTENCE FUNCTION
int countSentence(string s) // the 4th sub function to count the number of sentences
{
    int count_sentence = 0; // initialise the sentence total count
    int len = strlen(s);
    for (int i = 0; i < len; i++) // set the no. of iteration equal to the no. of char in string
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?') // as a sentence ends with ".", "!" or "?"
        {
            count_sentence ++;  // increment the sentence total count by 1
        }
    }
    return count_sentence; // return the result
}

// CALCULATE-GRADE FUNCTION
int calGrade(int count_letter, int count_word, int count_sentence) // the 5th sub function to calculate the grade
{
    // as when dividing int by int in C, the result will be round down to an int
    // therefore we have to convert one of the integers inside the division to "float"
    // we also initialise the result as an float (L and S)
    float L = ((float) count_letter / count_word) * 100;
    float S = ((float) count_sentence / count_word) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);  // now we could round the index and initialise it as an int
    return index;
}