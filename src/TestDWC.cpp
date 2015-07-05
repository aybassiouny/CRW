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
using namespace graphchi;

void doError(string msg){
	cout << msg << endl;
	exit(1);
}

int main(int argc, const char* argv[]){
	bool graphFile_binary = 1;
	int walks_per_node_num = 2, walk_length = 10, feat_num = 64, 
		it_num =100; 
		
	string graphFile_name= "simpAdjList.txt",
		walksFile_name = "walks.txt",
		fvFile_name = "fv.txt", 
		fileType = "adjlist"; 
	
	if (argc < 2) {
		//cout << "No input file specified. Please use 'TestDWC.exe <graph_file_name> <graph_file_type>' "<<end;
		return 0;
	}
	graphFile_name = argv[1];
	if (argc < 3) cout << "No file type specified. Using default: "<<fileType<<endl;
	else fileType = argv[2];

	DeepWalk dw(graphFile_name, walks_per_node_num, walk_length, feat_num, graphFile_binary, it_num );

	cout << "Building & saving corpus ..." << endl;
	if (dw.corpus_build(walksFile_name, fileType)) cout << "Built corpus." << endl;
	else doError("Error building corpus.");

	cout<<endl<<"Would you like to generate Feature Vector as well?"<<endl;
	char ch; cin>>ch;
	if(ch=='y' || ch=='Y'){
		if (dw.fv_gen_and_save(walksFile_name, fvFile_name))
			cout << endl<<"Model Generated & Saved." << endl;
	}

	return 0;
}