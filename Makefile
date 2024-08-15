CC = clang
CC_MPI = mpicc
C89 = -std=c89
C99 = -std=c99
CFLAGS = -pedantic -g
HEADERS = *.h
MAINS = 	gameOfLife.o gameOfLifeMPI.o
TARGETS	=	gameOfLife gameOfLifeMPI
.PHONY: all clean

all: ${TARGETS}

clean:
	rm ${TARGETS} ${MAINS} ${MODULES}

gameOfLifeMPI: gameOfLifeMPI.o
	$(CC_MPI) $(C99) $(CFLAGS) -o $@ $^

gameOfLife: gameOfLife.o
	$(CC) $(C89) $(CFLAGS) -o $@ $^

gameOfLife.o: gameOfLife.c ${HEADERS}
	${CC_MPI} -c $(C89) $(CFLAGS) -o $@ $<

gameOfLifeMPI.o: gameOfLifeMPI.c ${HEADERS}
	${CC_MPI} -c $(C99) $(CFLAGS) -o $@ $<

