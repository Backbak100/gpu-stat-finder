CC = gcc

CFLAGS = -fblocks -I./include
LDFLAGS = -framework CoreFoundation -framework IOKit -lIOReport

TARGET = a.out

SRC = src/main.c src/livestats.c src/staticstats.c src/sig_handler.c src/CoreFoundation.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run:
	./$(TARGET)
