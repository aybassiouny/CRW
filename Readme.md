Random Walk Cuda
-----------------
1. In order to generate binaries run "./install.sh" from root directory. 

2. Current supported arguments are: TestDWC.exe <graph_file_name> <graph_file_type>
<graph_file_name>: name of the graph file in ASCII
<graph_file_type>: either adjlist or edgelist

3. More arguments to follow of course. For this version, there are a couple more arguments if you would like to change, you will have to edit TestDWC.cpp, but it's pretty straightforward: 


	Variable_name (Default value): explanation
	walksFile_name("walks.txt")	: Name of the exported walks file 
	fvFile_name("fv.txt")		: Name of the end feature vector file
	walks_per_node_num (2)		: Number of walks per vertix
	walk_length (10)			: The length of a single walk
	feat_num (64)				: Length of feature vector representing each vertix


(either adjlist or edgelist)

4. For faster runs on machines with more memory, you can increase cashe size in conf/graphchi.local.conf by changing the value of membudget_mb (800 is good enough for a 4 gb machine).

5. Run "make clean" in the root directory in order to remove all temporary files (essential in case you change membudget_mb).