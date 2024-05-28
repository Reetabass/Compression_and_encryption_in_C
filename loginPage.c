#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "loginHeader.h"
#include "debugHeader.h"

/*global variables*/
/*username, used to access user data on all pages*/
char username[11];
/*mode, determined from command line: default, debug, admin*/
int mode;
/*name of debug file, if in debug mode and debug file entered it will write to it*/
extern char debugLog[256];

/*main handles mode selection and login/signup page info
determines what mode user wants to be in and modifies
global variables according to selection. Switchboard
allows user to log into an account, create an account,
or quit*/
int main(int argc, char* argv[]) {
    /*if there is only one argument in console,
    just initialize in default mode*/
    if(argc == 1) {
        printf("WARNING: No mode selected!\n");
        printf("Running in default mode.\n");
        mode = 0;
    }
    /*if there is two arguments,
    handle argument choices*/
    else if(argc == 2) {
        /*convert argument to int*/
        mode = atoi(argv[1]);

        switch(mode) {
            /*enter default mode*/
            case 0:
                printf("Running in default mode.\n");
                break;
            /*enter debug mode*/
            case 1:
                char choice;
                int valid = 0;

                printf("Running in debug mode.\n");

                /*while user has not entered y or n*/
                while(valid == 0) {
                    printf("Would you like to create a debug log? (y/n)\n>");
                    scanf(" %c", &choice);

                    if(choice == 'y') {
                        /*set global variable debugLog to
                        text file entered by user*/
                        char* temp = formatFileName();
                        /*if entered file is not "back"*/
                        if(strcmp(temp, "back") != 0) {
                            strcpy(debugLog, temp);
                            valid = 1;
                        }
                        /*if entered file is "back"*/
                        else {
                            strcpy(debugLog, "-1");
                            valid = 1;
                        }
                    }
                    else if(choice == 'n') {
                        strcpy(debugLog, "-1");
                        valid = 1;
                    }
                    else {
                        printf("\nInvalid choice, please enter y or n.\n");
                    }
                }
                break;
            /*enter admin mode*/
            case 2:
                printf("Running in admin mode.\n");
                
                /*while user has not entered "quit"*/
                while(strcmp(username, "quit") != 0) {
                    FILE *file;
                    int c;

                    /*enter username that user wants to access*/
                    printf("\nEnter username or \"quit\"\n>");
                    scanf(" %10[^\n]s", username);

                    while((c = getchar()) != EOF && c != '\n');

                    formatName();

                    /*if entered username is not quit and user does not exist*/
                    if(strcmp(username, "quit") != 0 && !(file = fopen(username, "r"))) {
                        printf("\nUser doesn't exist.");
                    }
                    /*if entered username is not quit and user does exist*/
                    else if(strcmp(username, "quit") != 0) {
                        fclose(file);
                        mainMenu();
                    }
                }
                printf("\nClosing...\n");
                return 0;
            /*invalid mode entered*/
            default:
                printf("ERROR: Invalid mode selected!\n");
                printf("MODES:\n");
                printf("0 - Default\n");
                printf("1 - Debug\n");
                printf("2 - Admin\n");
                printf("\nClosing...\n");
                return 0;
        }
    }
    /*more than two arguments*/
    else {
        printf("ERROR: Too many arguments!\n");
        printf("USAGE: ./assign3.out <mode>\n");
        exit(1);
    }

    int choice;

    /*switchboard, allows user to 
    sign up, login, or quit.
    if debug, lets user change
    debug file*/
    while(choice != 3) {
        printf("\nEncrypted File System\n");
        printf("1 - Signup\n");
        printf("2 - Login\n");
        printf("3 - Quit\n");
        if(mode == 1) {
            printf("4 - New Debug Log\n");
        }
        printf("Enter a command\n>");
        scanf(" %d", &choice);

        /*debug*/
        logDebugINT(choice);

        switch(choice) {
            case 1:
                signup();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Closing...\n");
                break;
            case 4:
                /*if user is in debug mode*/
                if(mode == 1) {
                    /*user enters new debug file name*/
                    char* temp = formatFileName();
                    /*if not "back" set new debug file*/
                    if(strcmp(temp, "back") == 1) {
                        strcpy(debugLog, temp);
                    }
                    else {
                        strcpy(debugLog, "-1");
                    }
                }
                /*if not in debug mode*/
                else {
                    printf("Unrecognized command\n");
                }
                break;
            default:
                printf("Unrecognized command\n");
                scanf("%*[^\n]");
                break;
        }
    }
    return 0;
}
/*formats username to replace "." and " "
with "_". this is done to avoid users 
affecting how data is stored by having
".txt", ".pdf", etc. at the end of their
username*/
void formatName(void) {
    int i;

    /*debug*/
    loopHeader();
    /*loop through username char by char
    if char matches "." or " ", replace
    with "_" char*/
    for(i = 0; i < strlen(username); i++) {
        /*debug*/
        forLoop(i, username[i]);
        if(username[i] == ' ' || username[i] == '.') {
            username[i] = '_';
        }
    }
}
/*allows user to create an account by 
entering a username and password,
then sends user to main menu page*/
void signup(void) {
    char password[11], confirmPswrd[11];
    int viable = 0, c;

    /*while entered username doesn't already exist*/
    while(viable == 0) {
        FILE *file;

        printf("\nPlease enter a username\n>");
        scanf(" %10[^\n]s", username);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(username);

        /*formats username*/
        formatName();

        /*checks if username exists*/
        if((file = fopen(username, "r"))) {
            printf("\nUsername already exists.");
            fclose(file);
        }
        else {
            viable = 1;
        }
    }

    /*reset variable for reuse*/
    viable = 0;

    printf("\nEnter a password\n>");
    /*while entered password doesn't match confirm password*/
    while(viable == 0) {
        scanf(" %10[^\n]s", password);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(password);

        /*confirm password*/
        printf("\nConfirm password\n>");
        scanf(" %10[^\n]s", confirmPswrd);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(confirmPswrd);
        compareSTR(confirmPswrd, password);

        /*check if passwords match*/
        if(strcmp(password, confirmPswrd) == 0) {
            printf("\nPasswords match.");
            viable = 1;
        }
        else {
            printf("\nPassword does not match.");
            printf("\nEnter a password\n>");
        }
    }

    printf("\nCreating account...");
    /*adds password to user file*/
    if(indexFile(password) == 0) {
        printf("\nAccount created. Logging you in...\n");
        mainMenu();
    }
    else {
        printf("\nError creating account.\n");
    }
}
/*user login page*/
void login(void) {
    char password[11], filePswrd[11];
    int viable = 0, i, c;
    FILE *file;

    /*while username and password are invalid*/
    while(viable == 0) {
        printf("\nUsername>");
        scanf(" %10[^\n]s", username);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(username);

        /*format username*/
        formatName();

        /*decrypt user file*/
       encrypt(username, username);

        /*check if username exists*/
        if((file = fopen(username, "r"))) {
            /*get password, reencrypt*/
            fgets(filePswrd, 11, file);
            fclose(file);
            encrypt(username, username); 

            /*ignore newline characters and spaces*/
            for(i = 0; i < strlen(filePswrd); i++) {
                if(filePswrd[i] == '\n') {
                    filePswrd[i] = '\0';
                }
                else if(filePswrd[i] == ' ') {
                    filePswrd[i] = '\0';
                }
            }
            viable = 1;
        }
        else {
            printf("\nUser does not exist, please try again.");
        }
    }

    viable = 0;

    printf("Password>");
    /*while password does not match*/
    while(viable == 0) {
        scanf(" %10[^\n]s", password);

        while((c = getchar()) != EOF && c != '\n');

        /*debug*/
        logDebugSTR(username);
        compareSTR(password, filePswrd);

        /*check if passwords match*/
        if(strcmp(filePswrd, password) == 0) {
            viable = 1;
        }
        else {
            printf("\nIncorrect password, please try again.\n");
            printf("\nWARNING: Program crashes may corrupt user data!\n");
            printf("\nPassword>");
        }
    }

    printf("\nLogging you in...\n");
    mainMenu();
}
/*adds user to system. creates a file
whose name matches username, adds password,
and encrypts the file*/
int indexFile(char password[11]) {
    FILE *file;
    if((file = fopen(username, "w"))) {
        fprintf(file, "%s\n", password);
        fclose(file);

        encrypt(username, username);
        return 0;
    }
    else {
        return 1;
    }
}
