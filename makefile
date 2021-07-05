CXX=g++
CXX_FLAGS= -std=c++17 -O2 -I/usr/local/opt/libressl/include
LD_FLAGS= -L/usr/local/opt/libressl/lib -lcrypto

.PHONY: cleanall

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^

TestIndex: Buffer.o Geometry.o Hash.o Node.o Query.o Record.o Utils.o \
	TestIndex.o
	$(CXX) $(LD_FLAGS) $^ -o TestIndex

TestQuery: Buffer.o Geometry.o Hash.o Node.o Query.o Record.o Utils.o \
	TestQuery.o
	$(CXX) $(LD_FLAGS) $^ -o TestQuery

TestSize: Buffer.o Geometry.o Hash.o Record.o Utils.o TestSize.o
	$(CXX) $(LD_FLAGS) $^ -o TestSize

QueryGenerator: Buffer.o Geometry.o Hash.o Record.o Utils.o QueryGenerator.o
	$(CXX) $(LD_FLAGS) $^ -o QueryGenerator

all: TestIndex TestQuery TestSize QueryGenerator

cleanall:
	-rm -f *.o TestIndex TestQuery TestSize QueryGenerator
