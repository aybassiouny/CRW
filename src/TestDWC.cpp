/////////////////////////////////////////////////////////////////////
// DeepWalk Cuda implementation.
// Rough pseudocode; 
//		read input network [done]
//		generate graph: nodes, edges, num nodes, num edges, [done]
//		initialize vars: num_walks, feat_num, walk_length [done]
//		generate all walks and truncate them
//			For each node generate n=num_walks of random_walk
//				using random_walk(self, path_length, alpha=0, rand=random.Random(), start=None):
//		word2vec returns model
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeepWalk.h"


using namespace std;

void doError(string msg){
	cout << msg << endl;
	exit(1);
}

int main(int argc, char* argv[]){
	bool graphFile_binary = 1;
	int walks_per_node_num = 10, walk_length = 80, feat_num = 64, 
		v_num=10, it_num =100; 
	string graphFile_name= "blogcatalog.txt",
		walksFile_name = "walks.txt",
		fvFile_name = "fv.txt"; 
	
	if (argc < 2) cout << "No input file specified. Using default 'sampleGraph.txt'."<<endl;
	else graphFile_name = argv[1];

	DeepWalk dw(graphFile_name, walks_per_node_num, walk_length, feat_num, graphFile_binary, v_num, it_num );

	cout << "Building corpus ..." << endl;
	if (dw.corpus_build()) cout << "Built corpus." << endl;
	else doError("Error building corpus.");

	cout << "Saving corpus ..." << endl;
	if (dw.corpus_save(walksFile_name)) cout << "Corpus saved." << endl;
	else doError("Error saving corpus.");
	if (dw.fv_gen_and_save(walksFile_name, fvFile_name))
	{
		cout << endl<<"Model Generated. Saving ..." << endl;
	}

	return 0;
}