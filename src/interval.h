//interval.h
#ifndef INTERVAL_H
#define INTERVAL_H

//#include <string>
using std::string;

class interval{
	private:
		long begin;
		long end;
		string seq;

	public:
		interval(long, long, string);
		long getBegin();
		long getEnd();
		string getSeq();
		void setBegin(long);
		void setEnd(long);
		void visualize();
		bool operator<(const interval&) const;
		bool operator<=(const interval&) const;
		bool operator>(const interval&) const;
		bool operator>=(const interval&) const;
		bool operator==(const interval&) const;
};

class less_than{
public:
	bool operator()(const interval* in1, const interval* in2) const{
		return *in1<*in2;
	}

};
#endif
