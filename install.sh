#make clean
rm *.o ./TestDWC
nvcc -c -g -G src/updateFunc.cu -gencode arch=compute_20,code=compute_20
make
nvcc -O0 -g -G -gencode arch=compute_20,code=compute_20 -lcudart -lz -Xcompiler -fopenmp -o TestDWC TestDWC.o DeepWalk.o updateFunc.o word2vec.o
#./TestDWC myspace.txt adjlist