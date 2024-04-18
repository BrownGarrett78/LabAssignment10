#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26 // English alphabet size

// Trie structure
struct Trie
{	
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord; // 1 if the node represents the end of a word
};

// Create a new trie node
struct Trie *createTrie()
{
    struct Trie *pNode = (struct Trie *)malloc(sizeof(struct Trie));
    pNode->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;
    while (*word)
    {
        int index = *word - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = createTrie();
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->isEndOfWord++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;
    while (*word)
    {
        int index = *word - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->isEndOfWord;
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie != NULL)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (pTrie->children[i] != NULL)
            {
                deallocateTrie(pTrie->children[i]);
            }
        }
        free(pTrie);
        pTrie = NULL;
    }
    return pTrie;
}

// Reads words from the file into the trie
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("File could not be opened\n");
        return 0;
    }

    char word[100];
    int count = 0;
    while (fgets(word, sizeof(word), file))
    {
        word[strcspn(word, "\n")] = 0; // Remove newline character
        pInWords[count] = strdup(word); // Allocate memory and copy the word
        count++;
    }
    fclose(file);
    return count;
}

int main(void)
{
    char *inWords[256];
    
    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
