#include <stdio.h> 
/* fseek ,ftell, rewind, fopen, fclose, 
getchar, fprintf, fread, fwrite, fscanf*/

#include <stdlib.h> /*malloc, free*/
#include <string.h> /*strcpy, strcmp, strlen*/
#include "databaseHeader.h"

/*switchboard that allows user to
read encrypted/compressed files, 
view files in database, search,
sort, and return to menu*/
void database(void) {
    int choice;

    while(choice != 5) {
        printf("\nDatabase Management\n");
        printf("1 - Read file\n");
        printf("2 - Remove file\n");
        printf("3 - View all files\n");
        printf("4 - Search\n");
        printf("5 - Sort\n");
        printf("6 - Back\n");
        printf("Enter a command\n>");
        scanf(" %d", &choice);

        /*debug*/
        logDebugINT(choice);

        switch(choice) {
            case 1:
                read();
                break;
            case 2:
                removefile("-1");
                break;
            case 3:
                viewDatabase();
                break;
            case 4:
                search();
                break;
            case 5:
                int sortMode = 0;
                while(sortMode != 3) {
                    printf("\nSort:\n");
                    printf("1 - Alphabetically\n");
                    printf("2 - By file size\n");
                    printf("3 - Back\n");
                    printf("Enter a command\n>");
                    scanf(" %d", &sortMode);

                    /*debug*/
                    logDebugINT(sortMode);

                    switch(sortMode) {
                        case 1:
                            alphabeticsort();
                            break;
                        case 2:
                            filesizesort();
                            break;
                        case 3:
                            printf("Backing out to database menu...\n");
                            break;
                        default:
                            printf("Unrecognized command\n");
                            scanf("%*[^\n]");
                            break;
                    }
                }
                break;
            case 6:
                printf("Backing out to main menu...\n");
                return;
            default:
                printf("Unrecognized command\n");
                scanf("%*[^\n]");
                break;
        }
    }
}
/*allows user to input exact file name,
if text, print either the whole or first
section to console, confirms if user wants
to see the full file. if yes, decrypt and
save text file to main directory for user
to view. same with image, but no preview 
and no prompt.*/
int read(void){
    char filename[256], fName[256]; 
    FILE *userFile;
    int i, c, pointinlist = 0, pointinfilenamearray;
    char* filenamearray;
    size_t filesize;
    int fileExists = 0;

    printf("\nEnter name of file\n>");
    scanf(" %255[^\n]s", fName);

    while((c = getchar()) != EOF && c != '\n');

    encrypt(username, username); 

    if((userFile = fopen(username, "r"))) {
        
        /*This peice of code up to the forloop copies the text data from
        the file into an array that holds that text the filesize is found
        by seeking the end of the file and putting the value into a 
        variable. Then memory is alocated of that variables size*/
    
        fseek(userFile, 0, SEEK_END);
        filesize = ftell(userFile);
        rewind(userFile);
        if(filesize == 0){
            printf("\nNo files in database\n");
            fclose(userFile);
            encrypt(username, username);
            return 0;
        }
        
        filenamearray = (char*)malloc(filesize);
        if(filenamearray == NULL) {
            printf("\nMemory allocation failed\n");
            fclose(userFile);
            encrypt(username, username);
            return 1;
        }
        /*reads data from file to filenamearray skips the first line so that 
        the username is not read*/
        fscanf(userFile, "%*[^\n]\n");
        filesize = fread(filenamearray, 1, filesize, userFile);
        
        if(filesize == 0){
            printf("\nNo files in database\n");
            fclose(userFile);
            free(filenamearray);
            encrypt(username, username); 
            return 0; 
        }

        /*This for loop iterates until it has either gone through the whole
        array containing the file or until it has found the file and fileExists 
        is 1. 
        
        If that is the case it prints confirmation that the file exists.
        
        If it gets to the end of the file and file exists does not 
        equal to one then it prints that the file does not exist.*/
        
        for(i = 0; pointinlist < filesize && fileExists != 1; i++){
            pointinfilenamearray = 0;
            /*while loop extracts the strings from the array containing all
            the users filenames*/
            while(filenamearray[pointinlist] != '\n'){
                filename[pointinfilenamearray] = filenamearray[pointinlist];
        
                pointinfilenamearray++;
                pointinlist++;
                
            }
            filename[pointinfilenamearray] = '\0';
            pointinlist++;

            /*debug*/
            compareSTR(fName, filename);

            /* If the string matches the input */
            if(strcmp(fName, filename) == 0){
                char fileType[5];
                int m, k = 0;

                printf("\nFile with name %s exists\n", fName);

                /*debug*/
                loopHeader();
                    
                /*get last 4 chars of file name
                to determine if it is text or image*/
                for(m = 0; m < strlen(fName); m++) {
                    /*debug*/
                    forLoop(m, fName[m]);
                    if(m >= strlen(fName) - 4) {
                        fileType[k] = fName[m];
                        k++;
                    }
                }
                /*check file type*/
                if(strcmp(fileType, ".txt") == 0) {
                    encrypt(fName, username);
                    decompress(fName);
                    fclose(userFile);
                    encrypt(username, username);
                    removefile(fName);
                }
                else if(strcmp(fileType, ".ppm") != 0) {
                    encrypt(fName, username);
                    fclose(userFile);
                    encrypt(username, username);
                    removefile(fName);
                }
                else {
                    printf("\nError: invalid file type\n");
                    fclose(userFile);
                    encrypt(username, username);
                }
                fileExists = 1;
            }
        }
        /* If the loop has finished and no file names match the input */
        if(fileExists != 1){
            printf("\nFile does not exist\n");
            fclose(userFile);
            encrypt(username, username);
        }
        free(filenamearray);  
    }
    else {
        printf("\nError opening file\n");
        encrypt(username, username);
    }
    return 0;
} 
/*allows user to view all the
files in the database by filename,
viewing 10 at a time and allowing user
to navigate pages*/
void viewDatabase(void) {
    int i, choice, prevpointscount = 0, pointinlist = 0, pointinfilenamearray;
    int prevpointsinlist[9];
    char* filenamearray;
    size_t filesize;
    char filename[256];


    FILE* file = fopen(username, "r");
    if(file == NULL){
        printf("Failed to open file\n");
        return;  
    }
    /*This peice of code up to the forloop copies the text data from
    the file into an array that holds that text. The filesize is found
    by seeking the end of the file telling that value
    and putting the value into a variable. 
    Then memory is alocated of that variables size, which is the size 
    of the file*/

    encrypt(username, username);
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    if(filesize == 0){
        printf("No files in database\n");
        fclose(file);
        encrypt(username, username);
        return;
    }
    
    filenamearray = (char*)malloc(filesize);
    if(filenamearray == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        encrypt(username, username); 
        return;
    }

    /*reads data from file to filenamearray skips the first line so that the 
    username is not read*/
    fscanf(file, "%*[^\n]\n");
    filesize = fread(filenamearray, 1, filesize, file);
    filenamearray[filesize] = '\0'; 
    
    if(filesize == 0){
        printf("No files in database\n");
        fclose(file);
        free(filenamearray);
        encrypt(username, username); 
        return; 
    }

    fclose(file);
    encrypt(username, username);

    choice = 1;
    while(choice != 3){
        
        if(choice != 1){
            printf("1 - Next page\n");
            printf("2 - Previous page\n");
            printf("3 - Back\n");
            printf("Enter a command\n>");
            scanf("%d", &choice);
        }

        logDebugINT(choice);

        /* Gives the user the choice to go to the next page previous page 
        or exit the page */
        switch(choice){
            case 1:
            /*Pointinlist is the counter that keeps track of the location 
            of where the user is in the filenamearray, which is the array 
            where the names of the users files are copied to. 
            
            prevpointsinlist is an array that holds the values that mark 
            the start of each block of ten filenames. 
            This allows the program to navigate back to the block of filenames. 
            Which block of filenames the program needs to print is given by 
            prevpointscount*/

                if(pointinlist == filesize){
                    /*In the case there are no more files to print, 
                    print out current block of filenames again and
                    print error */
                        printf("\nThere are no files after these\n");
                        pointinlist = prevpointsinlist[prevpointscount];
                        break;
                }
                else{
                    /*go to next block of filenames */
                    prevpointscount++;
                    break;
                }
            case 2:
                if(prevpointscount == 1){
                    /*In the case there are no previous files to print, 
                    print out current block of filenames again and
                    print error */
                    printf("\nThere are no files previous to these\n");
                    pointinlist = prevpointsinlist[prevpointscount];
                    break;
                }
                else{
                    /*go back to the previous block of filenames */
                    prevpointscount = prevpointscount - 1;
                    pointinlist = prevpointsinlist[prevpointscount];
                    break;
                }
            case 3:
                printf("\nExiting database...\n");
                free(filenamearray);  
                return;
            default:
                printf("\nInvalid input\n");
                pointinlist = prevpointsinlist[prevpointscount];
                scanf("%*[^\n]");
                break;
        }
        prevpointsinlist[prevpointscount] = pointinlist;
        choice = 0;

        loopHeader();

        /*The for loop prints each string ten from the 
        filenamearray or less than ten if there are no more files after that.*/
        for(i = 0; i < 10 && pointinlist < filesize; i++){
            forLoop(i, filenamearray[pointinlist]);
            pointinfilenamearray = 0;
            /*The while loop writes individual strings from the array with all
            the users files into a temp variable so that it may be printed*/
            while(filenamearray[pointinlist] != '\n'){
                filename[pointinfilenamearray] = filenamearray[pointinlist];
                pointinfilenamearray++;
                pointinlist++;
            }
            /* Null terminates the string before printing*/
            filename[pointinfilenamearray] = '\0';
            printf("%s\n", filename);
            /*Increment to skip newline character*/
            pointinlist++;
        }

        printf("\n");
    
    }
    free(filenamearray);  
    return;
}
/*user enters partial file name,
search algorithm goes through the users database
and prints names of all files with matching names*/
int search(void) {

    int i, j, choice, typechoice, filetypechoose = 0, match, searchlen;
    int pointinlist = 0, pointinfilenamearray, filenamelen, anyfiles, check;
    char* filenamearray;
    size_t filesize;
    char filename[256], searchletters[20];

    encrypt(username, username);
    /*Retreiving contents of the userfile very similar to other
    functions that use or manipulate userfile. 
    see veiwDatabase function line 147*/
    FILE* file = fopen(username, "r");
    if(file == NULL){
        printf("Failed to open file\n");
        fclose(file);
        encrypt(username, username);
        return 1;  
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    if(filesize == 0){
        printf("No files in database\n");
        fclose(file);
        encrypt(username, username);
        return 0;
    }
    
    filenamearray = (char*)malloc(filesize);
    if(filenamearray == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        encrypt(username, username);
        return 1;
    }
    fscanf(file, "%*[^\n]\n");
    filesize = fread(filenamearray, 1, filesize, file);
    filenamearray[filesize] = '\0'; /*maybe this will fix it */
    
    if(filesize == 0){
        free(filenamearray);
        printf("No files in database");
        return 0; 
    }

    fclose(file);
    encrypt(username, username);
    choice  = 1;
    
    while(choice != 3 || typechoice != 4){
        
        /*If statement to make it so that file type choose do loop only
        runs when the function is first run and when user wants to go back to 
        filetype selection*/
        if(filetypechoose == 0){
            do{
                printf("1 - All files\n");
                printf("2 - Text files\n");
                printf("3 - PPM files\n");
                printf("4 - Exit\n");
                printf("Enter your choice\n>");
                scanf("%d", &typechoice);

                logDebugINT(typechoice);
                /*choice = 1 is so that the second set of switch conditions 
                is automatically 1 for the first search*/
                check = 0;
                switch(typechoice){
                
                    case 1: 
                    printf("Searching through all files\n");
                    filetypechoose = 1;
                    choice = 1;
                    break;
                
                    case 2: 
                    printf("Searching for text files\n");
                    filetypechoose = 1;
                    choice = 1;
                    break;

                    case 3: 
                    printf("Searching for jpg files\n");
                    filetypechoose = 1;
                    choice = 1;
                    break;

                    case 4:
                    printf("Exiting file search\n");
                    free(filenamearray);
                    return 0;

                    default:
                    printf("Invalid input\n");
                    scanf("%*[^\n]");
                    check = 1;
                    break;                
                }        
            }while(check == 1);
        }

    do{
        /*menu only prints if the user hasnt just been at the 
        previous menu and selected a type to search for, to reduce redundancy*/
        if(choice != 1){
            printf("\n\nTo search type, 1\n");
            printf("To go back to filetype selection, type 2\n");
            printf("To exit database, type: 3\n");
            printf("Enter your choice>");
            scanf("%d", &choice);
        }

        logDebugINT(choice);
        
            check = 0;
            switch(choice){

                case 1:

                    printf("Enter the first characters");
                    printf("of the file you wish to find\n");
                    printf("Max of 15 characters>");
                    scanf(" %15[^\n]s", searchletters);
                    searchlen = strlen(searchletters);
                    scanf("%*[^\n]");                     
                    pointinlist = 0;
                    break;

                case 2:
                /*goes back to filetpye choose on next loop since if statement
                is satisfied */
                    printf("Returning to filetype selection\n");
                    filetypechoose = 0;
                    break;
                            
                case 3:
                printf("Exiting file search\n");
                free(filenamearray);
                return 0;
            
                default:
                printf("Invalid input\n");
                scanf("%*[^\n]");
                check = 1;
                break;

                }        
        }while(check == 1);

    choice = 0;
    anyfiles = 0;
    
        printf("Files starting with --> %s\n\n", searchletters);
        /* for loop goes through every filename and evaluates it one by one*/
        for(i = 0; pointinlist < filesize; i++){
            pointinfilenamearray = 0;
            /* while loop seperates each filename in the array that the file
            was copied into*/
            while(filenamearray[pointinlist] != '\n'){
                filename[pointinfilenamearray] = filenamearray[pointinlist];
                pointinfilenamearray++;
                pointinlist++;
            }
            filename[pointinfilenamearray] = '\0';
            filenamelen = strlen(filename); 
            pointinlist++;

            match = 0;
            /* Which if loop runs depends on the users choice of file they are 
            searching for. Doesn't print the filename if there is a difference
            in the first letters of the file name and the letters of the
            search*/
            if(typechoice == 1){
                loopHeader();
                /*each forloop checks that there are anymore search letters or
                letters in the filename left to match if not the loop
                terminates*/
                for(j = 0; j < searchlen && j < filenamelen; j++){
                    forLoop(j, searchletters[j]);
                    if(filename[j] != searchletters[j]){
                        match = 1;
                        }
                    }
                }

            if(typechoice == 2){
                loopHeader();
                for(j = 0; j < searchlen && j < filenamelen; j++){
                    forLoop(j, searchletters[j]);
                    if(filename[j] != searchletters[j]){
                        match = 1;
                        }
                    }

                    if(filename[filenamelen - 3] != 't' 
                    || filename[filenamelen - 2] != 'x' 
                    || filename[filenamelen - 1] != 't'){
                        match = 1;
                    }
                }

            if(typechoice == 3){
                loopHeader();
                for(j = 0; j < searchlen && j < filenamelen; j++){
                    forLoop(j, searchletters[j]);
                    if(filename[j] != searchletters[j]){
                        match = 1;
                        }
                    }
                    if(filename[filenamelen - 3] != 'p' 
                    || filename[filenamelen - 2] != 'p' 
                    || filename[filenamelen - 1] != 'm'){
                        match = 1;
                    }
                }


            if(match != 1){
                printf("%s ", filename);
                anyfiles = 1;
                }
        
            }
            /*If no files match the seach then print*/
            if(anyfiles == 0){
                printf("No files under %s\n", searchletters);
                }    
    }
     free(filenamearray);
     encrypt(username, username); 
    return 0;
}
/*allows user to sort database by
file size, then prints database 
like viewDatabase function*/
void filesizesort(void){
    
    struct{
        char filenamesToSort[256];
        size_t eachfilesize;

    }typedef filelist_t;

    int i, k, j = 0;
    int pointinlist = 0, pointinfilenamearray;
    char* filenamearray;
    size_t filesize;
    char filename[256];
    filelist_t filenamestructarr[256], temp;
    FILE* usersfile;
    char firstline[255];

/*Retreiving contents of the userfile very similar to other
functions that use or manipulate userfile. 
see veiwDatabase function line 147*/
    encrypt(username, username);
    FILE* file = fopen(username, "r");
    if(file == NULL){
        encrypt(username, username);
        printf("Failed to open file\n");
        return;  
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    if(filesize == 0){
        encrypt(username, username);
        printf("No files in database\n");
        return;
    }
    
    filenamearray = (char*)malloc(filesize);
    if(filenamearray == NULL) {
        encrypt(username, username);
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }
    fscanf(file, "%255[^\n]s", firstline);
    filesize = fread(filenamearray, 1, filesize, file);
    
    if(filesize == 0){
        encrypt(username, username);
        free(filenamearray);
        printf("No files in database");
        return; 
    }
    encrypt(username, username);
    fclose(file);

        /*For loop runs until every filename has been taken out of the array
    containing the userfile and placed into a struct*/
     for(i = 0; pointinlist < filesize; i++){
        /*The while loop writes individual strings from the array with all
            the users files into a temp variable so that it may be printed*/
            pointinfilenamearray = 0;
            while(filenamearray[pointinlist] != '\n'){
                filename[pointinfilenamearray] = filenamearray[pointinlist];
                pointinfilenamearray++;
                pointinlist++;
            }
            filename[pointinfilenamearray] = '\0';
            pointinlist++;
            
            usersfile = fopen(filename, "r");

            /*Tests if each file actually exists and can be opened.
            If not then print that the file couldn't be opened and
            continue */
            if(usersfile == NULL){
                printf("Failed to open the file < %s >\n", filename);
                continue;
            }
            else{
            /*find the filesize and copy to an array */
            fseek(usersfile, 0, SEEK_END);
            filenamestructarr[j].eachfilesize = ftell(usersfile);
            /*copy the filename to the same struct in the array as 
            the filesize of the file*/
            strcpy(filenamestructarr[j].filenamesToSort, filename);
            j++;
            }
            fclose(usersfile);
     }
    /* A bubble sort algorithm to rearange the files in the struct so that
    they are printed from smallest to largest filesize*/
     for(i = 0; i < j; i++){
        for(k = i; k < j; k++)
            if(filenamestructarr[k].eachfilesize <
            filenamestructarr[i].eachfilesize){
                temp = filenamestructarr[k];
                filenamestructarr[k] = filenamestructarr[i];
                filenamestructarr[i] = temp;
            }       
        
     }
     /*print the arranged list of filenames to the terminal by iterating 
     through the array containing the arranged filenames*/
     printf("List of files by filesize smallest to largest\n");
     for(i = 0; i < j; i++){
        printf("%s\n", filenamestructarr[i].filenamesToSort);
     }
    free(filenamearray);
    return;
}

/*allows user to sort database 
alphabetically, then prints database 
like viewDatabase function*/
void alphabeticsort(void){
    
    struct{
        char filenamestosort[256];
        int alphabetplace;
    }typedef filenamestruct_t;

    filenamestruct_t filenamestructarr[255], temp;
    int i, k, j = 0;
    int pointinlist = 0, pointinfilenamearray;
    char* filenamearray;
    size_t filesize;
    char filename[256];
    char firstline[255];

    /*Retreiving contents of the userfile very similar to other
    functions that use or manipulate userfile. 
    see veiwDatabase function line 147*/
    encrypt(username, username);
    FILE* file = fopen(username, "r");
    printf("%s", username);
    if(file == NULL){
        encrypt(username, username);
        printf("Failed to open file\n");
        return;  
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    if(filesize == 0){
        encrypt(username, username);
        printf("No files in database\n");
        return;
    }
    
    filenamearray = (char*)malloc(filesize);
    if(filenamearray == NULL) {
        encrypt(username, username);
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }
    fscanf(file, "%255[^\n]s", firstline);
    filesize = fread(filenamearray, 1, filesize, file);
    
    if(filesize == 0){
        free(filenamearray);
        encrypt(username, username);
        printf("No files in database");
        return; 
    }
    encrypt(username, username);
    fclose(file);
    
    /*For loop runs until every filename has been taken out of the array
    containing the userfile and placed into a */
     for(i = 0; pointinlist < filesize; i++){
            pointinfilenamearray = 0;
            /*The while loop writes individual strings from the array with all
            the users files into a temp variable so that it may be stored 
            in the filename structure array*/
            while(filenamearray[pointinlist] != '\n'){
                filename[pointinfilenamearray] = filenamearray[pointinlist];
        
                pointinfilenamearray++;
                pointinlist++;
            }
            filename[pointinfilenamearray] = '\0';
            pointinlist++;
            
            /*If and else loops make it so that lower and uppercase letters
            can be sorted together. ascii values are recorded in The
            alphabetplace variable inside the struct.
            Files starting with uppercase letters have 32 added to their
            value so that they are the same as their lowercase counterpart*/
            strcpy(filenamestructarr[i].filenamestosort, filename);
            if(filenamestructarr[i].filenamestosort[0] < 97){
                filenamestructarr[i].alphabetplace = 
                filenamestructarr[i].filenamestosort[0] + 32;
            }
            else{
                filenamestructarr[i].alphabetplace = 
                filenamestructarr[i].filenamestosort[0];
            }
            /*j is a counter for the number of filenames that have been 
            processed. This is used in the bubblesort termination conditions*/
            j++;
            }
    /*Bubblesort algorithm to arrange the structs from lowest alphabetplace
    to highest alphabetplace */
     for(i = 0; i < j; i++){
        for(k = i; k < j; k++)
            if(filenamestructarr[k].alphabetplace <
            filenamestructarr[i].alphabetplace){
                temp = filenamestructarr[k];
                filenamestructarr[k] = filenamestructarr[i];
                filenamestructarr[i] = temp;
            }       
        
     }
     printf("List of files by first letter of filename from a to b\n");
     /*for loop prints the arranged list to the terminal */
     for(i = 0; i < j; i++){
        printf("%s\n", filenamestructarr[i].filenamestosort);
     }

    free(filenamearray);

    return;
}

void removefile(char *inputFile){
    char removeName[256], inputName[256], firstline[256];/* fNamecodetable[266]*/
    FILE *removePTR;
    int i, c, pointinlist = 0, pointinremoveNamearray;
    char* removeNamearray;
    size_t removesize;
    int fileExists = 0;

    /*debug*/
    compareSTR(inputFile, "-1");

    if(strcmp(inputFile, "-1") == 0) {
        printf("\nEnter name of file\n>");
        scanf(" %255[^\n]s", inputName);

        while((c = getchar()) != EOF && c != '\n');
    }
    else {
        strcpy(inputName, inputFile);
    }
    
    encrypt(username, username);

    if((removePTR = fopen(username, "r")) == NULL){
        printf("\nFailed to open file");
        return;
    }
        
    /*This peice of code up to the forloop copies the text data from
    the file into an array that holds that text the removesize is found
    by seeking the end of the file and putting the value into a 
    variable. Then memory is alocated of that variables size*/

    fseek(removePTR, 0, SEEK_END);
    removesize = ftell(removePTR);
    rewind(removePTR);

    /*debug*/
    compareINT(removesize, 0);

    if(removesize == 0){
        printf("\nNo files in database\n");
        fclose(removePTR);
        encrypt(username, username);
        return;
    }
        
    removeNamearray = (char*)malloc(removesize);
    if(removeNamearray == NULL) {
        printf("\nMemory allocation failed\n");
        free(removeNamearray);
        fclose(removePTR);
        encrypt(username, username);
        return;
    }
    /*reads data from file to removeNamearray reads the first line so that 
    the username is read. It is then added to the list again seperately*/
    fscanf(removePTR, "%255[^\n]%*c", firstline);
    removesize = fread(removeNamearray, 1, removesize, removePTR);
    fclose(removePTR);

    /*debug*/
    compareINT(removesize, 0);

    if(removesize == 0){
        printf("\nNo files in database\n");
        free(removeNamearray);
        encrypt(username, username);
        return; 
    }
    /*This for loop iterates until it has either gone through the whole
    array containing the file or until it has found the file and fileExists 
    is 1. 
        
    If that is the case it prints confirmation that the file exists.
        
    If it gets to the end of the file and file exists does not 
    equal to one then it prints that the file does not exist.*/

    removePTR = fopen(username, "w");
    if(removePTR == NULL){
        printf("\nFailed to open file\n");
        free(removeNamearray);
        encrypt(username, username);
        return;
    }
        
    fprintf(removePTR, "%s\n", firstline);

    /*debug*/
    loopHeader();

    for(i = 0; pointinlist < removesize; i++){
        pointinremoveNamearray = 0;
        /*while loop extracts the strings from the array containing all
        the users filenames*/
        while(removeNamearray[pointinlist] != '\n'){
            removeName[pointinremoveNamearray] = removeNamearray[pointinlist];
            pointinremoveNamearray++;
            pointinlist++;  
        }
        removeName[pointinremoveNamearray] = '\0';
        pointinlist++;

        /*debug*/
        compareSTR(inputName, removeName);

        /*Compare each removeName to inputName, if they are the same dont print
        back to removePTR*/
        if(strcmp(inputName, removeName) != 0){
            fprintf(removePTR, "%s\n", removeName);
            printf("\removeName %s added\n", removeName);
        }
        else{
            /*confirms that the file name was removed from the removePTR*/
            printf("\nFile %s removed from removePTR\n", inputName);
            fileExists = 1;
        }
    }
    if(fileExists == 0){
        /*if there are no matches print that the file name entered
        wasnt in the users file list */
        printf("\nFile %s not found in removePTR\n", inputName);
    }
    free(removeNamearray);
    fclose(removePTR);      
    encrypt(username, username);
    return;
}
