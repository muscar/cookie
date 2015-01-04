CXX=clang++
CXXFLAGS=-std=c++1y -Wall -O2
TARGET=cookie

all: cookie.cpp
	$(CXX) $(CXXFLAGS) cookie.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
