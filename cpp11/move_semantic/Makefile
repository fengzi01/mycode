CXXFLAGS=-g -Wall -rdynamic -march=native -std=c++11
CXXFLAGS+=-O2

HEADERS=$(wildcard *.h)
TESTS= VectorTest 

all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS) -lpthread

VectorTest: VectorTest.cpp

clean:
	rm -f $(TESTS)
