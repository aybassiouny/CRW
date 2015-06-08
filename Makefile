INCFLAGS = -I/usr/local/include/ -I./graphchi/ -I./src


#CPP = g++
CPP = nvcc -gencode arch=compute_20,code=compute_20

#CPPFLAGS = -g -O3 -w -fdiagnostics-color $(INCFLAGS)  -fopenmp -Wno-strict-aliasing  -m64 -fdump-class-hierarchy-all
#CPPFLAGS = -w -fdiagnostics-color $(INCFLAGS)  -fopenmp -Wno-strict-aliasing  -m64  -std=c++11
CPPFLAGS = -w -g -G -c $(INCFLAGS)  -Xcompiler -fdiagnostics-color -Xcompiler -fopenmp -Xcompiler -Wno-strict-aliasing  -m64 -Xcompiler -std=c++11

LINKERFLAGS = -lz
DEBUGFLAGS = -g $(INCFLAGS)
HEADERS=$(shell find graphchi/ -name '*.hpp')


all: apps 
#apps: example_apps/mRW
#apps: src/RW
apps: src/TestDWC

echo:
	echo $(HEADERS)
clean:
	@rm -rf bin/example_apps/RW
	@rm -rf *.html graphchi_metrics.txt log.txt walks.txt out.txt *.o RWC TestDWC
	@rm -rf  *.numvertices 
	@rm -rf *.adjidx 
	@rm -rf *.adj 
	@rm -rf *.size
	@rm -rf *.bin
	@rm -rf *.deltalog
	@rm -rf *.vout
	@rm -rf  *.intervals
	@rm -rf blogcatalog.txt.dynamic..Z.e4B.1_2_blockdir_1048576 simpAdjList.txt.dynamic..Z.e4B.1_2_blockdir_1048576
	@rm -rf blogcatalog.txt.dynamic..Z.e4B.0_2_blockdir_1048576 simpAdjList.txt.dynamic..Z.e4B.0_2_blockdir_1048576
	@rm -rf blogcatalog.txt.edata..Z.e4B.0_1_blockdir_1048576

#src/% : src/%.cpp $(HEADERS)
src/% : src/%.cpp $(HEADERS)
	@mkdir -p bin/$(@D)
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -Isrc/  $@.cpp src/DeepWalk.cpp src/word2vec.cpp $(LINKERFLAGS) 
	#$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -Isrc/  $@.cpp -o bin/$@ $(LINKERFLAGS) 
	