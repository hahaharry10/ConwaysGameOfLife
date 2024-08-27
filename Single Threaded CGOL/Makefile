CC = clang
CFLAGS = -std=c89 -pedantic -g
INCLUDES = -I/usr/local/include/SDL2/
LIBS = -lSDL2
SRCS = gameOfLife.c GUI.c
MAIN = gameOfLife
OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
