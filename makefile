# We cannot use the -std=c90 flag here, because the book's code uses the
# strdup function, so it seems like its written in another C-dialect...
CFLAGS = -g -Wall -Wextra
CC = g++

all: tc

tc: driver.o errormsg.o util.o tiger.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o tc driver.o lex.yy.o errormsg.o util.o tiger.tab.o

driver.o: driver.c errormsg.h util.h
	$(CC) $(CFLAGS) -c driver.c

errormsg.o: errormsg.c errormsg.h util.h
	$(CC) $(CFLAGS) -c errormsg.c

lex.yy.o: lex.yy.c errormsg.h util.h
	$(CC) $(CFLAGS) -c lex.yy.c

tiger.tab.o: tiger.tab.c
	$(CC) $(CFLAGS) -c tiger.tab.c

lex.yy.c: tiger.l
	lex tiger.l

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

tiger.tab.c: 
	bison -d -t -v tiger.y

clean: 
	rm -f tc *.o lex.yy.c *.tab.c *.tab.h *.output 
