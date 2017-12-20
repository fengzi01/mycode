objects = s echo_server
CXXFLAGS = -L/usr/local/lib -I/usr/local/include
LDFLAGS = -Wl,-rpath=/usr/local/lib
all: $(objects)
s: simple_example.cpp
	g++ $(CXXFLAGS) $(LDFLAGS) simple_example.cpp -lev -o s
echo_server: echo_server.cpp
	g++ $(CXXFLAGS) $(LDFLAGS) echo_server.cpp -lev -o echo_server -g
clean:
	rm $(objects)
