CXXFLAGS=-g -Wall -rdynamic -march=native -std=c++11
CXXFLAGS+=-O2 -lpthread 
LDFLAGS=-Wl,--no-as-needed

HEADERS=$(wildcard *.h)
TESTS= example

all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS) 

example: example.cpp

clean:
	rm -f $(TESTS)
