#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

/******************************************************************************
* Global Variables
*******************************************************************************/

int numNodes = 0;
int queueEnd = 1;
struct node_t pool[256] = {{0}};
node priorityQ[255];
node *firstElement = priorityQ - 1;
char *code[128] = {0};
char buffer[1024];
int inputData = 0;
int outputData = 0;

/******************************************************************************
* Main
*******************************************************************************/
 void compress(char *text) {

    FILE *filepointer_in, *filepointer_out;
    char filename[256] = {0};
    char temp[256] = {0};
    unsigned int freq[128] = {0};
    unsigned int i;
    int fileArraySize;
    char *fileArray;

    strcpy(filename, text);
    strcpy(temp, text);
    strcat(temp, ".tpm");

    filepointer_in = fopen(filename, "r");

    if (filepointer_in == NULL) {
        printf("Error opening file\n\n\n");
        return;
    }

    importFile(filepointer_in, freq);

    filepointer_out = fopen(temp, "w");
    if (filepointer_out == NULL) {
        printf("Error Writing compressed File.");
    }

    encodeFunction(filepointer_in, filepointer_out, freq);
    fclose(filepointer_in);
    fclose(filepointer_out);
    
    fileArraySize = dynamicArray(temp);
    if(fileArraySize < 0) {
        printf("Error determining file size.\n");
        return;
    }
    fileArray = (char*)malloc(fileArraySize);
    if (fileArray == NULL) {
        printf("Error allocating memory.\n");
        return;
    }

    filepointer_out = fopen(temp, "r");

    if(filepointer_out == NULL) {
        printf("Error opening file.");
        free(fileArray);
        return;
    }

    fread(fileArray, 1, fileArraySize, filepointer_out);
    fclose(filepointer_out);

    filepointer_in = fopen(text, "w");
    if(filepointer_in == NULL) {
        printf("Error opening file.");
        free(fileArray);
        return;
    }

    fwrite(fileArray, 1, fileArraySize, filepointer_in);
    fclose(filepointer_in);

    int removeNum;
    removeNum = remove(temp);

    if(removeNum != 0) {
        printf("Temporary File not deleted properly.");
    }

    free(fileArray);

    strcat(filename, ".codeTable");
    filepointer_out = fopen(filename, "w");

    for (i = 0; i < 128; i++) {
        fprintf(filepointer_out, "%c", (char)freq[i]);
    }
    for(i = 0; i < 128; i++) inputData += freq[i];

    fclose(filepointer_out);

 }

/*****************************************************************************
* Prototypes
******************************************************************************/
node newNode(int freq, char data, node a, node b) {
    
    /* Assigns a new node from pool array & increments # of nodes */
    node n = pool + numNodes++;

    /* if freq is not 0 it means it is a leaf node */
    if(freq != 0) {

        n->data = data;
        n->freq = freq;

    }
    /* is an internal node & nodes a & b are assigned to left & right */
    else {

        n->left = a;
        n->right = b;
        /* node freq is the sum of the child freq */
        n->freq = a->freq + b->freq;

    }

    return n;

}

/*****************************************************************************
* Prototypes : inserts a node n into the priorty queue, Utalizing a min heap
* property. Ensures node with the lowest freq is always at the front
******************************************************************************/

void queueInsert (node n) {

    int j;
    /* Increments the queue end to allways be the last index of array */
    int i = queueEnd++;

    /* While loop calculates the index of the parent node j to the the
       current position index i if j > 0 then there is a parent node */

    while ((j = i / 2)) {
        
        /* Min Heap property is already maintained, break */
        if(firstElement[j]->freq <= n->freq) break;

        /* Swaps nodes and indexs' to ensure the node with the lowest freq is 
           first */
        firstElement[i] = firstElement[j], i = j;

    }
    
    firstElement[i] = n;

}

/*****************************************************************************
* Prototypes : Removes the node with the minimum frequency from the 
* priority queue
******************************************************************************/
node queueRemove() {

    int leftIndex;
    int i;
    
    node n = firstElement[i = 1];

    /* There are no nodes within the priorty queue returns */
    if(queueEnd < 2) {
        printf("There are no elements in the priority queue to remove\n");
        return 0;
    }

    /* Decrement the end of the queue by 1 */
    queueEnd--; 

    /* Downheaping function will maintain min-heap property */
    /* While loop will continue as long as the leftIndex is less than the 
       queueEnd */
    while((leftIndex = i * 2) < queueEnd) {
        
        /* leftIndex + 1 = index of right node 
           compares frequency of current node with the with freq of left
           and right child if the right child has the lowest freq it 
           will set the leftIndex to the right index to maintain 
           min-Heap property */
        if (leftIndex + 1 < queueEnd && 
        firstElement[leftIndex + 1]->freq < firstElement[leftIndex]->freq) 
        leftIndex++;

        /* Will set the node at the current position with the node
           of the lowest freq */
        firstElement[i] = firstElement[leftIndex];
        
        /* i is updated to the index of the node that was swaped */

        i = leftIndex;

        /* loop will continue until the current nodes frequency is 
           less than or equal to child nodes */

    }

    firstElement[i] = firstElement[queueEnd];
    
    return n;
}
/*****************************************************************************
* Prototypes : Recursive Function will traverse the huffman tree and store 
* the char codes. 
* Checks if if the node is a leaf node it will store the code into the code 
* array and if it is an internal node it will explore both child nodes. 
* Appends 0 for left child and 1 for right child.
******************************************************************************/
void buildCodeTree(node n, char *store, int length) {

    /* Pointer will retain value accross multiple function calls */
    static char *output = buffer;

    /* The base case will check if the current node is a leaf node
       if there is no data in the node it is a leaf node, a leaf node
       has a char stored in the data variable */
    if (n->data) {
        /* Assigns a null terminator to the end of the store array */
        store[length] = 0;
        /* Copies the code string constructed from store */
        strcpy(output, store); 
    
        /* Assigns the address stored in output (a pointer to the string code) 
           to an element in the code array */
        code[(int)n->data] = output;
        /* output pointer is incremented by the current code string length
           + 1 to account for the null terminator, this prepares the code
           to point to the next Memory location for the next code string */
        output += length + 1;
        
        return;
        
    }

    /* Recursive call if the node is not a leaf node (If it is a parent node)*/

    /* Adds a 0 for left child nodes to the
       store array at the current length */
    store[length] = '0';
    buildCodeTree(n->left, store, length + 1);
    /* Adds a 1 for right child nodes to the
       store array at the current length */
    store[length] = '1';
    buildCodeTree(n->right, store, length + 1);

}

/*****************************************************************************
* Prototypes : Function reads a file and will determine char frequency and
* and build a huffman tree and build code strings for the characters,
* parameters: pointer to a file representing the input file and a pointer tp 
* an array of ints that represent frequency of characters 
******************************************************************************/
void importFile(FILE *filepointer_in, unsigned int *freq) {

    char storeChar; /* Stores current char read from file */
    char store[20] = {0}; /* Stores code strings */
    int i = 0;

    printf("\n\nReading File...\n\n");
    /* Reads file char by char and increments frequecy until it reaches the end 
       of file */
    while((storeChar = fgetc(filepointer_in)) != EOF) {
        /* casts storeChar to int and is used as an index to access the freq 
           array*/
        freq[(int)storeChar]++;
        /* putchar(storeChar); */
        
    }
    
    /* node creation and queue insertion */

    /* for loop will create nodes if their is a char frequecy */
    for (i = 0; i < 128; i++) {
        /* create new nodes and insert them into the priority queue */
        if(freq[i]){
            queueInsert(newNode(freq[i], i, 0, 0));
        }
    }

    /* While loop will heapify the nodes */
    while (queueEnd > 2) {
        queueInsert(newNode(0, 0, queueRemove(), queueRemove()));
    }

    /* Build code table */
    buildCodeTree(firstElement[1], store, 0);

}

/******************************************************************************
* Prototypes : Encodes the file and inserts the characters to a new file
*******************************************************************************/
void encodeFunction (FILE* filepointer_in, FILE* filepointer_out, 
                     unsigned int *freq) {

    unsigned char input;
    char character;
    char tempBuffer[20] = {0};
    int i;
    int bufIndex = 0;
    int bitPos = 0;
    int numOfBits = 0;
    

    rewind(filepointer_in);

    /* For loop calculates the total number of bits required to store 
       encoded data */
    for(i = 0; i < 128; i++){
        if(freq[i]) numOfBits +=(freq[i] * strlen(code[i]));
    }

    outputData = numOfBits;
    /* Prints the number of bits to the begining of output file */
    fprintf(filepointer_out, "%d\n", numOfBits);
    
    printf("\n\nEncoding...\n\n");

    for(i = 0; i < numOfBits; i++) {

        /* if code storage reaches a null terminator it will store the code*/
        if(tempBuffer[bufIndex] == '\0') {

            input = fgetc(filepointer_in);

            strncpy(tempBuffer, code[input],sizeof(tempBuffer) - 1);

            bufIndex = 0;

            }
    
        /* Bitwise operation */
        if (tempBuffer[bufIndex] == '1') {

            character = character|(1<<(7 - bitPos));

        }

        else if(tempBuffer[bufIndex] == '0') {

            character = character|(0<<(7 - bitPos));
        }

        else {
            printf("Error Encoding \n");
        }

        bitPos++;
        bufIndex++;

        if (((i + 1) % 8 == 0) || (i == numOfBits - 1)) {

            bitPos = 0;
            fputc(character, filepointer_out);
            character = 0;
        }
    }

}

/******************************************************************************
* Prototypes : gets the length of file to store in array for writing to a 
* new file.
*******************************************************************************/
int dynamicArray (char *temp) {

    int fileArraySize;
    
    FILE *filepointer_out;
    
    filepointer_out = fopen(temp, "r");

    if(filepointer_out == NULL) {
        printf("Failed Dynamically Allocating Memory.\n");
        return 1;
    }

    fseek(filepointer_out, 0, SEEK_END);

    fileArraySize = ftell(filepointer_out);

    rewind(filepointer_out);


    if (fileArraySize == 0){
        printf("There is no information in file."
                "Failed dynamically allocating memory.\n");
        return 1;
    }
    
    fclose(filepointer_out);

    return fileArraySize;

}

