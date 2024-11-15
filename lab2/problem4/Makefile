CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = problem4
SRC = problem4.c

.PHONY: all clean run

# Build chương trình
all: $(TARGET)

# Biên dịch file nguồn
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run
run: $(TARGET)
	./$(TARGET)

# Dọn dẹp các file
clean:
	rm -f $(TARGET)
	rm -f shared_file.txt
