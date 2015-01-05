CXX=clang++
CXXFLAGS=-std=c++1y -Wall -O2
LIBS=-lncurses
TARGET=cookie

all: cookie.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) cookie.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
