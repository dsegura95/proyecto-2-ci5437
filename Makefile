GXX = g++
OPT = -O3 -Wall -std=c++11 -g

all:	src/main.cpp src/othello.hpp src/algorithms.hpp
	make clean
	cd src; $(GXX) $(OPT) -o ../bin/othello.out main.cpp

game:	src/game.cpp src/othello.hpp src/algorithms.hpp
	make clean
	cd src; $(GXX) $(OPT) -o ../bin/game.out game.cpp

clean:
	rm -f bin/*.out

