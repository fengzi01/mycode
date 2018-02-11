objects = evhiperfifo ev_curl_client
CXXFLAGS = -L/usr/local/lib -I/usr/local/include
#LDFLAGS = -Wl,-rpath=/usr/local/lib
C = gcc
all: $(objects)
evhiperfifo: evhiperfifo.c
	$(C) $(CXXFLAGS) $(LDFLAGS) evhiperfifo.c -lev -lcurl -o evhiperfifo -g
ev_curl_client: ev_curl_client.c
	$(C) $(CXXFLAGS) $(LDFLAGS) ev_curl_client.c -lev -lcurl -o ev_curl_client -g
clean:
	rm $(objects)
