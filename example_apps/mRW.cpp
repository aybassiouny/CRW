//#define DYNAMICEDATA 1

#include <string>
//#include <posix4win.h>
#include "graphchi_basic_includes.hpp"
//#include "api/dynamicdata/chivector.hpp"
//#include "util/toplist.hpp"
#include <utility>
#include <algorithm> 

using namespace graphchi;
using namespace std;
/**
 * Type definitions. Remember to create suitable graph shards using the
 * Sharder-program.
 */

typedef int VertexDataType;
typedef int EdgeDataType;
 

 
typedef graphchi_vertex<VertexDataType, EdgeDataType> graphchiVec;

int main(int argc, const char ** argv) {
    graphchiVec temp;
    int *xx = new int;
	*xx = 500; int i=0;
	cout<<"Location of x is "<<std::dec<<xx<<" and its value is "<<*xx<<endl<<endl;
    temp.inc = 1+i; temp.outc = 2+i; temp.dataptr = xx; temp.modified = 1; temp.parallel_safe = 1;
	temp.scheduled = 0; temp.vertexid = 3+i;

	char *x = (char*)&temp;
	int *val = new int;
	bool *bval = new bool;
	for(int i=0; i<64; i+=8){
		memccpy(val, x+i, 1, sizeof(int)); cout << *val << " ";
		memccpy(bval, x+i, 1, sizeof(bool)); cout << *bval  << endl;
	}

    return 0;
}
