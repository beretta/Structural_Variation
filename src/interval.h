//interval.h
#ifndef INTERVAL_H
#define INTERVAL_H

class interval{
	private:
		long begin;
		long end;

	public:
		interval(long, long);
		long getBegin();
		long getEnd();
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
