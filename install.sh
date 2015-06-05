make clean
nvcc -c example_apps/updateFunc.cu -gencode arch=compute_20,code=compute_20
make

nvcc -gencode arch=compute_20,code=compute_20 -lcudart -lz -Xcompiler -fopenmp -o RWC RW.o updateFunc.o 
#./RWC --file=simpAdjList.txt --filetype=adjlist --execthreads=8 --numV=10 
./RWC --file=blogcatalog.txt --filetype=adjlist --execthreads=8 --numV=10312 
#bin/example_apps/RW --file=simpAdjList.txt --filetype=adjlist --execthreads=8 --numV=10 
#bin/example_apps/mRW > log.txt
#bin/example_apps/RW --file=blogcatalog.txt --filetype=adjlist --execthreads=8 --numV=10312 > log.txt

