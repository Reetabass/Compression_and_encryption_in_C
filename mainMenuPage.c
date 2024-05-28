#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainMenuHeader.h"

/*switchboard for user to add files to
their account, view their database, or logout*/
void mainMenu(void) {
    int choice;

    while(choice != 4) {
        printf("\nMain Menu\n");
        printf("1 - Create a text file\n");
        printf("2 - Add a file\n");
        printf("3 - View database\n");
        printf("4 - Logout\n");
        printf("Enter a command\n>");
        scanf(" %d", &choice);

        /*debug*/
        logDebugINT(choice);

        switch(choice) {
            case 1:
                if(addText() == 1) {
                    printf("\nError writing new file.\n");
                };
                break;
            case 2:
                addFile();
                break;
            case 3:
                database();
                break;
            case 4:
                printf("Logging you out...\n");
                strcpy(username, "");
                break;
            default:
                printf("Unrecognized command\n");
                break;
        }
    }
}
/*adds file to user's file*/
void fileToUser(char fName[256]) {
    FILE *file;

    /*decrypt user file*/
    encrypt(username, username); 

    if((file = fopen(username, "a"))) {
        fprintf(file, "%s\n", fName);
        fclose(file);
    }
    else {
        printf("Error adding file to user's account.");
    }
    /*encrypt user file*/
    encrypt(username, username); 
}
/*allows user to enter text from the console,
then writes the text to a txt file, encrypts,
and saves it to the user's database*/
int addText(void) {
    char text[256], fName[256];
    FILE *file;
    int c;
    
    /*gets file name from user*/
    strcpy(fName, formatFileName());
    
    /*debug*/
    compareSTR(fName, "back");

    /*if file name is "back", break out*/
    if(strcmp(fName, "back") == 0) {
        return 0;
    }

    /*if file exists, throw an error*/
    if(!(file = fopen(fName, "w"))) {
        return 1;
    }

    printf("\nEnter text, or \"quit\" to finish typing.\n");

    /*while user does not enter "quit"*/
    while(strcmp(text, "quit") != 0) {
        printf(">");
        scanf(" %255[^\n]s", text);

        while((c = getchar()) != EOF && c != '\n');
        
        /*debug*/
        logDebugSTR(text);
        compareSTR(text, "quit");

        /*if entered text is not "quit", write to file*/
        if(strcmp(text, "quit") != 0) {
            fprintf(file, "%s\n", text);
        }
    }
    /*after user is finished writing to file,
    close, compress, and encrypt it*/
    fclose(file);
    compress(fName);
    encrypt(fName, username); 
    /*adds file to user database*/
    fileToUser(fName);

    return 0;
}
/*user inputs name of file in main directory,
code checks the last characters of the file
to determine file type(txt, pdf, png, etc), 
then encrypts/compresses depending on file type*/
void addFile(void) {
    char fName[256], fileType[5];
    FILE *file;
    int c, i, k = 0, check = 0;
    
    /*while file does not exist*/
    while(check == 0) {
        printf("\nEnter file name>");
        scanf(" %255[^\n]s", fName);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(fName);
        
        /*get last 4 chars of file name
        to determine if it is text or image*/
        if(strlen(fName) >= 4) {
            /*debug*/
            loopHeader();
            for(i = 0; i < strlen(fName); i++) {
                /*debug*/
                forLoop(i, fName[i]);
                if(i >= strlen(fName) - 4) {
                    fileType[k] = fName[i];
                    k++;
                }
            }
            /*if file is not .txt or .ppm, throw an error*/
            if(strcmp(fileType, ".txt") != 0 && strcmp(fileType, ".ppm") != 0) {
                printf("\nInvalid file type.");
            }
        }
        /*file must have at least 5 chars,
        EXAMPLE: "a.txt" is valid but ".txt" is not*/
        else if(strlen(fName) < 4) {
            printf("\nInvalid file name.");
            return;
        }

        /*if file exists*/
        if((file = fopen(fName, "r"))) {
            fclose(file);

            /*debug*/
            compareSTR(fileType, ".txt");
            compareSTR(fileType, ".ppm");

            /*if file is text, compress and encrypt*/
            if(strcmp(fileType, ".txt") == 0) {
                compress(fName);
                encrypt(fName, username);
                fileToUser(fName);
            }
            /*if file is image, only encrypt*/
            else if(strcmp(fileType, ".ppm") == 0) {
                /*https://en.wikipedia.org/wiki/Netpbm#File_formats
                https://stackoverflow.com/questions/69581117/how-to-read-images-using-c*/
                encrypt(fName, username);
                fileToUser(fName);
            }

            check = 1;
        }
        else {
            printf("File doesn't exist!\n");
            check = 0;
        }
    }
}
