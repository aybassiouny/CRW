#make clean
rm *.o ./TestDWC
nvcc -c -g -G src/updateFunc.cu -gencode arch=compute_20,code=compute_20
make
nvcc -O0 -g -G -gencode arch=compute_20,code=compute_20 -lcudart -lz -Xcompiler -fopenmp -o TestDWC TestDWC.o DeepWalk.o updateFunc.o word2vec.o
./TestDWC minitwitter.txt
#./TestDWC simpAdjList.txt

#nvcc -gencode arch=compute_20,code=compute_20 -lcudart -lz -Xcompiler -fopenmp -o RWC RW.o updateFunc.o 
#./RWC --file=simpAdjList.txt --filetype=adjlist --execthreads=8 --numV=10 
#./TestDWC --file=blogcatalog.txt --filetype=adjlist --execthreads=8 --numV=10312 
#bin/src/RW --file=simpAdjList.txt --filetype=adjlist --execthreads=8 --numV=10 
#bin/src/mRW > log.txt
#bin/src/RW --file=blogcatalog.txt --filetype=adjlist --execthreads=8 --numV=10312 > log.txt