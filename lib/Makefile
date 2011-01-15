
GCC=g++

INCLUDE=-I../include

ARGS=-Wall -O1 $(INCLUDE)

all: libgeckog251.so

geckog251.o: geckog251.cpp
	$(GCC) -c -fpic $(ARGS) $< -o $@

libgeckog251.a: geckog251.o
	$(AR) rs $@ $<

libgeckog251.so: geckog251.o
	$(GCC) -shared $(ARGS) -o $@ $<

clean:
	-rm -f geckog251.o
	-rm -f libgeckog251.so
	-rm -f libgeckog251.a
