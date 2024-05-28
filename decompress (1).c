#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"

/******************************************************************************
* Global Variables
*******************************************************************************/

int numNodesDe = 0;
int queueEndDe = 1;
struct node_tDe poolDe[256] = {{0}};
nodeDe priorityQDe[255];
nodeDe *firstElementDe = priorityQDe - 1;
char *codeDe[128] = {0};
char bufferDe[1024];

/*****************************************************************************
* Main
******************************************************************************/
 void decompress(char *text) {

    FILE *filepointer_table;
    FILE *filepointer_out;
    FILE *filepointer_compressed;
    char filename[256] = {0};
    char tableName[256] = {0};
    char temp[256] = {0};
    unsigned int freq[128] = {0};
    int fileArraySize;
    char *fileArray;

    strcpy(filename, text);
    strcpy(temp, text);
    strcat(temp, ".tmp");
    strcpy(tableName, text);
    strcat(tableName, ".codeTable");

    /* Opens file that is compressed */
    
    if ((filepointer_table = fopen(tableName, "r")) == NULL) {
        printf("Error opening table file. \n");
        return;
    }

    importTableDe(filepointer_table, freq);
    
    
    if ((filepointer_compressed = fopen(filename, "r")) == NULL) {
        printf("Error opening compressed file. \n");
        return;
    }

    if((filepointer_out = fopen(temp, "w")) == NULL) {
        printf("Error writing to file.");
        return;
    }

    decodeFunctionDe(filepointer_compressed, filepointer_out);
    
    fclose(filepointer_table);
    fclose(filepointer_compressed);
    fclose(filepointer_out);

    fileArraySize = dynamicArrayDe(temp);
    
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
    
   
    filepointer_compressed = fopen(filename, "w");
    
    if(filepointer_compressed == NULL) {
        printf("Error opening file.");
        free(fileArray);
        return;
    }

    fwrite(fileArray, 1, fileArraySize, filepointer_compressed);
    
    fclose(filepointer_compressed);
    free(fileArray);


    int removeNum;
    removeNum = remove(tableName);
    if(removeNum != 0) {
        printf("Error deleting tablefile.");
    }

    int removeNum2 = remove(temp);
    if(removeNum2 != 0) {
        printf("Error deleting tablefile.");
    }
    

 }

/******************************************************************************
* Prototypes
*******************************************************************************/
nodeDe newNodeDe(int freq, char data, nodeDe a, nodeDe b) {
    
    /* Assigns a new node from pool array & increments # of nodes */
    nodeDe n = poolDe + numNodesDe++;

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

/*******************************************************************************
* Prototypes : inserts a node n into the priorty queue, Utalizing a min heap
* property. Ensures node with the lowest freq is always at the front
*******************************************************************************/

void queueInsertDe (nodeDe n) {

    int j;
    /* Increments the queue end to allways be the last index of array */
    int i = queueEndDe++;

    /* While loop calculates the index of the parent node j to the the
       current position index i if j > 0 then there is a parent node */

    while ((j = i / 2)) {
        
        /* Min Heap property is already maintained, break */
        if(firstElementDe[j]->freq <= n->freq) break;

        /* Swaps nodes and indexs' to ensure the node with the lowest freq is 
           first */
        firstElementDe[i] = firstElementDe[j];
        i = j;

    }
    
    firstElementDe[i] = n;

}

/******************************************************************************
* Prototypes : removes the node with the minimum frequency from the 
* priority queue
*******************************************************************************/
nodeDe queueRemoveDe() {

    /**/
    int leftIndex;
    int i;
    
    nodeDe n = firstElementDe[i = 1];

    /* There are no nodes within the priorty queue returns */
    if(queueEndDe < 2) {
        printf("There are no elements in the priority queue to remove\n");
        return 0;
    }

    /* Decrement the end of the queue by 1 */
    queueEndDe--; 

    /* Downheaping function will maintain min-heap property */
    /* While loop will continue as long as the leftIndex is less than the 
       queueEnd */
    while((leftIndex = i * 2) < queueEndDe) {
        
        /* leftIndex + 1 = index of right node */
        /* compares frequency of current node with the with freq of left
           and right child if the right child has the lowest freq 
           it will set the leftIndex to the right index to maintain 
           min-Heap property */
        if (leftIndex + 1 < queueEndDe && 
        firstElementDe[leftIndex + 1]->freq < firstElementDe[leftIndex]->freq) 
        leftIndex++;

        /* Will set the node at the current position with the node
           of the lowest freq */
        
        firstElementDe[i] = firstElementDe[leftIndex];
        
        /* i is updated to the index of the node that was swaped */

        i = leftIndex;

        /* loop will continue until the current nodes frequency is 
           less than or equal to child nodes */

    }

    firstElementDe[i] = firstElementDe[queueEndDe];
    
    return n;
}

/******************************************************************************
* Prototypes : removes the node with the minimum frequency from the 
* priority queue
*******************************************************************************/
void importTableDe(FILE *filepointer_table, unsigned int *freq) {

    char storeChar;
    int i = 0;

    printf("\n\nImporting Table....\n\n");

    while((storeChar = fgetc(filepointer_table)) != EOF) {
        freq[i++] = (unsigned char)storeChar;

    }
    for (i = 0; i < 128; i++) {
        /* create new nodes and insert them into the priority queue */
        if(freq[i]){
            queueInsertDe(newNodeDe(freq[i], i, 0, 0));
        }
    }

    /* While loop will heapify the nodes */
    while (queueEndDe > 2) {
        queueInsertDe(newNodeDe(0, 0, queueRemoveDe(), queueRemoveDe()));
    }

}

/******************************************************************************
* Prototypes : removes the node with the minimum frequency from the 
* priority queue
*******************************************************************************/
void decodeFunctionDe(FILE *filepointer_compressed, FILE *filepointer_out) {
    
    int i = 0;
    int numOfBits = 0;
    int j = 0; 
    char storeChar;
    nodeDe n = firstElementDe[1];

    
    fscanf(filepointer_compressed, "%d", &numOfBits);
    
    fseek(filepointer_compressed, 1, SEEK_CUR);

    printf("\nDecompressing...\n\n");

    for(i = 0; i < numOfBits; i++) {

        if(j == 0) {
            storeChar = fgetc(filepointer_compressed);
        }

        if(storeChar&128) {
            n = n->right;
        }
        else {
            n = n->left;
        }

        if(n->data) {
            /* putchar(n->data); */ 
            fputc(n->data, filepointer_out);
            n = firstElementDe[1];
        }

        storeChar = storeChar<<1;
        if(++j>7){
            j = 0;
        }
    }
}

/******************************************************************************
* Prototypes : removes the node with the minimum frequency from the 
* priority queue
*******************************************************************************/
int dynamicArrayDe (char *temp) {

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
