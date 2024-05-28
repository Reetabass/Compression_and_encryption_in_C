#ifndef MAINMENUHEADER_H_
#define MAINMENUHEADER_H_

/*declare external variables and functions*/
extern char username[11];
extern int mode;
extern char debugLog[256];
extern char* formatFileName(void);
extern void encrypt(const char* file, char *username);
extern void logDebugSTR(char* string);
extern void logDebugINT(int integer);
extern void compareSTR(char* first, char* second);
extern void compareINT(int first, int second);
extern void loopHeader(void);
extern void forLoop(int num, char val);

/*declare functions*/
void mainMenu(void);
void fileToUser(char fName[256]);
int addText(void);
void addFile(void);
void database(void);

void compress(char *text);

#endif
