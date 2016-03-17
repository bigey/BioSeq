#include "SA_LCP.h"

using namespace std;

/**
 * String comparison
 */
struct SAcomp {
    const string& seq;
    SAcomp(const string &s): seq(s) {}
    
    bool operator()(size_t a, size_t b) const
    {
        size_t length = seq.size();
        
        while( (a < length) && (b < length) && (seq[a] == seq[b]) ) {
            a++; b++;
        }
        
        return (a >= length) || ( (b < length) && (seq[a] < seq[b]) );
    }
};


/**
 * Building suffix array
 */
vector<size_t> buildSA(const string& s)
{
    vector<size_t> sa(s.size());

    // iota(sa.begin(), sa.end(), 0);
    for(size_t i = 0; i < sa.size(); i++)
        sa[i] = i;

    sort(sa.begin(), sa.end(), SAcomp(s));
    return sa;
}

/**
 * Building LCP array
 */
vector<size_t> buildLCP(const string& s, vector<size_t>& sa)
{
    vector<size_t> lcp(sa.size());
    lcp[0] = 0;
    
    string prev_s = "$";
    size_t prev_l = 0;
    
    for(size_t k = 1; k < sa.size(); k++)
    {
        string suffix = s.substr(sa.at(k), string::npos);
        size_t s_length = suffix.size();
        
        size_t i = 0;
        
        while( (i < prev_l) && (i < s_length) && (prev_s[i] == suffix[i]) ) {
            i++; 
        }
        
        lcp[k] = i;
        prev_s = suffix;
        prev_l = s_length;
    }
    return lcp;
}
