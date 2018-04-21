all: rockworld 

rockworld: main.cpp Level.cpp separate.cpp
	g++ -std=c++11 -w -g -o rockworld main.cpp Level.cpp separate.cpp -lcurses

clean:
	rm -rf rockworld
