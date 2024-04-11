//Author: Mikel Garner
#include <stdio.h>
#include <stdlib.h>

// Hash table size
#define HASH_SIZE 23

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};



// HashType
struct HashType
{
    struct RecordType record;
    struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable[], struct RecordType record)
{
    int index = hash(record.id);
    struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->record = record;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray[], int hashSz)
{
    int i;
    struct HashType* temp;
    for (i=0;i<hashSz;++i)
    {
        // if index is occupied with any records, print all
       if (pHashArray[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct HashType* temp = pHashArray[i];
            while (temp != NULL)
            {
                printf("%d %c %d", temp->record.id, temp->record.name, temp->record.order);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    struct HashType* hashTable[HASH_SIZE] = { NULL };
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    //implementing table
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newNode->record = pRecords[i];
        newNode->next = hashTable[index];
        hashTable[index] = newNode;
    }

    // Display records in the hash structure
    printf("Records in Hash Structure:\n");
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free allocated memory
    for (int i = 0; i < HASH_SIZE; i++) {
        struct HashType* temp = hashTable[i];
        while (temp != NULL) {
            struct HashType* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(pRecords);

    return 0;
}
