#include "SuffixArray.h"

using namespace std;


/**
 * Sequence comparison
 */
struct SaComp {

    GenericSeq gs;
    SaComp(const GenericSeq& s): gs(s) {}

    bool operator() (size_t a, size_t b) const
    {
        size_t length = gs.get_length();

        while ((a < length) && (b < length) && (gs[a] == gs[b]))
        {
            a++; b++;
        }

        return (a == length) || ((b < length) && (gs[a] < gs[b]));
    }
};


/**
 * Default constructor
 */
SuffixArray::SuffixArray() {}


/**
 * Constructor using an GenericSeq object
 */
SuffixArray::SuffixArray(const GenericSeq& gseq):
    length(gseq.get_length()), gs(gseq), sa(NULL), lcp(NULL)
{
    // Memory allocation for SA and LCP array
    if (length)
    {
        sa  = new size_t[length];
        lcp = new size_t[length];
    }

    // SA construction
    cerr << "   Builing SA...\n";
    buildSa();

    // LCP construction
    cerr << "   Building LCP...\n";
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


/**
 * Get the length of the suffix array table
 */
size_t SuffixArray::get_length() const
{
    return length;
}


/**
 * Get the id (name) of the indexed sequence
 */
string SuffixArray::get_id() const
{
    return gs.get_id();
}


/**
 * Operator at []
 */
size_t SuffixArray::operator[] (size_t i) const
{
    if (i < length)
        return sa[i];

    cerr << "ERROR: position " << i
         << " is greater than suffix array length ("
         << length << ")" << endl;

    throw;
}


/**
 * Building suffix array []
 */
void SuffixArray::buildSa()
{
    if ( length == 0 )
    {
        cerr << "ERROR: indexing a sequence of length 0!\n";
        throw;
    }

    clock_t start = clock();
    double cpu_time;

    for(size_t i = 0; i < length; i++)
        sa[i] = i;

    cerr << "      sorting...\n";
    sort(sa, sa + length, SaComp(gs));


    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cerr << "   SA builded in " << cpu_time << " sec\n";
}


/**
 * Compute the length of the lcp between suffixes
 *  at posisition i and j in the SA
 */
size_t SuffixArray::compute_lcp(size_t i, size_t j) const
{
    size_t k = 0;

    while ( sa[i] + k < length
                && sa[j] + k < length
                && gs[sa[i] + k] == gs[sa[j] + k] )
    {
        k++;
    }

    return k;
}


/**
 * Building the LCP array []
 */
void SuffixArray::buildLcp()
{
    clock_t start = clock();
    double cpu_time;
    lcp[0] = 0;

    for (size_t i = 1; i < length; i++)
    {
        lcp[i] = compute_lcp(i-1, i);
    }

    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cerr << "   LCP array builded in " << cpu_time << " sec\n";
}


/**
 * Return the lcp between postions i and i-1 in SA
 */
size_t SuffixArray::get_lcp(size_t s_num) const
{
    if (s_num > 0 && s_num < length)
    {
        return lcp[s_num];
    }

    cerr << "ERROR: LCP array position was " << s_num
         << ". Should be [1.." << length-1 << "]" << endl;

    assert(s_num <= 0 && s_num >= length);
    return MAX;
}


/**
 * Comparison between string 'str' and the suffix at position 's_num'
 *
 *  if str > suffix: return  1
 *  if str < suffix: return -1
 *  else: return 0
 */
short int SuffixArray::strCmp(const string& str, size_t s_num) const
{
    size_t k = 0;

    while (k < str.size() && sa[s_num]+k < length && gs[sa[s_num]+k] == str[k])
    {
        k++;
    }

    if ( (k < str.size() && sa[s_num]+k >= length) ||
         (k < str.size() && sa[s_num]+k < length &&
          str[k] > gs[sa[s_num]+k]) )
    {
        return 1;
    }
    else if ( k < str.size() &&
              sa[s_num]+k < length &&
              str[k] < gs[sa[s_num]+k] )
    {
        return -1;
    }

    return 0;
}


/**
 * Perform a binary search in the suffix array:
 *   return the index of the suffix where str was found
 */
size_t SuffixArray::find_match(const string& str) const
{
    if ( str == "" )
    {
        cerr << "WARN: search of an empty string!\n";
        return MAX;
    }

    size_t Left(0), Right(length - 1);

    while ( Left != Right )
    {
        size_t Middle = (Left + Right) / 2;

        if ( strCmp(str, Middle) > 0 )
        {
            Left = Middle + 1;
        }
        else
        {
            Right = Middle;
        }
    }

    if (strCmp(str, Left) == 0)
    {
        return Left;
    }

    return MAX;
}
