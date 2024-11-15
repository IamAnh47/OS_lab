# Biến cho trình biên dịch và cờ
CC = gcc
CFLAGS = -pthread -lrt -O2
TARGET = problem3

# Biên dịch mục tiêu chính
all: $(TARGET)

# Quy tắc biên dịch
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $^

# Dọn dẹp
clean:
	rm -f $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)
