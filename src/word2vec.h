#pragma once


#include "stdafx.h"



int word2vec_main(std::string in_corpusFile_name, std::string in_outFile_name, int in_hs, int in_iter, int in_min_count, int in_cbow, int in_layer1_size, 
	int in_threads_num);