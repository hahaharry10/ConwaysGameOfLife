CC = clang
CFLAGS = -pedantic -g -std=c89
HEADERS = *.h
MAINS = 	gameOfLife.o
TARGETS	=	gameOfLife
.PHONY: all clean

all: ${TARGETS}

clean:
	rm ${TARGETS} ${MAINS} ${MODULES} *.o

gameOfLife: gameOfLife.o
	$(CC) $(CFLAGS) $(C89FLAGS) -o $@ $^

%.o: %.c ${HEADERS}
	${CC} -c $(CFLAGS) ${C89FLAGS} -o $@ $<
