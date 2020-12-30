#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Arrays of candidates, pairs, and victories (no. of votes won) of winner in each pair
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
int victory[MAX];

// Initialisation of 2 global variables:
// the number of pairs in the array pairs
int pair_count;
// the number of candidates in the array candidates
int candidate_count;


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void bubbleSort(void);
//void selectionSort(void);
//void insertionSort(void);
//void mergeSort(void);
void lock_pairs(void);
bool isCyclic(int source, int end);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    // Iterate over all candidates and insert their names into array candidates
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++) // Loop alls rows
    {
        for (int j = 0; j < candidate_count; j++) // Loop alls columns
        {
            locked[i][j] = false; // our initial graph will have no edges in it
        }
    }

    pair_count = 0; // prepare to count the no. of pairs
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) // Ask for each voter's preferences
    {
        // declare local array to store each voter's preferences
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks)) // if the candidate name is not in candidates list
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i ++)
    {
        if (!strcmp(name, candidates[i])) // if the name appears in the candidate list
        {
            ranks [rank] = i; // update the candidate in the array
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i ++)
        for (int j = i + 1; j <  candidate_count; j ++)
        {
            preferences [ranks[i]][ranks[j]] ++;
        }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i ++)
        for (int j = 0; j <  candidate_count; j ++)
        {
            if (preferences [i][j] > preferences [j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count ++;
            }
        }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i ++)
    {
        victory[i] = preferences [pairs[i].winner][pairs[i].loser];
    }

        bubbleSort();
//        selectionSort();
//        insertionSort();
//        mergeSort();

    return;
}

void bubbleSort(void)
{
    int sort_total = pair_count - 1;
    int swap_counter = -1;
    while (swap_counter != 0)
    {
        swap_counter = 0;
        for (int i = 0; i < sort_total; i ++)
        {
            if (victory[i] < victory[i + 1])
            {
                int swap_victories = victory[i];
                victory[i] = victory[i + 1];
                victory[i + 1] = swap_victories;

                pair swap_pairs = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = swap_pairs;

                swap_counter ++;
            }
        }
        sort_total --;
    }
}

void selectionSort(void)
{
    for (int i = 0; i < pair_count - 1; i ++)
    {
        int max_index = i;
        for (int j = i + 1; j < pair_count; j ++)
        {
            if (victory[j] > victory[max_index])
            {
                max_index = j;
            }
        }
        int swap = victory[i];
        victory[i] = victory[max_index];
        victory[max_index] = swap;

        pair swap_pairs = pairs[i];
        pairs[i] = pairs[max_index];
        pairs[max_index] = swap_pairs;
    }
}

void insertionSort(void)
{
    for (int i = 1; i < pair_count; i ++)
    {
        int swap = victory[i];
        pair swap_pairs = pairs[i];

        int j = i - 1;
        while ((j >= 0) && (victory[j] < swap))
        {
            victory[j + 1] = victory[j];
            pairs[j + 1] = pairs[j];
            j --;
        }
        victory[j + 1] = swap;
        pairs[j + 1] = swap_pairs;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0 ; i < pair_count; i ++) // loop alls pairs
    {
        if (!isCyclic(pairs[i].winner, pairs[i].loser)) // if the edge (pair) wouldn't create a cycle
        {
            locked [pairs[i].winner][pairs[i].loser] = true; // include the new edge (pair) to the graph
        }
    }
    return;
}

// recursive function to identify a cycle inside a graph
// it infinitely extends the stack (trail)
// until the new node added to the stack (trail) has been already in the stack (trail)
// that's when a cycle is created
// Note: source = winner, end = loser
bool isCyclic(int source, int end)
{
    if (source == end) // when the source node is also the end node of a stack (trail)
    {
        return true; // a cycle is created within the graph
    }

    for (int i = 0; i < candidate_count; i++) // if the source node is not yet the end node of a stack (trail)
    {
        if (locked[end][i]) // check if we could add one more node to the stack (trail), that added node become new end node
        {
            if (isCyclic(source, i)) // now the stack (trail) is longer, again check if the source node = new end node
            {
                return true; // if true, a cycle is created within the graph
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i ++)
    {
        int true_count = 0; // calculate the total number of nodes which points to node i
        for (int j = 0; j < candidate_count; j ++)
        {
            if (locked[j][i] == true)
            {
                true_count ++; // increment the total number of nodes which points to node i by 1
            }
        }
        if (true_count == 0) // the source node doesn't have any node which points to it
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}