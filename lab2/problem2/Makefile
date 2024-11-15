CC = gcc
CFLAGS = -pthread -O2
TARGETS = sum_serial sum_multi_thread

all: $(TARGETS)

sum_serial: sum_serial.c
	$(CC) $(CFLAGS) -o $@ $^

sum_multi_thread: sum_multi_thread.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
