CC = mpicc
CFLAGS = -pedantic -g -std=c89
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
