CC = icc
CFLAGS = -std=c++11

all: 
	$(CC) -c main.cpp $(CFLAGS) -o arrayWGraph

.PHONY: clean

clean:
	rm -f *.o
	rm -f arrayWGraph
