CC = clang
CC_MPI = mpicc
CFLAGS = -pedantic -g -std=c89
HEADERS = *.h
MAINS = 	gameOfLife.o gameOfLifeMPI.o
TARGETS	=	gameOfLife gameOfLifeMPI
.PHONY: all clean

all: ${TARGETS}

clean:
	rm ${TARGETS} ${MAINS} ${MODULES} *.o

gameOfLifeMPI: gameOfLife.o
	$(CC_MPI) $(CFLAGS) -o $@ $^

gameOfLife: gameOfLife.o
	$(CC) $(CFLAGS) -o $@ $^

gameOfLife.o: gameOfLife.c ${HEADERS}
	${CC_MPI} -c $(CFLAGS) -o $@ $<

gameOfLifeMPI.o: gameOfLifeMPI.c ${HEADERS}
	${CC_MPI} -c $(CFLAGS) -o $@ $<

