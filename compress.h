#ifndef COMPRESSHEADER_H_
#define COMPRESSHEADER_H_


/*******************************************************************************
* Structs
*******************************************************************************/
typedef struct node_t {
    int freq;
    char data;
    struct node_t *left;
    struct node_t *right;

} *node;

void compress(char *text);
node newNode(int freq, char data, node a, node b);
void queueInsert(node n); 
node queueRemove();
void buildCodeTree(node n, char *store , int length);
void importFile(FILE *filepointer_in, unsigned int *freq);
void encodeFunction (FILE* filepointer_in, FILE* filepointer_out, unsigned int *freq);
int dynamicArray (char *temp);

#endif
