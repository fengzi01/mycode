objects = a.out
CXXFLAGS = -I /usr/local/protobuf/include/ -L /usr/local/protobuf/lib -Wl,--rpath=/usr/local/protobuf/lib -lpthread -lprotobuf
all: $(objects)
a.out: main.cpp test.pb.cc
	g++ $(CXXFLAGS) main.cpp test.pb.cc -o a.out
clean:
	rm ${objects}
