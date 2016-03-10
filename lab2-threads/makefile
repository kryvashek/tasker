.PHONY: all clean

CMPLR=g++
OPTS=-pthread -std=c++0x

all: productor

clean:
	rm -rf ./*.o ./*.a

productor: productor.o kit.a
	$(CMPLR) $(OPTS) productor.o kit.a -o productor

productor.o: main.cpp
	$(CMPLR) $(OPTS) -c main.cpp -o productor.o

kit.a: kit.o
	ar cr kit.a kit.o
	ranlib kit.a

kit.o: kit.cpp kit.hpp
	$(CMPLR) $(OPTS) -c kit.cpp -o kit.o
