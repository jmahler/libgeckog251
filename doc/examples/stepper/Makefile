
GCC=g++

# these work when compiling from the source directory
INCLUDE=../../../include
LIB=../../../lib

ARGS=-Wall -O1 -I$(INCLUDE) -lgeckog251 -L$(LIB)

# Once this program is compilee your LD_LIBARARY_PATH must be
# setup so when the program is run the shared library can be found.
#
# export LD_LIBRARY_PATH=$(HOME)/lib

all: stepper

stepper: stepper.cpp
	$(GCC) $(ARGS) $< -o $@

clean:
	-rm -f stepper
