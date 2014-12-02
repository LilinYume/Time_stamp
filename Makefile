# デタラメです。メイクファイルの書き方わかりません。
CC		=	gcc
CCFLAGS =	-c -Wall
AR		=	ar
MAIN	=	main

all:
	$(CC) *.c $(CCFLAGS)
	rm $(MAIN).o
	$(AR) -r libtimestamp *.o
	$(CC) $(MAIN).c $(CCFLAGS) -I./-timestamp
	@(echo --------------------------)
	@(./a.out)
clean:
	rm -f *.o libtimestamp
