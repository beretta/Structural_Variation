/*
Parse the alignments and return as output:
Number_of_alignments PE_number Left_align_position Rigth_align_position

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <string>
#include <algorithm>

#include "chr_separation.h"

using namespace std;

string get_position(string line){
    char ch = '\0';
    string end = "";
    unsigned int end_len = 0;
    int tab = 0;
    while(end_len < line.length()){
	ch = line.at(end_len);
	end_len++;
	if(tab == 3 && ch != '\t'){
	    end += ch;
	}
	if(ch == '\t'){
	    tab++;
	}
    }
    return end;
}


string get_seq(string line){
  char ch = '\0';
  string seq = "";
  unsigned int end_len = 0;
  int tab = 0;
  while(end_len < line.length()){
    ch = line.at(end_len);
    end_len++;
    if(tab == 4 && ch != '\t'){
      seq += ch;
    }
    if(ch=='\t'){
       tab++;
    }
  }
return seq;
 }

string get_chromosome(string line){
    char ch = '\0';
    string chromosome = "";
    unsigned int end_len = 0;
    int tab = 0;
    while(end_len < line.length()){
	ch = line.at(end_len);
	end_len++;
	if(tab == 2 && ch != '\t'){
            chromosome += ch;
	}
	if(ch == '\t'){
	   tab++;
	}
    }
    chromosome = chromosome.substr(3,chromosome.size() - 3);
    if(chromosome == "X") return "23";
    if(chromosome == "Y") return "24";
    if(chromosome == "M") return "25";
    if(chromosome.size() > 1) return chromosome;
    else return "0" + chromosome;
}


int parse_alignment(int argc, char *argv[]) {
	if(argc <= 2){
		std::cerr << endl << "Usage: map_analyzer parse_alignment <bowtie_align_file>" << endl << endl;
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
        //Percentage counting
	long long read_size = 0;
	int perc = 0;
	time_t execution_time = time(NULL);
        
        //Parsing
	ifstream align_file;
	string line = "";
        bool different_PE = false;
        string s = "";
        queue <string> q;
	bool first_end = true;
	string first_pos = "";
	string second_pos = "";
        string chromosome = "";
	string seq = "";
	
        //Out File
	string last_pe = "";
	int count = 1;
	string out_file = argv[1];
	out_file += "_parsed";
	ofstream outfilestream;
	outfilestream.open(out_file.c_str());
	
        //Processing Lines
	if(read_file(argv[1], align_file)==0 && outfilestream.is_open()){
		std::cerr << "Start Reading..." << endl;
		while (getline(align_file,line)){
			//PE name from alignment
			int i = 0;
			string paired_end = "";
			while(line.at(i)!=' '){
				paired_end+= line.at(i);
				i++;
			}
			if(first_end){;
                            //first_pos = atol(get_position(line).c_str());
                            first_pos = get_position(line);
                            chromosome = get_chromosome(line);
			     seq = get_seq(line);
                            first_end = false;
			}
			else{
                            first_end = true;
                            //second_pos = atol(get_position(line).c_str());
                            second_pos = get_position(line);
			    if(strcmp(paired_end.c_str(),last_pe.c_str())==0){
				count++;
                            }else{
                                last_pe = paired_end;
                                different_PE = true;
                            }
                            if(different_PE){
                                while(!q.empty()){
                                    outfilestream << count << '\t';
                                    outfilestream << q.front() << '\n';
                                    q.pop();
                                }
                                different_PE = false;
                                count = 1;
                            }
                            s += paired_end;
                            s += '\t';
                            s += chromosome;
                            s += '\t';
                            s += first_pos;
                            s += '\t';
                            s += second_pos;
			    s += '\t';
			    s += seq;
                            q.push(s);
                            s = "";
                        }
			read_size+=(line.length()*sizeof(char));
			if ((read_size/(double)file_dimension)*100 - 1 >= perc) {
				perc++;
				time_t diff_time = time(NULL) - execution_time;
				time_t exp_time = diff_time/(double)perc * (100 - perc);
                                if(perc % 10 == 0){
                                    std::cerr << "Processed: " << perc << "% - Excution Time: " << diff_time << " second(s) - Expected Time: " << exp_time << " second(s)" << endl;
                                }
			}
		}
		std::cerr << "Processed: 100%" << endl;
  		align_file.close();
		outfilestream.close();
	}
	else{
		return -1;
	}
	return 0;
}
