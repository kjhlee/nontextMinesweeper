CXX = g++
CXXFLAGS = -std=c++11 -Wall -I/opt/homebrew/opt/sfml/include
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: Main.o
	$(CXX) -o main Main.o $(LDFLAGS)

Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) -c Main.cpp

clean:
	rm -f *.o main
