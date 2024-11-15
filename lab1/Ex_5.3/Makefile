CC = gcc
CFLAGS = -Wall -std=c99
SRC = calc.c operations.c
OBJ = $(SRC:.c=.o)
TARGET = calc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
