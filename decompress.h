#ifndef DECOMPRESSHEADER_H_
#define DECOMPRESSHEADER_H_

/*******************************************************************************
* Structs
*******************************************************************************/
typedef struct node_tDe {
    int freq;
    char data;
    struct node_tDe *left;
    struct node_tDe *right;

} *nodeDe;

/*******************************************************************************
* Prototype decleration (Will move to header file)
*******************************************************************************/

void decompress(char *text);
nodeDe newNodeDe(int freq, char data, nodeDe a, nodeDe b);
void queueInsertDe(nodeDe n); 
nodeDe queueRemoveDe();
void importTableDe();
void decodeFunctionDe();
int dynamicArrayDe (char *temp);

#endif
