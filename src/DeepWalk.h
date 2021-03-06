#ifndef _DeepWalk
#define _DeepWalk

#include "stdafx.h"

#include "util/toplist.hpp"
#include "Walk.h"
#include "word2vec.h"

class DeepWalk{
public:
	int _walks_per_source, _steps_per_walk;
	int feat_num;
	std::vector<Walk> Walks;
	std::vector<int> nodes_ids;
	std::string graph_filename;
	int vertices_num;
	int iterations_number;
	
	//DeepWalk(std::string fNameGraph, int walks_per_node_num, int in_walk_length, int in_feat_num, bool graphFile_binary);
	DeepWalk(std::string fNameGraph, int walks_per_node_num, int in_walk_length, int in_feat_num, bool graphFile_binary, int _iterations_number);
	bool corpus_build(std::string outFile_name, std::string filetype);
	bool corpus_save(std::string outFile_name);
	bool fv_gen_and_save(std::string corpusFile_name, std::string fvFile_name);
	Walk random_walk_gen(int walk_length, int srand_val, int snode_id);
	std::vector<Walk> walks_gen_cuda(int walk_length, int srand_val, int snode_id);
	Walk random_walk_gen_cuda(int walk_length, int srand_val, int snode_id);


	void beforeRun(); // The method that will be called before any iterations start.
	void afterRun(); // The method that will be called after all iterations are done.
	void beforeIteration(); // The method that will be called just before an iteration starts.
	void afterIteration(); // The method that will be called just after an iteration completes.
	void update(); // The method in which vertex updates need to happen. Will be called on every iteration for every interval.
	bool hasConverged(); // Should return true if the algorithm converged during execution.
	void assembleAndSaveFullResultSet(); // Will be called at the end of the execution to assemble results to files that can be sent back for visualization.
};


#endif