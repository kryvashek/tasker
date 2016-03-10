.PHONY: all clean

CMPLR=g++
OPTS=-pthread -std=c++0x

all: productor

clean:
	rm -rf ./*.o ./*.a

productor: productor.o cortege.a
	$(CMPLR) $(OPTS) productor.o cortege.a -o productor

productor.o: main.cpp
	$(CMPLR) $(OPTS) -c main.cpp -o productor.o

cortege.a: cortege.o
	ar cr cortege.a cortege.o
	ranlib cortege.a

cortege.o: cortege.cpp cortege.hpp
	$(CMPLR) $(OPTS) -c cortege.cpp -o cortege.o
