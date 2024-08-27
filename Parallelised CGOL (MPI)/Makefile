CC = clang
CC_MPI = mpicc
C99 = -std=c99
CFLAGS = -pedantic -g
HEADERS = *.h
MAINS = 	gameOfLifeMPI.c
TARGETS	=	gameOfLifeMPI
.PHONY: all clean

all: ${TARGETS}

clean:
	rm ${TARGETS} ${MAINS} ${MODULES} *.o

gameOfLifeMPI: gameOfLifeMPI.o
	$(CC_MPI) $(C99) $(CFLAGS) -o $@ $^

gameOfLifeMPI.o: gameOfLifeMPI.c ${HEADERS}
	${CC_MPI} -c $(C99) $(CFLAGS) -o $@ $<

