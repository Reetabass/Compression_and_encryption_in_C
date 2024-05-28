#ifndef LOGINHEADER_H_
#define LOGINHEADER_H_

/*handle global var and functions*/
extern int mode;
extern void logDebugSTR(char* string);
extern void compareSTR(char* first, char* second);
extern void loopHeader(void);
extern void forLoop(int num, char val);

/*declare functions*/
void formatName(void);
void signup(void);
void login(void);
void mainMenu(void);
int indexFile(char password[11]);

void changeUser(void);
char* formatFileName(void);
void encrypt(const char* file, char *username);

/*prompts user to enter a file name
this handles checking if it is valid,
and adding ".txt" if the user forgets to*/
char* formatFileName(void) {
    char fileName[256], fileType[5];
    FILE *file;
    int c, i, k = 0, check = 0;
                        
    /*while user has not entered valid file name*/
    while(check == 0) {
        printf("\nEnter file name or \"back\"\n>");
        scanf(" %255[^\n]s", fileName);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(fileName);
        compareSTR(fileName, "back");

        /*if user enters "back"*/
        if(strcmp(fileName, "back") == 0) {
            char *returnSTR = fileName;
            return returnSTR;
        }
                            
        /*if entered file name is greater than 4*/
        if(strlen(fileName) > 4) {
            /*debug*/
            loopHeader();

            /*get last four characters*/
            for(i = 0; i < strlen(fileName); i++) {
                if(i >= strlen(fileName) - 4) {
                    /*debug*/
                    forLoop(i, fileName[i]);

                    fileType[k] = fileName[i];
                    k++;
                }
            }

            /*debug*/
            compareSTR(fileType, ".txt");

            /*if last four aren't ".txt", add it*/
            if(strcmp(fileType, ".txt") != 0) {
                strcat(fileName, ".txt");
            }
        }
        /*if too short to have ".txt", add it*/
        else if(strlen(fileName) >= 1) {
            strcat(fileName, ".txt");
        }
        else {
            printf("\nInvalid file name.");
        }

        /*check if file exists*/
        if((file = fopen(fileName, "r"))) {
            printf("File already exists!\n");
            fclose(file);
        }
        else {
            if(!(file = fopen(fileName, "w"))) {
                printf("Error creating file.\n");
            }
            check = 1;
        }
    }
    char *returnSTR = fileName;
    return returnSTR;
}
/*handles encryption*/
void encrypt(const char* file, char *username) {
    /*put encrypt code here*/
    int i = 0, filesize;
    size_t bytesread;
    char key[strlen(username)];

    strcpy(key, username);

    char *ciphertext, *plaintext;

    FILE *inputfile = fopen(file, "r");
    if(inputfile == NULL){
        printf("Failed to read file\n");
        return;
    }
        
    fseek(inputfile, 0, SEEK_END);
    filesize = ftell(inputfile);
    rewind(inputfile);
    plaintext = (char*)malloc(filesize);
    ciphertext = (char*)malloc(filesize);

    bytesread = fread(plaintext, 1, filesize, inputfile);
    if (bytesread < filesize) {
        printf("Error reading file\n");
        fclose(inputfile);
        free(plaintext);
        free(ciphertext);
        return;
    }
    else {
        fclose(inputfile);
    }

    /*debug*/
    loopHeader();
    while(i < filesize){
        ciphertext[i] = plaintext[i] ^ key[i % strlen(key)]; 
        /*debug*/
        forLoop(i, ciphertext[i]);
        i++;
    }

    inputfile = fopen(file, "w");
    if(inputfile == NULL){
        printf("Failed to write to file\n");
        return;
    }

    fwrite(ciphertext, sizeof(char), filesize, inputfile);
    free(plaintext);
    free(ciphertext);
    rewind(inputfile);
    fclose(inputfile);
    return;
}

#endif
