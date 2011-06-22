/*
Build Common Region(s) from alignment Bowtie file(s) splitted into Chromosomes.

Take file(s) produced before to build common regions.
It's possbile to skip the Chromosome separation step and build common regions from
the entire Bowtie Alignment file

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "build_regions.h"

#define CHR_IDENTIFIER "NC_00"

using namespace std;

int read_chromosome(char* file_name, ifstream& myfile) {
	
	myfile.open(file_name, ios::in);
	if(myfile.is_open())
		return 0;
	else{
		cout << endl << "Unable to open file " << file_name << endl << endl; 
		return 1;
	}
}

string find_align_position(string line){

	unsigned int chr_pos = line.find(CHR_IDENTIFIER);
	string align_pos = "";
	if(chr_pos != string::npos){
		unsigned int start = line.find('\t',chr_pos+1)+1;
		if(start != string::npos){
			int stop = line.find('\t',start+1);
			align_pos = line.substr(start,stop-start);
		}
		else{
			align_pos = "Undef";		
		}
	}
	else{
		align_pos = "Undef";
	}
	return align_pos;
}

vector <interval*>* merge_regions(vector <interval*>& regions){
	vector <interval*>* cont_regions = new vector <interval*>;
	interval* tmp_int = new interval(regions.at(0)->getBegin(),regions.at(0)->getEnd());
	for(unsigned int i=0; i<regions.size();i++){
		if(regions.at(i)->getBegin()<=tmp_int->getEnd()){
			if(regions.at(i)->getEnd()>=tmp_int->getEnd())
				tmp_int->setEnd(regions.at(i)->getEnd());
		}
		else{
			cont_regions->push_back(tmp_int);
			tmp_int = new interval(regions.at(i)->getBegin(),regions.at(i)->getEnd());
		}
		delete regions.at(i);
	}
	return cont_regions;
}

int build_regions(int argc, char* argv[]){

	if(argc <= 2){
		cout << endl << "Usage: map_analyzer build_regions <chromosome_**** file>" << endl << endl;
		return -1;	
	}
	vector <interval*> regions;
	ifstream align_file;
	string pe1 = "";
	string pe2 = "";
	if(read_chromosome(argv[1], align_file)==0){
		cout << "Reading intervals...";
		while (! align_file.eof() ){
			char ch = '\0';
			int seq_length = 0;
			unsigned int line_len = 0;
			int tab = 0;
			getline(align_file,pe1);
			long pos_pe1 = atol(find_align_position(pe1).c_str());
			getline(align_file,pe2);
			long pos_pe2 = atol(find_align_position(pe2).c_str());
			/*
			NOTE:
			pos_pe{1,2} is the start point of the end alignment
			it's necessary to sum the length of the end sequence
			*/
			while(line_len < pe2.length()){
				ch = pe2.at(line_len);
				line_len++;
				if(ch == '\t'){
					tab++;
				}
				if(tab == 4){
					seq_length++;
				}
			}
			if(pos_pe1 != 0 && pos_pe2 != 0){
				regions.push_back(new interval(pos_pe1, pos_pe2 + (seq_length-1) ));
			}
    		}
		cout << "Done!" << endl;
  		align_file.close();
		//Sort the vector...
		stable_sort(regions.begin(),regions.end(),less_than());
		cout << "Merging intervsals...";
		//...and merge the intervals.
		vector <interval*>* cont_regions = merge_regions(regions);
		cout << "Done!" << endl;
		cout << "Intervals Before merge: " << regions.size() << endl;
		cout << "Intervals After merge: " << cont_regions->size() << endl;
		//Write intervals on file
		string interval_file = argv[1];
		interval_file += "_intervals";
		ofstream ifile;
		ifile.open(interval_file.c_str());
		if (ifile.is_open()){
			for(unsigned int i=0;i<cont_regions->size();i++){
				ifile << cont_regions->at(i)->getBegin();
				ifile << '\t';
				ifile << cont_regions->at(i)->getEnd();
				ifile << '\n';
			}
			ifile.close();
		}
		else cout << "Error writing interval file " << interval_file << endl;

		return 0;
	}
	else{
		return -1;	
	}
	return 0;
}

