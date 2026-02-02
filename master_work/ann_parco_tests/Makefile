TARGET_ISA=x86

GEM5_HOME=$(realpath ../../)
$(info   GEM5_HOME is $(GEM5_HOME))

CXX=gcc

CFLAGS=-static -I$(GEM5_HOME)/include

LDFLAGS=-L$(GEM5_HOME)/util/m5/build/$(TARGET_ISA)/out -lm5

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=)

all: $(OBJECTS)

# test_access:
# 	$(CXX) -o $(OBJECTS) test_access.c $(CFLAGS) $(LDFLAGS)
%: %.c
	$(CXX) $(CFLAGS) $< -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJECTS)

.PHONY: all clean