/*
Build Common Interval(s) from alignment Bowtie file(s).
Take file(s) produced before to build common regions.

Starting_point Ending_point Gap_size (referred to next interval)
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "common_interval.h"
#include "chr_separation.h"

using namespace std;

vector <interval*>* merge_intervals(vector <interval*>& regions){
    
    vector <interval*>* cont_regions = new vector <interval*>;
    interval* tmp_int = new interval(regions.at(0)->getBegin(),regions.at(0)->getEnd(),regions.at(0)->getSeq());
    for(unsigned int i=0; i<regions.size();i++){
        if(regions.at(i)->getBegin()<=tmp_int->getEnd()){
            if(regions.at(i)->getEnd()>=tmp_int->getEnd())
                tmp_int->setEnd(regions.at(i)->getEnd());
        }
        else{
            cont_regions->push_back(tmp_int);
            tmp_int = new interval(regions.at(i)->getBegin(),regions.at(i)->getEnd(),regions.at(i)->getSeq());
        }
        delete regions.at(i);
    }
    cont_regions->push_back(tmp_int);
    return cont_regions;
}

int common_interval(int argc, char* argv[]){

    srand(time(NULL));
    if(argc <= 2){
        std::cerr << endl << "Usage: map_analyzer common_interval <sorted_parsed_file>" << endl << endl;
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
    std::cerr << "Start Processing File..." << endl;
    ifstream align_file;
    string line = "";
    if(read_file(argv[1], align_file)==0){
        /*SVILUPPO
        1) Scelta casuale
        2) Smistare in base al cromosoma
        3) Comporre gli intervalli
        */
        vector <interval*> regions[25];
        while(getline(align_file,line)){
            char ch = '\0';
            string int_1 = "";
            string int_2 = "";
            string num_align = "";
            string chrom = "";
	    string seq = "";
            unsigned int end_len = 0;
            int tab = 0;
            while(end_len < line.length()){
                ch = line.at(end_len);
                end_len++;
                if(tab == 0 && ch != '\t'){
                    num_align += ch;
                }
                if(tab == 2 && ch != '\t'){
                    chrom += ch;
                }
                if(tab == 3 && ch != '\t'){
                    int_1 += ch;
                }
                if(tab == 4 && ch != '\t'){
                    int_2 += ch;
	        }
                if(tab == 5 && ch != '\t'){
                    seq += ch;
	        }
                if(ch == '\t'){
                    tab++;
                }
            }
            
            int alignments = atoi(num_align.c_str());
            long pos_pe1 = atol(int_1.c_str());
            long pos_pe2 = atol(int_2.c_str());
            
            if(pos_pe1 != 0 && pos_pe2 != 0){
                if(alignments == 1){
		  regions[atoi(chrom.c_str()) - 1].push_back(new interval(pos_pe1, pos_pe2, seq));
                }
                else{

                    //Scelta Allineamento
                    int offset = rand() % alignments;
                    //cout << "offset " << offset << endl;
                    if(offset == 0){
		      regions[atoi(chrom.c_str()) - 1].push_back(new interval(pos_pe1, pos_pe2, seq));
                        //cout << "line0 " << line << endl;
                    }
                    for(int i = 1; i<alignments; i++){
                        getline(align_file,line);
                        read_size+=(line.length()*sizeof(char));
                        if ((read_size/(double)file_dimension)*100 - 1 >= perc) {
                            perc++;
                            time_t diff_time = time(NULL) - execution_time;
                            time_t exp_time = diff_time/(double)perc * (100 - perc);
                            if(perc % 10 == 0){
                                std::cerr << "Processed: " << perc << "% - Excution Time: " << diff_time << " second(s) - Expected Time: " << exp_time << " second(s)" << endl;
                            }
                        }
                        if(i == offset){
                            //Push an alignment
                            ch = '\0';
                            int_1 = "";
                            int_2 = "";
                            end_len = 0;
                            chrom = "";
			    seq = "";
                            tab = 0;
                            while(end_len < line.length()){
                                ch = line.at(end_len);
                                end_len++;
                                if(tab == 2 && ch != '\t'){
                                    chrom += ch;
                                }
                                if(tab == 3 && ch != '\t'){
                                    int_1 += ch;
                                }
                                if(tab == 4 && ch != '\t'){
                                    int_2 += ch;
                                }
                                if(tab == 5 && ch != '\t'){
                                    seq += ch;
                                }
                                if(ch == '\t'){
                                    tab++;
                                }
                            }
                            
                            pos_pe1 = atol(int_1.c_str());
                            pos_pe2 = atol(int_2.c_str());
                            regions[atoi(chrom.c_str()) - 1].push_back(new interval(pos_pe1, pos_pe2, seq));
                        }
                    }
                }
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
        std::cerr << "File Processed...100%" << endl;
        align_file.close();
        //Sort the vector...
        std::cerr << "Merging intervsals for each chromosome..." << endl;
        for(int i=0;i < 25;i++){
            stable_sort(regions[i].begin(),regions[i].end(),less_than());
        }
        //...and merge the intervals.
        vector <interval*>* cont_regions[25];
        for(int i=0;i < 25;i++){
            cont_regions[i]= merge_intervals(regions[i]);
	}
        std::cerr << "Intervsals for each chromosome merged...100%" << endl;
        
        //Write intervals on file
        string interval_file = argv[1];
        interval_file += "_intervals_CHR";
        ofstream ifile;
        for(int z=0;z < 25;z++){
            std::stringstream str;
            str << z+1;
            std::string result;
            str >> result;
            string new_file = interval_file;
            new_file += result;
            long last_insert = 0;
            long gap_size = 0;
            ifile.open(new_file.c_str());
            if (ifile.is_open()){
                for(unsigned int i=0;i<cont_regions[z]->size();i++){
                    ifile << cont_regions[z]->at(i)->getBegin();
                    ifile << '\t';
                    ifile << cont_regions[z]->at(i)->getEnd();
                    ifile << '\t';
                    gap_size = cont_regions[z]->at(i)->getBegin() - last_insert;
                    ifile << gap_size;
		    ifile << '\t';
		    ifile << cont_regions[z]->at(i)->getSeq();
                    ifile << '\n';
                    last_insert = cont_regions[z]->at(i)->getEnd();
                }
                ifile << "Intervals Before merge: " << regions[z].size() << '\n';
                ifile << "Intervals After merge: " << cont_regions[z]->size() << '\n';
                ifile.close();
                cout << "Intervals Before merge CHR " << z+1 << ": " << regions[z].size() << endl;
                cout << "Intervals After merge  CHR " << z+1 << ": " << cont_regions[z]->size() << endl;
            }
            else{
                std::cerr << "Error writing interval file " << interval_file << endl;
            }
        }
        return 0;
    }
    else{
        return -1;
    }
}
