//#define DYNAMICEDATA 1

#include <string>
//#include <posix4win.h>
//#include "graphchi_basic_includes.hpp"
//#include "api/dynamicdata/chivector.hpp"
//#include "util/toplist.hpp"
#include <utility>
#include <algorithm> 
#include <map>
#include <vector>
#include <iostream>
#include <typeinfo>
//using namespace graphchi;
using namespace std;
/**
 * Type definitions. Remember to create suitable graph shards using the
 * Sharder-program.
 */

//typedef int VertexDataType;
//typedef int EdgeDataType;
 

 
//typedef graphchi_vertex<VertexDataType, EdgeDataType> graphchiVec;

int main(int argc, const char ** argv) {
	
	multimap<int, vector<int> > mmap;
	vector<int> v; v.push_back(11);
	mmap.insert(std::make_pair(1, v));
	v.clear();v.push_back(111);
	mmap.insert(std::make_pair(1, v));
	
	v.clear();v.push_back(22);
	mmap.insert(std::make_pair(2, v));
	v.clear();v.push_back(222);
	mmap.insert(std::make_pair(2, v));
	
	std::cout << "elements in mymm:" << '\n';
	
	auto it = mmap.find(1);
	std::cout << "y => " << it->second[0] << '\n';
	//it->first = 2;
	mmap.insert(std::make_pair(2, it->second));
	mmap.erase(it);
	it = mmap.find(1);
	 std::cout << "y => " << it->second[0] << '\n';
	it = mmap.find(2);
	std::cout << "z => " << it->second[0] << '\n';
	it++; std::cout << "y => " << it->second[0] << '\n';
	it++; std::cout << "y => " << it->second[0] << '\n';
	it++; std::cout << "y => " << it->second[0] << '\n';
//     graphchiVec temp;
//     int *xx = new int;
// 	*xx = 500; int i=0;
// 	cout<<"Location of x is "<<std::dec<<xx<<" and its value is "<<*xx<<endl<<endl;
//     temp.inc = 1+i; temp.outc = 2+i; temp.dataptr = xx; temp.modified = 1; temp.parallel_safe = 1;
// 	temp.scheduled = 0; temp.vertexid = 3+i;
// 
// 	char *x = (char*)&temp;
// 	int *val = new int;
// 	bool *bval = new bool;
// 	for(int i=0; i<64; i+=8){
// 		memccpy(val, x+i, 1, sizeof(int)); cout << *val << " ";
// 		memccpy(bval, x+i, 1, sizeof(bool)); cout << *bval  << endl;
// 	}
// 
//     return 0;
}
