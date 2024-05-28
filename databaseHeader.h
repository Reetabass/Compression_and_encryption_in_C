#ifndef DATABASEHEADER_H_
#define DATABASEHEADER_H_

/*declare external variables and functions*/
extern char username[11];
extern int mode;
extern char debugLog[256];
extern void encrypt(const char* file, char *username);
extern void logDebugSTR(char* string);
extern void logDebugINT(int integer);
extern void compareSTR(char* first, char* second);
extern void compareINT(int first, int second);
extern void loopHeader(void);
extern void forLoop(int num, char val);

/*declare functions*/
void database(void);
int read(void);
void viewDatabase(void);
int search(void);
void alphabeticsort(void);
void filesizesort(void);
void removefile(char *inputFile);
void decompress(char *text);

#endif
