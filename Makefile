VM_OBJECTS := $(patsubst %.cc,%.o,$(wildcard vm/*.cc))

INCLUDES = -Iinclude/
CXX = clang++
CXXFLAGS = -emit-llvm $(INCLUDES) -O3 -c -o

all: vm

vm: $(VM_OBJECTS)
	llvm-link $(VM_OBJECTS) -o lib/vm.bc

%.o: %.cc
	$(CXX) $(CXXFLAGS) $@ $<

clean:
	rm -f $(VM_OBJECTS)

install:
	echo "not implemented"

uninstall:
	echo "not implemented"

test:
	@bash test/test

.PHONY: all vm clean install uninstall test
