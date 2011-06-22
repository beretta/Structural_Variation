/*
Separate Bowtie Alignment file into Chromesome file(s).

Bowtie align paired-ends into the reference genome and indicate the Chromosome in which it's aligned.
It's possible to separate lines of Bowtie Alignment file into Chromosome file(s), each one containing 
paired-ends aligned in the corrisponding chromosome.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "chr_separation.h"

#define CHR_IDENTIFIER "NC_00"

using namespace std;

int read_file(char* file_name, ifstream& myfile) {
	
	myfile.open(file_name, ios::in);
	if(myfile.is_open())
		return 0;
	else{
		cout << endl << "Unable to open file " << file_name << endl << endl; 
		return 1;
	}
}

string find_chr(string line){
	
	unsigned int chr_pos = line.find(CHR_IDENTIFIER);
	string chr = "";
	if(chr_pos != string::npos){
		chr_pos+=5;
		chr = line.at(chr_pos);
		chr+= line.at(chr_pos+1);
		chr+= line.at(chr_pos+2);
		chr+= line.at(chr_pos+3);
	}
	else{
		chr = "Undef";
	}
	return chr;
	//Conversione in int
	//return atoi(chr.c_str());
}

int build_chr(string line){

	if(line != ""){
		string chr = "chromosome_";
		chr += find_chr(line);
		ofstream chr_file (chr.c_str(), ios::app);
		chr_file << line << endl;
		chr_file.close();
	}
	return 0;
}


int chr_separation(int argc, char *argv[]) {

	if(argc <= 2){
		cout << endl << "Usage: map_analyzer chr_separation <bowtie_align_file>" << endl << endl;
		return -1;	
	}
	long file_dimension = 1;
	ifstream f(argv[1],ios::in|ios::binary|ios::ate);
	if(f.is_open()){
		file_dimension = f.tellg();
  		f.close();
	}
	else{
		return -1;	
	}
	long read_size = 0;
	int perc = 0;
	time_t execution_time = time(NULL);
	ifstream align_file;
	string line = "";
	if(read_file(argv[1], align_file)==0){
		while (! align_file.eof() ){
			getline(align_file,line);
			read_size+=(line.length()*sizeof(char));
			if ((read_size/(double)file_dimension)*100 - 1 >= perc) {
				perc++;
				time_t diff_time = time(NULL) - execution_time;
				time_t exp_time = diff_time/(double)perc * (100 - perc);
				cout << "Processed: " << perc << "% - Excution Time: " << diff_time << " second(s) - Expected Time: " << exp_time << " second(s)" << endl;
			}
			build_chr(line);
    		}
		cout << "Processed: 100%" << endl;
  		align_file.close();
		return 0;
	}
	else{
		return -1;	
	}
}
