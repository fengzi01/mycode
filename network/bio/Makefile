objects = c s
all: $(objects)
c: tcp_client.cpp
	g++ $(CFLAGS) tcp_client.cpp -o c
s: echo_server.cpp
	g++ $(CFLAGS) echo_server.cpp -o s
clean:
	rm s c
