CC := g++
CFLAGS := -Wall -O3
objects = aStar RBFS


all: $(objects)

aStar:
		$(CC) $(CFLAGS) -DHEUR -o aStar ./src/aStar.cpp
RBFS:
		$(CC) $(CFLAGS) -DHEUR -o rbfs ./src/RBFS.cpp

britain:
		$(CC) $(CFLAGS) -DBRITAIN -o aStarb ./src/aStar.cpp
		$(CC) $(CFLAGS) -DBRITAIN -o rbfsb ./src/RBFS.cpp


clean:
		rm -rf *.o $(objects)
