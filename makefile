#makefile CFLAGS . -Wall -Werror -ansi -lm

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm

assign3.out: login.out mainMenu.out compress.out decompress.out database.out
	$(CC) $(CFLAGS) -o assign3.out login.out mainMenu.out compress.out decompress.out database.out

login.out: loginPage.c loginHeader.h debugHeader.h
	$(CC) $(CFLAGS) -c -o login.out loginPage.c

mainMenu.out: mainMenuPage.c mainMenuHeader.h
	$(CC) $(CFLAGS) -c -o mainMenu.out mainMenuPage.c

compress.out: compress.c compress.h
	$(CC) $(CFLAGS) -c -o compress.out compress.c

decompress.out: decompress.c decompress.h
	$(CC) $(CFLAGS) -c -o decompress.out decompress.c

database.out: databasePage.c databaseHeader.h
	$(CC) $(CFLAGS) -c -o database.out databasePage.c

clean:
	rm login.out mainMenu.out compress.out decompress.out database.out assign3.out
