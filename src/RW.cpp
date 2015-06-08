#include <string>
#include "graphchi_basic_includes.hpp"
#include "api/dynamicdata/chivector.hpp"
#include "util/toplist.hpp"
#include <utility>
#include <fstream>
#include <algorithm> 


using namespace graphchi;
using namespace std;
/**
* Type definitions. Remember to create suitable graph shards using the
* Sharder-program.
*/

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



int main(int argc, const char ** argv) {

	/* GraphChi initialization will read the command line
	arguments and the configuration file. */
	graphchi_init(argc, argv);

	/* Metrics object for keeping track of performance counters
	and other information. Currently required. */
	metrics m("randomwalk");

	/* Basic arguments for application */
	std::string filename = get_option_string("file");  // Base filename
	int numV = get_option_int("numV");
	int niters = get_option_int("niters", 100); // Number of iterations
	bool scheduler = true;                       // Whether to use selective scheduling

	/* Detect the number of shards or preprocess an input to create them */
	bool preexisting_shards;
	int nshards = convert_if_notexists<vid_t>(filename, get_option_string("nshards", "auto"), preexisting_shards);

	const int _walks_per_source = 5, 
            _steps_per_walk = 10;
	/* Run */
	RandomWalkProgram program;

	//initialize walks
	
	int numWalks = _walks_per_source*numV;
	cout << "numWalks is " << numWalks << endl;

	ofstream out("walks.txt");;
	
	//go
	graphchi_engine<VertexDataType, EdgeDataType> engine(filename, nshards, scheduler, m, _walks_per_source, _steps_per_walk);
	if (preexisting_shards) {
		engine.reinitialize_edge_data(0);
	}
	engine.run(program, niters);

	/* List top 20 */
	//std::vector< vertex_value<VertexDataType> > top = get_top_vertices<VertexDataType>(filename, ntop);
	//std::cout << "Print top 20 vertices: " << std::endl;


	for (int i = 0; i < engine.completedWalks.size(); i++) {
		for (int j = 0; j < engine.completedWalks[i].size(); j++)
			out << engine.completedWalks[i][j] << " ";
		out << std::endl;
	}
	out.close();
	//out << walks.size() << " " << walks[0].size() << endl;
	/* Report execution metrics */
	metrics_report(m);
	return 0;
}
