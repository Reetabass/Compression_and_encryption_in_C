#ifndef DEBUGHEADER_H_
#define DEBUGHEADER_H_

/*declare external variables*/
extern int mode;
char debugLog[256];

/*declare functions*/
void logDebugSTR(char* string);
void logDebugINT(int integer);
void compareSTR(char* first, char* second);
void compareINT(int first, int second);
void loopHeader(void);
void forLoop(int num, char val);

/*handles debug string*/
void logDebugSTR(char* string) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("\nInputted Value: %s\n", string);

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "\nInputted Value: %s\n", string);
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}
/*handles debug int*/
void logDebugINT(int integer) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("\nInputted Value: %d\n", integer);

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "\nInputted Value: %d\n", integer);
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}
/*handles debug comparing strings*/
void compareSTR(char* first, char* second) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("\nInput: \"%s\" | Compare: \"%s\"\n", first, second);

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "\nInput: \"%s\" | Compare: \"%s\"\n", first, second);
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}
/*handles debug comparing ints*/
void compareINT(int first, int second) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("\nInput: %d | Compare: %d\n", first, second);

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "\nInput: %d | Compare: %d\n", first, second);
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}
/*handles debug loop header*/
void loopHeader(void) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("\nFOR LOOP:\n");

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "\nFOR LOOP:\n");
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}
/*handles debug loop info*/
void forLoop(int num, char val) {
    /*if in debug mode*/
    if(mode == 1) {
        printf("At %d | Value %c\n", num, val);

        /*if user wants to write to a file*/
        if(strcmp(debugLog, "-1") != 0) {
            FILE *file;
            /*open file and write info into it*/
            if((file = fopen(debugLog, "a"))) {
                fprintf(file, "At %d | Value %c\n", num, val);
                fclose(file);
            }
            else {
                printf("Error writing to debug log.\n");
            }
        }
    }
}

#endif
