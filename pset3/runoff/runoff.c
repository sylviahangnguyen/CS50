#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i ++)
    {
        if (!strcmp(name, candidates[i].name)) // if the name appears in the candidate list
        {
            preferences [voter][rank] = i; // update the candidate in the array
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    for (int voter = 0; voter < voter_count; voter ++) // set iteration through each voter
    {
        int rank = 0; // firstly, consider the 1st rank candidate
        int i = preferences [voter][rank];
        while (candidates[i].eliminated == true) // if the 1st is eliminated, move to the next
        {
            rank ++;
            i = preferences [voter][rank];
        }
        candidates[i].votes ++; // update the vote of non-eliminated candidates
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated
            && candidates[i].votes > ceil(voter_count / 2)) // consider only non-eliminated candidates that have the majority of votes
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int start = 0; // set the 1st candidate to take the min vote value
    while (candidates[start].eliminated == true) // While that candidate is eliminated, move to the next
    {
        start ++;
    }
    int min = candidates[start].votes; // set the min value = the 1st non-eliminated candidate to date
    for (int i = start + 1; i < candidate_count; i++) // from the next candidate onwards
    {
        if (min > candidates[i].votes && !candidates[i].eliminated) // if the min value > the current value
        {
            min = candidates[i].votes; // update the min value based on the current value
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    int eliminate_false_count = 0; // the no. of non-eliminated candidates to date
    int eliminate_false_total_votes = 0; // the no. of votes of non-eliminated candidates to date
    for (int i = 0 ; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            eliminate_false_count ++; // update the no. of non-eliminated candidates to date
            eliminate_false_total_votes += candidates[i].votes; // update the no. of votes of non-eliminated candidates to date
        }
    }
    // if all non-eliminated candidates to date have the same vote number as the min value
    if (min * eliminate_false_count == eliminate_false_total_votes)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // among non-eliminated candidates, remove the ones that has the vote number = min value
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}