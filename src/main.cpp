#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "chr_separation.h"
#include "build_regions.h"
#include "common_interval.h"
#include "count_alignment.h"
#include "parse_alignment.h"

using namespace std;

static int usage(){
	cout << endl << "Program: map_analyzer (Analyze Bowtie Alignement File)" << endl;
        cout << "Usage: map_analyzer <command>" << endl;
	cout << "Key commands:" << endl;
        cout << "\t parse_alignment  -> Parse the alignment file and create interval for each PE" << endl;
	cout << "\t common_interval  -> Build common interval(s) from Alignment Bowtie file(s)" << endl;
	cout << "\t count_alignment  -> Count the number of alignments for each Paired-End" << endl;
	cout << "\t OLD!!! chr_separation -> Separate Bowtie Alignment file into Chromesome file(s)" << endl;
	cout << "\t OLD!!! build_regions  -> Build common region(s) from Alignment Bowtie file(s) spliced into chromosomes" << endl;	
	cout << endl;
	return 1;
}

int main(int argc, char* argv[]){

	if (argc < 2) return usage();
	
        if (strcmp(argv[1], "chr_separation") == 0) return chr_separation(argc, argv+1);
	else if (strcmp(argv[1], "build_regions") == 0) return build_regions(argc, argv+1);
	else if (strcmp(argv[1], "common_interval") == 0) return common_interval(argc, argv+1);
	else if (strcmp(argv[1], "count_alignment") == 0) return count_alignment(argc, argv+1);
	else if (strcmp(argv[1], "parse_alignment") == 0) return parse_alignment(argc, argv+1);
	else {
        	cout << endl << "ERROR: Unrecognized command " << argv[1] << endl << endl;
        	return -1;
        }
}
