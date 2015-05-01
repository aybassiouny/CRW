

/**
 * @file
 * @author  Aapo Kyrola <akyrola@cs.cmu.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright [2012] [Aapo Kyrola, Guy Blelloch, Carlos Guestrin / Carnegie Mellon University]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 
 *
 * @section DESCRIPTION
 *
 * Random walk simulation. From a set of source vertices, a set of 
 * random walks is started. Random walks walk via edges, and we use the
 * dynamic chivectors to support multiple walks in one edge. Each
 * vertex keeps track of the walks that pass by it, thus in the end
 * we have estimate of the "pagerank" of each vertex.
 *
 * Note, this version does not support 'resets' of random walks.
 * TODO: from each vertex, start new random walks with some probability,
 * and also terminate a walk with some probablity.
 *
 */

#define DYNAMICEDATA 1

#include <string>
//#include <posix4win.h>
#include "graphchi_basic_includes.hpp"
#include "api/dynamicdata/chivector.hpp"
#include "util/toplist.hpp"
#include <utility>

using namespace graphchi;
using namespace std;
/**
 * Type definitions. Remember to create suitable graph shards using the
 * Sharder-program.
 */

typedef unsigned int VertexDataType;
typedef chivector<vid_t> pvid_t;
typedef pvid_t EdgeDataType;

vector<chivector<vid_t>*> walks;
 
struct RandomWalkProgram : public GraphChiProgram<VertexDataType, EdgeDataType> {

    int steps_per_walk()
    {
        return 5;
    }
    int walks_per_source() 
    {
        return 1;
    }
    
    /**
     *  Vertex update function.
     */
    void update(graphchi_vertex<VertexDataType, EdgeDataType > &vertex, graphchi_context &gcontext) {
        if (gcontext.iteration == 0) {
            //if (is_source(vertex.id())) {
            /*for(int i=0; i < walks_per_source(); i++) {
                graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
                if (outedge != NULL) {
                    chivector<vid_t>* walk = new chivector < vid_t >;
                    chivector<vid_t> * evector = outedge->get_vector();
                    walk->add(vertex.id());
                    walks.push_back(walk);
                    evector->add(walks.size()-1);
                    gcontext.scheduler->add_task(outedge->vertex_id()); // Schedule destination
                }
            }*/
            //}
            //vertex.set_data(0);
        } else {
            /*for(int i=0; i < vertex.num_inedges(); i++) {
                graphchi_edge<EdgeDataType> * edge = vertex.inedge(i);
                chivector<vid_t> *invector = edge->get_vector();
                for (int j = 0; j < invector->size(); j++){
                    //vid_t curwalk = ;
                    if (walks[invector->get(j)]->size() < steps_per_walk()){
                        graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
                        if (outedge != NULL) {
                            chivector<vid_t> *outvector = outedge->get_vector();
                            walks[invector->get(j)]->add(vertex.id());
                            //walk.push_back(vertex.id());
                            outvector->add(invector->get(j));
                            gcontext.scheduler->add_task(outedge->vertex_id()); // Schedule destination
                        }
                    }
                }
                invector->clear();
            }*/
            //vertex.set_data(vertex.get_data() + num_walks);
        }
    }
    
    /**
     * Called before an iteration starts.
     */
    void before_iteration(int iteration, graphchi_context &gcontext) {
    }
    
    /**
     * Called after an iteration has finished.
     */
    void after_iteration(int iteration, graphchi_context &gcontext) {
    }
    
    /**
     * Called before an execution interval is started.
     */
    void before_exec_interval(vid_t window_st, vid_t window_en, graphchi_context &gcontext) {
    }
    
    /**
     * Called after an execution interval has finished.
     */
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
    int niters           = get_option_int("niters", 20); // Number of iterations
    bool scheduler       = true;                       // Whether to use selective scheduling
    
    /* Detect the number of shards or preprocess an input to create them */
    bool preexisting_shards;
    int nshards          = convert_if_notexists<vid_t>(filename, get_option_string("nshards", "auto"), preexisting_shards);
    
    /* Run */
    RandomWalkProgram program;
    graphchi_engine<VertexDataType, EdgeDataType> engine(filename, nshards, scheduler, m);
    if (preexisting_shards) {
        engine.reinitialize_edge_data(0);
    }
    engine.run(program, niters);
    
    /* List top 20 */
    //std::vector< vertex_value<VertexDataType> > top = get_top_vertices<VertexDataType>(filename, ntop);
    std::cout << "Print top 20 vertices: " << std::endl;
    for(int i=0; i < (int) walks.size(); i++) {
        for (int j = 0; j < walks[i]->size(); j++)
            std::cout << walks[i]->get(j)<<" ";
        std::cout << std::endl;
    }

    /* Report execution metrics */
    metrics_report(m);
    return 0;
}
