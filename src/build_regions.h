//build_regions.h
#ifndef BUILD_REGIONS_H
#define BUILD_REGIONS_H

#include <string>
#include <vector>

#include "interval.h"

int read_chromosome(char*, std::ifstream&);
std::string find_align_position(std::string);
std::vector <interval*>* merge_regions(std::vector <interval*>&);
int build_regions(int, char*argv[]);

#endif
