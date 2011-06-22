//chr_separation.h
#ifndef CHR_SEPARATION_H
#define CHR_SEPARATION_H

#include <string>
#include <iostream>

int read_file(char*, std::ifstream&);
std::string find_chr(std::string);
int build_chr(std::string);
int chr_separation(int, char*argv[]);

#endif
