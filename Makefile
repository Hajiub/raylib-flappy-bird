CC = gcc
CFLAGS = -Wall  -std=c99 -Werror
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRCS = main.c
OBJS = $(SRCS:.c=.o)
EXEC = flappy 

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

run:
	./$(EXEC)
