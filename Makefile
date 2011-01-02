VERSION   = 0.1

COMPILER  = $(patsubst %.cc,%.o,$(wildcard compiler/*.cc))
VM        = $(patsubst %.cc,%.o,$(wildcard vm/*.cc))

compiler: CXXFLAGS = -DVERSION=$(VERSION) -O3 -c
vm:       CXXFLAGS = -emit-llvm -O3 -c

all: compiler vm

compiler: $(COMPILER)
	clang++ $(COMPILER) -o proty

vm: $(VM)
	llvm-link $(VM) -o lib/vm.bc

%.o: %.cc
	clang++ $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(COMPILER) proty
	rm -f $(VM) lib/vm.bc

.PHONY: all compiler vm clean