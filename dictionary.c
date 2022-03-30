// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 456976;

// Hash table
node *table[N];

//Size Counter
int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    //Convent word to all lower for hash
    char *tmp = malloc(strlen(word));
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        tmp[i] = tolower(word[i]);
    }

    //Hash the word & free tmp
    int Hash = hash(tmp);
    //Free tmp
    free(tmp);
    
    //Access that hashed linked list & traverse it
    //Make cursor
    node *cursor = table[Hash];
    //Look through all words using a loop
    while(cursor != NULL)
    {
        //Check if words are the same
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        //Move cursor to next word if not
        cursor = cursor->next;
    }
    
    //If can't find word return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % N; //N is size of hashtable
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open Dictionary File
    FILE *dict = fopen(dictionary, "r");
    //Check if file opened successfully
    if (dict == NULL)
    {
        return false;
    }
    
    //Read Strings from File
    //Word buffer
    char *word = malloc(LENGTH + 1);
    //Scan words from dictionary with loop
    while (fscanf(dict, "%s", word) != EOF)
    {
        //Create a New Node
        node *Node = malloc(sizeof(node));
        //Check for NULL
        if (Node == NULL)
        {
            return false;
        }
        //Copy word into node
        strcpy(Node->word, word);
        
        //Hash Word
        int Hash = hash(Node->word);
        
        //Insert Node into Hash Table
        //Make Node pointer point at first node in the bucket so that we don't lose it
        Node->next = table[Hash];
        //Set the bucket to point to new node
        table[Hash] = Node;
        
        //Count each word
        counter++;
    }
    //Free word buffer & close file
    free(word);
    fclose(dict);
    
    // TODO
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Loop through entire table freeing all words from memory
    for(int i = 0; i < N; i++)
    {
        //Assign variables
        node *cursor = table[i];
        node *tmp = table[i];
        
        //Move cursor and free nodes in loop
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    //Return true once finished freeing all nodes
    return true;
}
