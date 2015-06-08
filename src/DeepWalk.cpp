#include "stdafx.h"
#include "DeepWalk.h"

using namespace graphchi;
using namespace std;

typedef unsigned int VertexDataType;
typedef unsigned int EdgeDataType;

struct RandomWalkProgram : public GraphChiProgram<VertexDataType, EdgeDataType> {
	
	// int steps_per_walk()
	// {
	// 	return _steps_per_walk;
	// }
	// int walks_per_source()
	// {
	// 	return _walks_per_source;
	// }	
	void update(graphchi_vertex<VertexDataType, EdgeDataType > &vertex, graphchi_context &gcontext) {
	}
	void before_iteration(int iteration, graphchi_context &gcontext) {
	}
	void after_iteration(int iteration, graphchi_context &gcontext) {
	}
	void before_exec_interval(vid_t window_st, vid_t window_en, graphchi_context &gcontext) {
	}
	void after_exec_interval(vid_t window_st, vid_t window_en, graphchi_context &gcontext) {
	}
};


DeepWalk::DeepWalk(std::string fNameGraph, int walks_per_node_num, int in_walk_length, int in_feat_num, bool graphFile_binary, int _vertices_num, int _iterations_number)
{	
	walks_num = walks_per_node_num;
	walk_length = in_walk_length;
	feat_num = in_feat_num;
	graph_filename = fNameGraph;
	vertices_num = _vertices_num; 
	iterations_number = _iterations_number;
// 	Graph = TNGraph::New();
// 	std::ifstream in;
// 	if (graphFile_binary)
// 		in.open(fNameGraph, ifstream::binary);
// 	else
// 		in.open(fNameGraph);
// 	if (!in){
// 		std::cout << "Unable to open file" << std::endl;
// 		return;
// 	}
// 	long long e1, e2;
// 	std::vector<int> E;
// 	std::set<int> V;
// 	bool good = 1;
// 	while (!in.eof()){
// 		if (graphFile_binary){
// 			in.read((char*)&e1, sizeof(long long));
// 			in.read((char*)&e2, sizeof(long long));
// 		}
// 		else in >> e1 >> e2;
// 		E.push_back(e1); E.push_back(e2);
// 		V.insert(e1); V.insert(e2);
// 	}
// 	in.close();
// 
// 	for (auto it = V.begin(); it != V.end(); it++){
// 		Graph->AddNode(*it);
// 		nodes_ids.push_back(*it);
// 	}
// 	for (int i = 0; i < E.size(); i += 2)
// 		Graph->AddEdge(E[i], E[i + 1]);

	//init vars

}


bool DeepWalk::corpus_save(string outFile_name)
{
	int walks_num = Walks.size();
	ofstream outFile(outFile_name);
	if (!outFile) return 0;
	for (int walk_cur_id = 0; walk_cur_id < walks_num; walk_cur_id++){
		Walk walk_cur = Walks[walk_cur_id];
		int words_perWalk = walk_cur.words.size();
		for (int word_cur_id = 0; word_cur_id < words_perWalk; word_cur_id++){
			outFile << walk_cur.words[word_cur_id]-1<<" ";
		}
		outFile << endl;
	}
	return 1;
}

bool DeepWalk::corpus_build()
{
	int argc = 3;
	const char* argv[] = { "blabla", "--filetype=adjlist", "--execthreads=8" };
	graphchi_init(argc, argv);
	
	
	metrics m("randomwalk");
	
	/* Basic arguments for application */
	//std::string filename = get_option_string("file");  // Base filename
	//int numV = get_option_int("numV");
	//int niters = get_option_int("niters", 100); // Number of iterations
	bool scheduler = true;                       // Whether to use selective scheduling

	/* Detect the number of shards or preprocess an input to create them */
	bool preexisting_shards;
	//get_option_string("filetype", "adjlist");
	int nshards = convert_if_notexists<vid_t>(graph_filename, get_option_string("nshards", "auto"), preexisting_shards);

	//--filetype=adjlist --execthreads=8
	
	

	const int _walks_per_source = 5, 
            _steps_per_walk = 10;
	/* Run */
	RandomWalkProgram program;

	//initialize walks
	
	int numWalks = _walks_per_source*vertices_num;
	
	//go
	graphchi_engine<VertexDataType, EdgeDataType> engine(graph_filename, nshards, scheduler, m, _walks_per_source, _steps_per_walk);
	if (preexisting_shards) {
		engine.reinitialize_edge_data(0);
	}
	engine.run(program, iterations_number);

	for(int i=0; i<engine.completedWalks.size(); i++){
		Walk temp; 
		temp.words = engine.completedWalks[i];
		Walks.push_back(temp);
	}
	//out << walks.size() << " " << walks[0].size() << endl;
	/* Report execution metrics */
	metrics_report(m);
	
	// for (int path_cur = 0; path_cur < walks_num; path_cur++)
	// {
	// 	cout << "Done with " << path_cur * 10 << "% of walks ..." << endl;
	// 	vector<int> nod_shuffle;
	// 	for (int node_cur = 0; node_cur < Graph->GetNodes(); node_cur++)
	// 	{
	// 		Walk  oneWalk = random_walk_gen(walk_length, std::time(0), nodes_ids[node_cur]);
	// 		Walks.push_back(oneWalk);
	// 	}
	// }
	// return 1;
}

Walk DeepWalk::random_walk_gen(int walk_length, int srand_val, int snode_id)
{
	Walk walk;
	// int len_cur = 1;
	// int node_cur_id = snode_id;
	// 
	// //TODO add uniformity condition 
	// walk.words.push_back(snode_id); 
	// while (len_cur<walk_length){
	// 	TNGraph::TNodeI node_cur = Graph->GetNI(node_cur_id);
	// 	int outNodes_num = node_cur.GetOutDeg();
	// 	if (outNodes_num > 0){
	// 		int outNode_rand = rand() % outNodes_num;
	// 		// path.append(rand.choice(G[cur]))
	// 		walk.words.push_back(node_cur.GetOutNId(outNode_rand));
	// 		node_cur_id = node_cur.GetOutNId(outNode_rand);
	// 		len_cur++;
	// 	}
	// 	else
	// 		break;
	// }
	return walk;
}

bool DeepWalk::fv_gen_and_save(string corpusFile_name, string fvFile_name){
	word2vec_main(corpusFile_name, fvFile_name, 1, 1, 0, 0, 128, 12);
	return 1;
}