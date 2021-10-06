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

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
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
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    
    //Preset all preferences to 0
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
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
    //Look for a candidate called name
    for (int i = 0; i < candidate_count; i++)
    {
        //If candidate is found, update ranks and return true
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
       
    }
    //If no candidates found, don't update any ranks and return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //Define counter variable
    int counter = 0;
    //Look at each pair of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //Determine the winner and loser, if any and add them to the pairs array
            if (preferences[i][j] > 0)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[counter].winner = i;
                    pairs[counter].loser = j;
                    counter++;
                }
            
            }
        }
    }
    
    //Update global variable pair_count to be the total number of pairs
    pair_count = counter;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Define sorting variables
    pair x;
    pair y;
    int counter;
    
    //Sort pairs with bubble sort
    do
    {
        counter = 0;
        for (int i = 0; i < pair_count; i++)
        {
            for (int j = i + 1; j < i + 2 && j < pair_count; j++)
            {
                if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
                {
                    x = pairs[i];
                    y = pairs[j];
                    pairs[i] = y;
                    pairs[j] = x;
                    counter++;
                }
            }
        }
    }
    while (counter > 0);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //Lock first pair
    locked[pairs[0].winner][pairs[0].loser] = true;
    
    //Check for loops
    //Loops counter
    int counter;
    //Look through each candidate to check if there would be a loop
    for (int i = 1; i < pair_count; i++)
    {
        //If the loser of the current pair matches any previous winners,
        counter = 0;
        for (int j = 0; j < i && j < pair_count; j++) 
        {
            if (pairs[i].loser == pairs[j].winner)
            {
                //And if the winner of the current pair matches any previous losers then add to loop counter
                for (int k = 0; k < i && k < pair_count; k++)
                {
                    if (pairs[i].winner == pairs[k].loser)
                    {
                        counter++;
                    }
                }
            }
        }
        //If there were no loops counted then lock in the current pair
        if (counter < 1)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //Define counter variable
    int counter;
    //Look through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //Check if candidate is losing to anyone, and if not print that candidate as the winner
        counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                counter++;
            }
        }
        if (counter == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

