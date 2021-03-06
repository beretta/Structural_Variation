/*
Interval class

Class used to model an interval of alignment point (paired-ends aligned to a reference genome).
*/

#include <iostream>
#include <string>
#include "interval.h"

using namespace std;

interval::interval(long begin, long end, string seq){
	this->begin = begin;
	this->end = end;
	this->seq = seq;
}

long interval::getBegin(){
	return begin;
}

long interval::getEnd(){
	return end;
}

string interval::getSeq(){
        return seq;
}

void interval::setBegin(long begin){
	this->begin = begin;
}

void interval::setEnd(long end){
	this->end = end;
}

void interval::visualize(){
  cout << begin << '\t' << end << '\t' <<seq << endl;
}

bool interval::operator<(const interval& in) const{
	return begin<in.begin;
}

bool interval::operator<=(const interval& in) const{
	return begin<=in.begin;
}

bool interval::operator>(const interval& in) const{
	return begin>in.begin;
}

bool interval::operator>=(const interval& in) const{
	return begin>=in.begin;
}

bool interval::operator==(const interval& in) const{
	return begin==in.begin;
}
