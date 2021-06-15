CXX=g++
CXX_FLAGS= -std=c++17 -O2 -I/usr/local/opt/libressl/include
LD_FLAGS= -L/usr/local/opt/libressl/lib -lcrypto

.PHONY: cleanall

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^

main: Buffer.o Geometry.o Hash.o Node.o Query.o Utils.o main.o
	$(CXX) $(LD_FLAGS) $^ -o main

all: main

cleanall:
	-rm -f *.o main
