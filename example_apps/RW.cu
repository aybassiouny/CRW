#define DYNAMICEDATA 1

#include <string>
//#include <posix4win.h>
#include "graphchi_basic_includes.hpp"
#include "api/dynamicdata/chivector.hpp"
#include "util/toplist.hpp"
#include <utility>
#include <algorithm> 

using namespace graphchi;
using namespace std;
/**
 * Type definitions. Remember to create suitable graph shards using the
 * Sharder-program.
 */

typedef unsigned int VertexDataType;
typedef chivector<vid_t> pvid_t;
typedef pvid_t EdgeDataType;

vector< vector<vid_t> > walks;
 
const int _walks_per_source = 10, 
            _steps_per_walk = 5;


__global__ void update(int *inc, int *outc, int *rpt, int **res )//int ***vData,int *Voutc, int** res)
    //graphchi_vertex<VertexDataType, EdgeDataType > &vertex, graphchi_context &gcontext) 
{
    int curVer = cudastuff;
    for(int i=0; i<rpt[curVer]; i++){
        int *edge_rand =  new int; 
        *edge_rand = rand()%outc[curVer];
        res[curVer][i] = *edge_rand;
        atomicAdd(rpt+(*edge_rand), 1);
    }
    
    // if (iter == 0) {
    //     for(int i=0; i < _walks_per_source; i++) {
    //         int edge_rand = rand()%outc;
    //          //graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
    //          vector<vid_t> walk;
    //          chivector<vid_t> * evector = outedge->get_vector();
    //          int x = vertex.id()*walks_per_source()+i;
    //          evector->add(x);
    //          //return outedge->vertex_id(); // Schedule destination
    //     }
    // } else {
    //     for(int i=0; i < inc; i++) {
    //         graphchi_edge<EdgeDataType> * edge = vertex.inedge(i);
    //         chivector<vid_t> *invector = edge->get_vector();
            
    //         for (int j = 0; j < invector->size(); j++){
    //             if (walks[invector->get(j)].size() < steps_per_walk()){
    //                 graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
    //                 chivector<vid_t> *outvector = outedge->get_vector();
                    
    //                 walks[invector->get(j)].push_back(vertex.id());
                    
    //                 outvector->add(invector->get(j));
    //                 gcontext.scheduler->add_task(outedge->vertex_id()); // Schedule destination
    //             }
    //         }
    //         invector->clear();
    //     }
        
    // }
    
}


//static int x =0;
pthread_mutex_t lock;
struct RandomWalkProgram : public GraphChiProgram<VertexDataType, EdgeDataType> {

    int steps_per_walk()
    {
        return _steps_per_walk;
    }
    int walks_per_source() 
    {
        return _walks_per_source;
    }
    
    /**
     *  Vertex update function.
     */
    void update(graphchi_vertex<VertexDataType, EdgeDataType > &vertex, graphchi_context &gcontext) {
        
        if (gcontext.iteration == 0) {
            
            for(int i=0; i < walks_per_source(); i++) {
                 graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
                 if (outedge != NULL) {
                     vector<vid_t> walk;
                     chivector<vid_t> * evector = outedge->get_vector();
                     int x = vertex.id()*walks_per_source()+i;
                     evector->add(x);
                     //cout<<vertex.id()<<" "<<x<<endl;
                     gcontext.scheduler->add_task(outedge->vertex_id()); // Schedule destination
                     
                 }
                 else cout<<"WTF"<<endl;
            }
        } else {
            for(int i=0; i < vertex.num_inedges(); i++) {
                graphchi_edge<EdgeDataType> * edge = vertex.inedge(i);
                chivector<vid_t> *invector = edge->get_vector();
                
                for (int j = 0; j < invector->size(); j++){
                    if (walks[invector->get(j)].size() < steps_per_walk()){
                        graphchi_edge<EdgeDataType> * outedge = vertex.random_outedge();
                        if (outedge != NULL) {
                            chivector<vid_t> *outvector = outedge->get_vector();
                            
                            walks[invector->get(j)].push_back(vertex.id());
                            
                            outvector->add(invector->get(j));
                            gcontext.scheduler->add_task(outedge->vertex_id()); // Schedule destination
                        }
                        else cout<<"WTF"<<endl;
                    }
                }
                
                invector->clear();
            }
            
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
        if (gcontext.iteration == 0){
            cout<<"I am after iteration 0"<<endl;
        }
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
    int niters           = get_option_int("niters", 100); // Number of iterations
    bool scheduler       = true;                       // Whether to use selective scheduling
    
    /* Detect the number of shards or preprocess an input to create them */
    bool preexisting_shards;
    int nshards          = convert_if_notexists<vid_t>(filename, get_option_string("nshards", "auto"), preexisting_shards);
    
    


    /* Run */
    RandomWalkProgram program;
    
    //initialize walks

    int numWalks= program.walks_per_source()*numV;
    cout<<"numWalks is "<<numWalks<<endl;

    ofstream out("walks.txt");;
    for(int i=0; i<numV; i++){
        vector<vid_t> walk; walk.push_back(i);
        for(int j=0; j<program.walks_per_source(); j++)
            walks.push_back(walk);
    }
    // for(int i=0; i < min(int(walks.size()), 1000); i++) {
    //     for (int j = 0; j < walks[i].size(); j++)
    //         out << walks[i][j]<<" ";
    //     out << std::endl;
    // }
    // out<<"#################################################################"<<endl;
    // out<<"#################################################################"<<endl;
    // out<<"#################################################################"<<endl;

    //go
    graphchi_engine<VertexDataType, EdgeDataType> engine(filename, nshards, scheduler, m);
    if (preexisting_shards) {
        engine.reinitialize_edge_data(0);
    }
    engine.run(program, niters);
    
    /* List top 20 */
    //std::vector< vertex_value<VertexDataType> > top = get_top_vertices<VertexDataType>(filename, ntop);
    //std::cout << "Print top 20 vertices: " << std::endl;
    
    
    for(int i=0; i < min(int(walks.size()), 1000); i++) {
        for (int j = 0; j < walks[i].size(); j++)
            out << walks[i][j]<<" ";
        out << std::endl;
    }
    out.close();
    out<< walks.size()<<" "<<walks[0].size()<<endl;
    /* Report execution metrics */
    metrics_report(m);
    return 0;
}
