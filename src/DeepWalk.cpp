//#define DYNAMICEDATA 1

#include "stdafx.h"
#include "DeepWalk.h"
#include "graphchi_basic_includes.hpp"
#include "api/dynamicdata/chivector.hpp"

using namespace graphchi;
using namespace std;



typedef unsigned int VertexDataType;
typedef unsigned int EdgeDataType;
//typedef chivector<vid_t>  EdgeDataType;

struct RandomWalkProgram : public GraphChiProgram<VertexDataType, EdgeDataType> {
	
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
	_walks_per_source = walks_per_node_num;
	_steps_per_walk = in_walk_length;
	feat_num = in_feat_num;
	graph_filename = fNameGraph;
	vertices_num = _vertices_num; 
	iterations_number = _iterations_number;
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
			outFile << walk_cur.words[word_cur_id]<<" ";
		}
		outFile << endl;
	}
	return 1;
}

bool DeepWalk::corpus_build()
{			
	int argc = 3;
	const char* argv[] = { "blabla", "--filetype=adjlist", "--execthreads=8" };
	//const char* argv[] = { "blabla", "--filetype=edgelist", "--execthreads=8" };
	graphchi_init(argc, argv);
	
	metrics m("randomwalk");
	
	bool scheduler = true;                       // Whether to use selective scheduling

	/* Detect the number of shards or preprocess an input to create them */
	bool preexisting_shards;
	//get_option_string("filetype", "adjlist");
	int nshards = convert_if_notexists<vid_t>(graph_filename, get_option_string("nshards", "auto"), preexisting_shards);
	//int nshards = convert_if_notexists<vid_t>(graph_filename, get_option_string("nshards", "2"), preexisting_shards);
	
	
	/* Run */
	RandomWalkProgram program;

	//initialize walks
	
	//int numWalks = _walks_per_source*vertices_num;
	
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
	metrics_report(m);
}

Walk DeepWalk::random_walk_gen(int walk_length, int srand_val, int snode_id)
{
	Walk walk;
	return walk;
}

bool DeepWalk::fv_gen_and_save(string corpusFile_name, string fvFile_name){
	word2vec_main(corpusFile_name, fvFile_name, 1, 1, 0, 0, 128, 12);
	return 1;
}