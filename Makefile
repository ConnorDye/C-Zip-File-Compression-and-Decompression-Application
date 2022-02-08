CC = gcc
CFLAGS = -Wall -g -ansi -pedantic
LD = gcc
LDFLAGS =

all: hencode hdecode

hdecode: hdecode.o tree.o list.o bit.o
	$(LD) $(LDFLAGS) -o hdecode hdecode.o tree.o list.o bit.o

hencode: hencode.o tree.o list.o bit.o
	$(LD) $(LDFLAGS) -o hencode hencode.o tree.o list.o bit.o

hdecode.o: hdecode.c
	$(CC) $(CFLAGS) -c -o hdecode.o hdecode.c

hencode.o: hencode.c
	$(CC) $(CFLAGS) -c -o hencode.o hencode.c

tree.o: tree.c
	$(CC) $(CFLAGS) -c -o tree.o tree.c

list.o:
	$(CC) $(CFLAGS) -c -o list.o list.c

bit.o:
	$(CC) $(CFLAGS) -c -o bit.o bit.c

test:
	./hencode INPUT out

test2:
	./hdecode INPUT

git:
	git add list.c hdecode.c hencode.c tree.c list.h bit.c bit.h README Makefile
	git commit -m "$m"
	git push -u origin master

clean:
	rm *.o hencode
	rm *.o hdecode
handin:
	scp hdecode.c list.c hencode.c tree.c bit.c list.h bit.h README Makefile chdye@unix2.csc.calpoly.edu:./asgn3
handinhde:
	scp hdecode.c chdye@unix1.csc.calpoly.edu:./asgn3
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./hencode /home/pn-cs357/lib/lab03//Tests/inputs/05_binary
