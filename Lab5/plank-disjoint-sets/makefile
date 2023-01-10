ALL = bin/dj_test

GPP ?= g++
INCLUDE = -Iinclude
CFLAGS ?= -Wall -Wextra

all: $(ALL)

clean:
	rm -f bin/* obj/*


obj/disjoint_set.o: src/disjoint_set.cpp include/disjoint_set.hpp
	$(GPP) $(INCLUDE) $(CFLAGS) -c -o obj/disjoint_set.o src/disjoint_set.cpp

obj/dj_test.o: src/dj_test.cpp include/disjoint_set.hpp
	$(GPP) $(INCLUDE) $(CFLAGS) -c -o obj/dj_test.o src/dj_test.cpp

bin/dj_test: obj/dj_test.o obj/disjoint_set.o
	$(GPP) $(INCLUDE) $(CFLAGS) -o bin/dj_test obj/dj_test.o obj/disjoint_set.o


