#include "SuffixArray.h"

using namespace std;


/**
 * String comparison
 */
struct SaComp {
    const GenericSeq& gs;
    SaComp(const GenericSeq& s): gs(s) {}
    
    bool operator() (size_t a, size_t b) const
    {
        size_t length = gs.get_length();
        while ((a < length) && (b < length) && (gs[a] == gs[b])) {
            a++; b++;
            //cout << a << " " << b << endl;
        }
        
        //cout << "gs[" << a << "]:" << gs[a] << " == gs[" << b << "]:" << gs[b] << endl;
        return (a >= length) || ((b < length) && (gs[a] < gs[b]));
    }
};


/**
 * Constructor
 */
SuffixArray::SuffixArray() {}


/**
 * Constructor using an GenericSeq object 
 */
SuffixArray::SuffixArray(GenericSeq& s): length(s.get_length()), gs(s), sa(NULL), lcp(NULL)
{
    // Memory allocation for SA and LCP array
    if (length)
    {
        sa  = new size_t[length];
        lcp = new size_t[length];
    }
    
    // SA construction
    buildSa();
    
    // LCP construction
    buildLcp();
}


/**
 * Copy constructor
 */
SuffixArray::SuffixArray(SuffixArray const& s): length(s.length), gs(s.gs), sa(NULL), lcp(NULL)
{
    if (length)
    {
        sa  = new size_t[length];
        lcp = new size_t[length];
        
        for (size_t i = 0; i < length; i++)
        {
            sa[i]  = s.sa[i];
            lcp[i] = s.lcp[i];
        }
    }
}


/**
 * Affectation operator (=)
 */
SuffixArray& SuffixArray::operator=(SuffixArray const& su)
{
    if(this != &su)
    {
        if (length != su.length)
        {
            if (length)
            {
                delete[] sa;  sa  = NULL;
                delete[] lcp; lcp = NULL;
            }
            
            length = su.length;
            
            if (length)
            {
                sa  = new size_t[length];
                lcp = new size_t[length];
            }
        }
        
        if (length)
        {
            for (size_t i = 0; i < length; i++)
            {
                sa[i]  = su.sa[i];
                lcp[i] = su.lcp[i];
            }
        }
        
    }
    return *this;
}


/**
 * Destructor
 */
SuffixArray::~SuffixArray()
{
    if (length) {
        delete[] sa;
        delete[] lcp;
    }
}


size_t SuffixArray::get_length() const
{
    return length;
}

size_t SuffixArray::operator[] (size_t i) const
{
    if (i < length)
        return sa[i];
    else
        return 0;
}

size_t SuffixArray::get_lcp(size_t i) const
{
    if (i < length)
        return lcp[i];
    else
        return 0;
}

/**
 * Building suffix array
 */
void SuffixArray::buildSa()
{
    // iota(sa.begin(), sa.end(), 0);
    for(size_t i = 0; i < length; i++)
        sa[i] = i;

    sort(sa, sa + length, SaComp(gs));
}


/**
 * Building LCP array
 */
void SuffixArray::buildLcp()
{
    lcp[0] = 0;
    size_t curr_suff_lg, prev_suff_lg;
    
    for (size_t k = 1; k < length; k++)
    {
        curr_suff_lg = length - sa[k];
        prev_suff_lg = length - sa[k-1];
        
        size_t i = 0;
        
        while ( (i < curr_suff_lg) && (i < prev_suff_lg) && (gs[sa[k-1]+i] == gs[sa[k]+i]) )
        {
            i++; 
        }
        
        lcp[k] = i;
    }
}
