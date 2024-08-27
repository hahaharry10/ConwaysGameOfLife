CC = clang
C89 = -std=c89
CFLAGS = -pedantic -g
HEADERS = *.h
MAINS = 	gameOfLife.o
TARGETS	=	gameOfLife
.PHONY: all clean

all: ${TARGETS}

clean:
	rm ${TARGETS} ${MAINS} ${MODULES}

gameOfLife: gameOfLife.o
	$(CC) $(C89) $(CFLAGS) -o $@ $^

gameOfLife.o: gameOfLife.c ${HEADERS}
	$(CC) -c $(C89) $(CFLAGS) -o $@ $<

