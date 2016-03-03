#include "SA.h"

using namespace std;

struct SAcomp {
	const string& seq;
	SAcomp(const string &s): seq(s) {}
	bool operator()(size_t a, size_t b) const
	{
		size_t length = seq.size();
		while( (a < length) && (b < length) && (seq[a] == seq[b]) ){
			a++; b++;
		}
		return (a >= length) || ( (b < length) && (seq[a] < seq[b]) );
	}
};

vector<size_t> buildSA(const string& s)
{
	vector<size_t> SA(s.size());
	
	// iota(sa.begin(), sa.end(), 0);
	for(size_t i = 0; i < SA.size(); i++)
		SA[i] = i;
	
	sort(SA.begin(), SA.end(), SAcomp(s));
	return SA;
}
