// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100000;
unsigned int count_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word);
    node *p = table[index];
    while (p != NULL)
    {
        if (strcasecmp(p->word, word) == 0)
        {
            return true;
        }
        p = p->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*
    polynomial rolling hash function
    source: https://cp-algorithms.com/string/string-hashing.html#calculation-of-the-hash-of-a-string
    */
    const int p = 31;
    long long power = 1, hash_value = 0;
    for (unsigned int i = 0; i < strlen(word); ++i)
    {
        hash_value = (hash_value + (toupper(word[i]) - 'A' + 1) * power) % N;
        power = (power * p) % N;
    }
    return hash_value;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }
    char w[LENGTH + 1];
    while (fscanf(input, "%s", w) != EOF)
    {
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            unload();
            return false;
        }
        strcpy(new->word, w);
        unsigned int index = hash(w);

        new->next = table[index];
        table[index] = new;

        count_words ++;
    }
    fclose(input);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; ++i)
    {
        node *n = table[i];
        while (n != NULL)
        {
            node *tmp = n;
            n = n -> next;
            free(tmp);
        }
        // if the function successfully freed up all the allocated memory
        if (i == N - 1 && n == NULL)
        {
            return true;
        }
    }
    return false;
}
