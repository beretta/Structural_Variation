/*
Count the number of alignments for each Paired-End

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "chr_separation.h"

using namespace std;

int count_alignment(int argc, char *argv[]) {
	if(argc <= 2){
		cout << endl << "Usage: map_analyzer count_alignment <bowtie_align_file>" << endl << endl;
		return -1;	
	}
	long long file_dimension = 1;
	ifstream f(argv[1],ios::in|ios::binary|ios::ate);
	if(f.is_open()){
		file_dimension = f.tellg();
  		f.close();
	}
	else{
		return -1;	
	}
	long long read_size = 0;
	int perc = 0;
	time_t execution_time = time(NULL);
	ifstream align_file;
	string line = "";
	//Out File
	string last_pe = "";
	int count = 0;
	string out_file = argv[1];
	out_file += "_intervals";
	ofstream outfilestream;
	outfilestream.open(out_file.c_str());
	//Processing Lines
	if(read_file(argv[1], align_file)==0 && outfilestream.is_open()){
		cout << "Start Reading..." << endl;
		while (getline(align_file,line)){
			//PE name from alignment
			int i = 0;
			string paired_end = "";
			while(line.at(i)!=' '){
				paired_end+= line.at(i);
				i++;
			}
			if(strcmp(paired_end.c_str(),last_pe.c_str())==0){
				count++;
			}
			else{
				count = count / 2;
				outfilestream << last_pe << '\t';
				outfilestream << count << '\n';
				last_pe = paired_end;
				count = 1;
			}
			read_size+=(line.length()*sizeof(char));
			if ((read_size/(double)file_dimension)*100 - 1 >= perc) {
				perc++;
				time_t diff_time = time(NULL) - execution_time;
				time_t exp_time = diff_time/(double)perc * (100 - perc);
				cout << "Processed: " << perc << "% - Excution Time: " << diff_time << " second(s) - Expected Time: " << exp_time << " second(s)" << endl;
			}
		}
		cout << "Processed: 100%" << endl;
  		align_file.close();
		outfilestream.close();
	}
	else{
		return -1;
	}
	return 0;
}
