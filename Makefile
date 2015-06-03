INCFLAGS = -I/usr/local/include/ -I./src/

CPP = g++
CPPFLAGS = -g -O3 -w -fdiagnostics-color $(INCFLAGS)  -fopenmp -Wno-strict-aliasing  -m64 -fdump-class-hierarchy-all
#CPPFLAGS = -g -O3 -w -fdiagnostics-color $(INCFLAGS)  -fopenmp -Wno-strict-aliasing  -m64
LINKERFLAGS = -lz
DEBUGFLAGS = -g -ggdb $(INCFLAGS)
HEADERS=$(shell find src/ -name '*.hpp')


all: apps 
apps: example_apps/mRW
#apps: example_apps/RW
als: example_apps/matrix_factorization/als_edgefactors  example_apps/matrix_factorization/als_vertices_inmem
tests: tests/basic_smoketest tests/bulksync_functional_test tests/dynamicdata_smoketest tests/test_dynamicedata_loader

echo:
	echo $(HEADERS)
clean:
	@rm -rf bin/example_apps/RW
	@rm -rf *.html graphchi_metrics.txt log.txt walks.txt out.txt
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

blocksplitter: src/preprocessing/blocksplitter.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) src/preprocessing/blocksplitter.cpp -o bin/blocksplitter $(LINKERFLAGS)

sharder_basic: src/preprocessing/sharder_basic.cpp $(HEADERS)
	@mkdir -p bin
	$(CPP) $(CPPFLAGS) src/preprocessing/sharder_basic.cpp -o bin/sharder_basic $(LINKERFLAGS)

example_apps/% : example_apps/%.cpp $(HEADERS)
	@mkdir -p bin/$(@D)
	$(CPP) $(CPPFLAGS) $(DEBUGFLAGS) -Iexample_apps/ $@.cpp -o bin/$@ $(LINKERFLAGS) 



myapps/% : myapps/%.cpp $(HEADERS)
	@mkdir -p bin/$(@D)
	$(CPP) $(CPPFLAGS) -Imyapps/ $@.cpp -o bin/$@ $(LINKERFLAGS)

tests/%: src/tests/%.cpp $(HEADERS)
	@mkdir -p bin/$(@D)
	$(CPP) $(CPPFLAGS) src/$@.cpp -o bin/$@	$(LINKERFLAGS)


graphlab_als: example_apps/matrix_factorization/graphlab_gas/als_graphlab.cpp
	$(CPP) $(CPPFLAGS) example_apps/matrix_factorization/graphlab_gas/als_graphlab.cpp -o bin/graphlab_als $(LINKERFLAGS)

cf:
	cd toolkits/collaborative_filtering/; bash ./test_eigen.sh; 
	if [ $$? -ne 0 ]; then exit 1; fi
	cd toolkits/collaborative_filtering/; make 
cf_test:
	cd toolkits/collaborative_filtering/; make test; 
cfd:
	cd toolkits/collaborative_filtering/; make -f Makefile.debug

parsers:
	cd toolkits/parsers/; make
parsersd:
	cd toolkits/parsers/; make -f Makefile.debug
ga:
	cd toolkits/graph_analytics/; make
ta:
	cd toolkits/text_analysis/; make

docs: */**
	doxygen conf/doxygen/doxygen.config


	

	