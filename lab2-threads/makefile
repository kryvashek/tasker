.PHONY: all clean

CMPLR=g++
OPTS=-pthread -std=c++0x

all: productor

clean:
	rm -rf ./*.o ./*.a

productor: productor.o
	$(CMPLR) $(OPTS) productor.o -o productor

productor.o: main.cpp threadarmy.hpp
	$(CMPLR) $(OPTS) -c main.cpp -o productor.o
