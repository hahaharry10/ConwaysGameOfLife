CC = mpicc
CFLAGS = -pedantic -g -std=c99
HEADERS = *.h
MAINS = 	gameOfLifeMPI.o
TARGETS	=	gameOfLifeMPI
.PHONY: all clean

all: ${TARGETS}

clean:
	rm $(TARGETS) $(MAINS) *.o

gameOfLifeMPI: gameOfLifeMPI.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c ${HEADERS}
	$(CC) -c $(CFLAGS) -o $@ $<
