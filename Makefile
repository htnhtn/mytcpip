
main = main
deps = device.h packetio.h types.h
src = $(main).cpp device.cpp packetio.cpp types.cpp
obj = $(src:%.cpp=%.o)
cflags = -Iinclude

$(main): $(obj)
	g++ -o $(main) $(obj)

%.o: %.c $(deps)
	g++ -c $< #-o $@

clean:
	rm -rf %(obj) $(main)
