MAKEFLAGS += -s

ifndef VERBOSE
.SILENT:
endif

all:
	@ mkdir build; cd build; cmake ..
	@ cmake --build build -- -j 4

clean:
	@ rm -rf build
	@ rm -rf cmake-build-debug
	@ rm -rf Havoc