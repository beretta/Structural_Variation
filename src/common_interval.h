//common_interval.h
#ifndef COMMON_INTERVAL_H
#define COMMON_INTERVAL_H

#include <vector>
#include "interval.h"

std::vector <interval*> * merge_intervals(std::vector <interval*> &);
int common_interval(int, char*argv[]);

#endif
