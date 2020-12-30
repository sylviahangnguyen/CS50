// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 17576;

// Hash table
node *table[N];


int main(void)
{
    const char *dictionary = "dictionaries/large";
    const char *word = "SYnthetasE";
    bool loaded = load(dictionary);

    if (!loaded)
    {
        printf("Not loaded!\n");
    }
    else
    {
        bool checked = check(word);
            if (!checked)
            {
                printf("Not there!\n");
            }
            else
            {
                printf("Found!\n");
                printf("%i words in dictionary\n", size());
                bool unloaded = unload();
                if (!unloaded)
                {
                    printf("Unloaded!\n");
                }
                else
                {
                    printf("Not unloaded!\n");
                }
            }
    }
}

bool check(const char *word)
{
    // Hash word to obtain the hash value
    int bucket = hash(word);

    // Search the linked list
    node *tmp = table[bucket];
    do
    {
        if (strcasecmp( word, tmp->word) == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    while (tmp != NULL);
    return false;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dict = fopen(dictionary , "r");
    if (dict == NULL) // if input file cannot be read, exit the program
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 1;
    }
    // Read strings from file one at a time
    char *word = malloc(LENGTH + 1);

    // Create a new node for each word
    node *new_node = NULL;

    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create a new node for each word
        new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return 1;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash word to obtain the hash value
        int bucket = hash(word);

        // Insert node into hash table at that location
        if (table[bucket] == NULL)
        {
            table[bucket] = new_node;
        }
        else
        {
            new_node->next = table[bucket];
            table[bucket] = new_node;
        }
    }
    fclose(dict);
    free(word);
    return true;
}

// Hashes function, taking input as the 1st 3 letters in the word, returns an integer
unsigned int hash(const char *word)
{
    int len = strlen(word); // count the number of letters in the word
    unsigned int bucket; // the value for the output bucket

    if (len == 1) // if the word has only 1 letter
    {
        bucket = ((int) toupper(word[0]) - 65) * pow(26, 2);
    }
    else if (len == 2) // if the word has only 2 letters
    {
        bucket = ((int) toupper(word[0]) - 65) * pow(26, 2)
               + ((int) toupper(word[1]) - 65) * pow(26, 1);
    }
    else // if the word has more than 2 letters
    {
        bucket = ((int) toupper(word[0]) - 65) * pow(26, 2)
               + ((int) toupper(word[1]) - 65) * pow(26, 1)
               + ((int) toupper(word[2]) - 65) * pow(26, 0);
    }
    return bucket;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int count = 0;

    for (int i = 0; i < N; i ++)
    {
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            count ++;
        }
    }
    return count;
}

bool unload(void)
{
    node *tmp = NULL;
    for (int i = 0; i < N; i ++)
    {
        if (table[i] != NULL)
        {
            tmp = table[i];
            node *cursor = table[i]->next;
            while (cursor != NULL)
            {
                free(tmp);
                tmp = cursor;
                cursor = cursor->next;
                if (cursor == NULL)
                {
                    free(tmp);
                }
            }
        }
    }
    if (tmp == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}