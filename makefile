#
#	File:	makefile
#	Author:	Matteo Loporchio
#

CXX=g++
CXX_FLAGS= -std=c++17 -O2 -I/usr/local/opt/libressl/include
LD_FLAGS= -L/usr/local/opt/libressl/lib -lcrypto

.PHONY: all clean

OBJECTS=Buffer.o Geometry.o Hash.o Node.o Query.o Record.o
TARGETS=TestIndex TestQuery QueryGen

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^

TestIndex: $(OBJECTS) TestIndex.o
	$(CXX) $(LD_FLAGS) $^ -o TestIndex

TestQuery: $(OBJECTS) TestQuery.o
	$(CXX) $(LD_FLAGS) $^ -o TestQuery

QueryGen: $(OBJECTS) QueryGen.o
	$(CXX) $(LD_FLAGS) $^ -o QueryGen

all: $(TARGETS)

clean:
	-rm -f *.o $(TARGETS)
