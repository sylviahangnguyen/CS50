#! /usr/bin/env python3

# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file

def main():
    # GET STRING
    s = input("Text: ")  # call the 1st sub function to get the string
    length = len(s)

    # COUNT THE NO. OF LETTER, WORD AND SENTENCE TO CALCULATE THE INDEX NUMBER
    index = calGrade(countLetter(s),
                     countWord(s),
                     countSentence(s))  # call other sub functions to pass the index

    # PRINT THE GRADE
    if index >= 16:  # when a grade is equivalent to or greater than a senior undergraduate reading level
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")

# COUNTLETTER FUNCTION


def countLetter(s):  # the 2nd sub function to count the number of letters
    count_letter = 0  # initialise the letter total count
    for c in s:   # set the no. of iteration equal to the no. of char in string
        if c.isalpha() != 0:  # check if the char is a alphabetic letter
            count_letter += 1  # increment the letter total count by 1
    return count_letter  # return the result

# COUNTWORD FUNCTION


def countWord(s):  # the 3rd sub function to count the number of words
    count_word = 0  # initialise the word total count
    for c in s:   # set the no. of iteration equal to the no. of char in string
        if c == ' ':  # check if the char is a blank space
            count_word += 1  # increment the word total count by 1
    count_word += 1  # as the total no. of blank space + 1 = the total no. of word
    return count_word  # return the result

# COUNTSENTENCE FUNCTION


def countSentence(s):  # the 4th sub function to count the number of sentences
    count_sentence = 0  # initialise the sentence total count
    for c in s:  # set the no. of iteration equal to the no. of char in string
        if c == '.' or c == '!' or c == '?':  # as a sentence ends with ".", "!" or "?"
            count_sentence += 1  # increment the sentence total count by 1
    return count_sentence  # return the result

# CALCULATE-GRADE FUNCTION


def calGrade(count_letter, count_word, count_sentence):  # the 5th sub function to calculate the grade
    # as when dividing int by int in C, the result will be round down to an int
    # therefore we have to convert one of the integers inside the division to "float"
    # we also initialise the result as an float (L and S)
    L = (count_letter / count_word) * 100
    S = (count_sentence / count_word) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)  # now we could round the index and initialise it as an int
    return index


if __name__ == "__main__":
    main()