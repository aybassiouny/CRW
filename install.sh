make clean
make
bin/example_apps/RW --file=simpAdjList.txt --filetype=adjlist --execthreads=8 --numV=10 > log.txt
#bin/example_apps/RW --file=blogcatalog.txt --filetype=adjlist --execthreads=8 --numV=10312 > log.txt