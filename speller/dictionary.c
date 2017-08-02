/**
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

typedef struct node
{
    bool is_word;
    struct node* children[27];
} 
node;

node* head;

unsigned int words = 0;

bool freeMemo(node* ptr); 

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
   // create pointer that points to preexisting nodes in linked list.
   node* cursor = head;

    // for each character
    for (int i = 0; word[i] != '\0'; i++) 
    {
       // store its index 
       int index;
       // if current character is apostrophe, index will be 26
       //otherwise, it will be its position in the alphabet (starting from 0)
       if (word[i] == '\'') 
       {
           index = 26;
       }
       else 
       {
           index = tolower(word[i]) % 97;
       }

        // if current element in children is NULL, then word is mispelled
        if (cursor->children[index] == NULL) 
        {
            return false;
        }

        // otherwise set cursor to next character
        cursor = cursor->children[index];
    }

    // once at end of input word, check if is_word is true
    return cursor->is_word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    
    head = malloc(sizeof(node));
    
    //open dictionary
    FILE* d = fopen(dictionary, "r");
    
    if(d == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }
    
    
    node* cursor = head;
    
    //iterate through each word
    for (int c = fgetc(d); c != EOF; c = fgetc(d))
    {
        // Check if char is newline
        if (c == '\n') 
        {
            // set boolean to true
            cursor->is_word = true;

            // Increment number of words
            words++;

            // reset cursor to root to traverse trie again
            cursor = head;
        }
        else 
        {
            //store index
            int i;
            // if current character is apostrophe, index will be 26
            //otherwise, it will be its position in the alphabet (starting from 0)
            if (c == '\'') 
            {
                i = 26;
            }
            else 
            {
                i = tolower(c) % 97;
            }

            // create a new node if current character in children is NULL
            if (cursor->children[i] == NULL) 
            {
                cursor->children[i] = malloc(sizeof(node));
            }

            //set cursor to next node
            cursor = cursor->children[i];
        }
    }
    
    // close dictionary
    fclose(d);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
  return freeMemo(head);
}

/**
 *  Free all memory allocated. 
 */

bool freeMemo(node* ptr)  
{
    // For each character in children array
    for (int i = 0; i < 27; i++)
    {
        // If value at current index is not NULL, free memory for that node
        if (ptr->children[i] != NULL) 
        {
            freeMemo(ptr->children[i]);
        }
    }

    // free pointers 
    free(ptr);

    return true;
}



