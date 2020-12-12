CC = icc
CFLAGS = -std=c++11
LIB = -I include
all: 
	$(CC) main.cpp $(CFLAGS) $(LIB) -o arrayWGraph

.PHONY: clean

clean:
	rm -f *.o
	rm -f arrayWGraph
