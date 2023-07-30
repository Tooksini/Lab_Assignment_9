#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for the linked list (for separate chaining)
struct Node
{
    struct RecordType data;
    struct Node *next;
};

// Fill out this structure
struct HashType
{
    struct Node *head;
};

// Compute the hash function
int hash(int x, int hashSz){
    return x % hashSz;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
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
		// Implement parse data block
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
	struct Node *current;

	for (i = 0; i < hashSz; ++i){
		current = pHashArray[i].head;
		if (current != NULL){
			printf("Index %d -> \n", i);
			while (current != NULL){
				printf("\t%d %c %d -> ", current->data.id, current->data.name, current->data.order);
				current = current->next;
			}
			printf("NULL\n");
		}
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementaion
    int hashSz = 15;
    struct HashType *pHashArray = (struct HashType *)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL){
        printf("Memory allocation error for hash array\n");
        free(pRecords);
        exit(-1);
    }

    // Initialize each head of the linked lists to NULL
    for (int i = 0; i < hashSz; ++i){
        pHashArray[i].head = NULL;
    }

    // Insert records into the hash table using separate chaining
    for (int i = 0; i < recordSz; ++i){
        int index = hash(pRecords[i].id, hashSz);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        if (newNode == NULL){
            printf("Memory allocation error for new node\n");
            free(pRecords);
            free(pHashArray);
            exit(-1);
        }

        newNode->data = pRecords[i];
        newNode->next = pHashArray[index].head;
        pHashArray[index].head = newNode;
    }

    // Display records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free allocated memory
    for (int i = 0; i < hashSz; ++i){
        struct Node *current = pHashArray[i].head;
        while (current != NULL){
            struct Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
    free(pRecords);

    return 0;
}
