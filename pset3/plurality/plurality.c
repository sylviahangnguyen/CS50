#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9 // define a constant

// Candidates have name and vote count
typedef struct // define the type candidate with 2 attributes: name and votes
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name)) // if vote(name) return False or 0
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // set iteration through each candidate
    {
        if (!strcmp(name, candidates[i].name)) // if the name appears in the candidate list
        {
            candidates[i].votes ++; // increment that candidate's votes by 1
            return true;
        }
    }
    return false; // name doesn't appear in the candidate list
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    int max = 0; // initialize the maximum vote count
    for (int i = 0; i < candidate_count; i++) // set iteration through each candidate
    {
        if (max < candidates[i].votes) // if the current maximum vote count < ith candidate's vote count
        {
            max = candidates[i].votes; // set the new max vote count = ith candidate's vote count
        }
    }
    for (int i = 0; i < candidate_count; i++)  // set iteration through each candidate
    {
        if (candidates[i].votes == max) // find all the candidates that have vote count = the maximum vote count
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}