CC=g++
CFLAGS=-Wall -O3 -std=c++14
BINS=tarea
all: clean tarea

tarea:
	$(CC) $(CFLAGS) -o tarea main.cpp

clean:
	@echo " [CLN] Removing binary files"
	rm -f $(BINS)
