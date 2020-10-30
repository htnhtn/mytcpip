
main = main
deps = device.h packetio.h types.h
src = $(main).cpp device.cpp packetio.cpp types.cpp
obj = $(src:%.cpp=bin/%.o)
cflags = -Iinclude -lpcap -lpthread

vpath %.h include
vpath %.cpp src
vpath %.o bin

$(main): $(obj)
	g++ -o bin/$(main) $(obj) $(cflags)
	bin/$(main)

bin/%.o: %.cpp $(deps)
	g++ -c src/$*.cpp -o bin/$*.o $(cflags)

clean:
	rm -rf bin/*

