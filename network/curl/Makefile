objects = evhiperfifo
CXXFLAGS = -L/usr/local/lib -I/usr/local/include
#LDFLAGS = -Wl,-rpath=/usr/local/lib
C = gcc
all: $(objects)
evhiperfifo: evhiperfifo.c
	$(C) $(CXXFLAGS) $(LDFLAGS) evhiperfifo.c -lev -lcurl -o evhiperfifo -g
clean:
	rm $(objects)
