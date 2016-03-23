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
        //size_t c;
        
        while ( (a < length) && (b < length) && (gs[a] == gs[b]) )
        //while ( (c < length) && (gs[a] == gs[b]) )
        {
            a++; b++;
            //c = a > b ? a : b;
            //cerr << a << " " << b << endl;
        }
        
        //cerr << "gs[" << a << "]:" << gs[a] << " == gs[" << b << "]:" << gs[b] << endl;
        return (a >= length) || ((b < length) && (gs[a] < gs[b]));
    }
};


/**
 * Default constructor
 */
SuffixArray::SuffixArray() {}


/**
 * Constructor using an GenericSeq object 
 */
SuffixArray::SuffixArray(const GenericSeq& gseq): length(gseq.get_length()), gs(gseq), sa(NULL), lcp(NULL)
{
    // Memory allocation for SA and LCP array
    if (length)
    {
        sa  = new size_t[length];
        lcp = new size_t[length];
    }
    
    // SA construction
    cerr << "   builing SA...\n";
    buildSa();
    
    // LCP construction
    cerr << "   building LCP...\n";
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

        cerr << "ERROR: position " << i
             << " is greater than suffix array length ("
             << length << ")" << endl;
             
        assert (i < length);
        return 0;
}


/**
 * Building suffix array: sa[]
 */
void SuffixArray::buildSa()
{
    if ( length == 0 )
        cerr << "ERROR: indexing a sequence of length 0!\n";
    
    assert(length != 0);
    
    clock_t start = clock();
    double cpu_time_used;
    
    //cerr << "      filling SA...\n";
    for(size_t i = 0; i < length; i++)
        sa[i] = i;

    //cerr << "      sorting...\n";
    sort(sa, sa + length, SaComp(gs));
    
    cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cout << "   SA builded in " << cpu_time_used << " sec\n";
}


/**
 * Building LCP array: lcp[]
 */
void SuffixArray::buildLcp()
{
    clock_t start = clock();
    double cpu_time_used;
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
    
    cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cout << "   LCP array builded in " << cpu_time_used << " sec\n";
}


/**
 * Return the value stored in the LCP array at postion 's_num'
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
    return 0xFFFFFFFFFFFFFFFF;
}


/**
 * Comparison between string 'str' and the suffix 'sa[s_num]' from position 'p'
 */
// bool SuffixArray::compare(const string& str, size_t s_num, size_t p) const
// {
//     size_t k = 0;
//     
//     while ( p+k < str.size() 
//         && sa[s_num] + p+k < length 
//         && gs[sa[s_num] + p+k] == str[p+k] )
//     {
//         k++;
//     }
//     
//     
//     /* 
//      * Return true if:
//      * 
//      *   (1) we reach the end of the indexed sequence,
//      * 
//      *   OR
//      * 
//      *   (2) the symbol at position 'p+k 'of 'str' is greater than 
//      *          the symbol in the suffix
//      */ 
//     return (p+k < str.size() && sa[s_num] + p+k >= length) ||
//            (p+k < str.size() && sa[s_num] + p+k < length &&
//               str[p+k] > gs[sa[s_num] + p+k]);
// }

/**
 * 
 * Comparison between string 'str' and the suffix 'sa[s_num]'
 * 
 *  if str > suffix: return  1
 *  if str < suffix: return -1
 *  else: return 0
 * 
 */
short int SuffixArray::strCmp(const string& str, size_t s_num) const
{
    size_t k = 0;
    
    while (k < str.size() && sa[s_num]+k < length && gs[sa[s_num]+k] == str[k])
    {
        k++;
    }
    
    if (  (k < str.size() && sa[s_num]+k >= length) ||
          (k < str.size() && sa[s_num]+k < length &&
              str[k] > gs[sa[s_num]+k])
       )
    {
        return 1;
    }
    else if (   k < str.size() && 
                sa[s_num]+k < length &&
                str[k] < gs[sa[s_num]+k]
            )
    {
        return -1;
    }
    
    return 0;
}


/**
 * Compute the length of the longest common prefix between a string 'str'
 *   and the suffix 'sa[s_num]'
 */
// size_t SuffixArray::compute_lcp(const string& str, size_t s_num) const
// {
//     size_t k = 0;
//     
//     while ( k < str.size() 
//         && sa[s_num] + k < length 
//         && gs[sa[s_num] + k] == str[k] )
//     {
//         k++;
//     }
//     
//     return k;
// }


/**
 * Binary search of the string 'str' in the prefix of the suffix array
 */
// size_t SuffixArray::binary_search(const string& str, size_t low, size_t high) const
// {
//     if (high == low)
//     {
//         if ( compute_lcp(str, high) == str.size() )
//         {
//             return high;
//         }
//         else
//         {
//             return 0xFFFFFFFFFFFFFFFF;
//         }
//     }
//     
//     size_t middle = low + (high - low) / 2;
//     size_t k = compute_lcp(str, middle);
//        
//     if ( k == str.size() )  // str a été retrouvé
//     {   
//         return middle;
//     }
//     else if ( k == 0 )      // aucun char commun
//     {
//         if ( str[0] > gs[sa[middle]] )
//         {
//             //cout << "k=" << k <<"|"<< low <<" "<<middle <<" "<< high << " l=m\n";
//             low = middle+1;
//         }
//         else
//         {
//             //cout << low <<" "<<middle <<" "<< high << " h=m\n";
//             high = middle;
//         }
//     }
//     else if ( compare(str, middle, k) )       // str > sa[middle]
//     {   
//         //cout << "k=" << k <<"|"<<low <<" "<<middle <<" "<< high << " l=m\n";
//         low = middle+1;
//     }
//     else                                 // str < sa[middle+1]
//     {
//         //cout << "k=" << k <<"|"<<low <<" "<<middle <<" "<< high << " h=m\n";
//         high = middle;
//     }
//     
//     //cout << "low:" << low << " high:" << high << endl;
//     return binary_search(str, low, high);
// }


size_t SuffixArray::find_match(const string& str) const
{
    if ( str == "" )
    {
        cerr << "WARN: search of an empty string!\n";
        return 0xFFFFFFFFFFFFFFFF;
    }
    
    size_t Left(0), Right(length-1);
    
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
        return sa[Left];
    }
    
    return 0xFFFFFFFFFFFFFFFF;
}


/**
 * Find a string 'str' in the indexed sequence using suffix array 'sa[]'
 */
// size_t SuffixArray::find_match(const string& str) const
// {
//     if ( str == "" || length == 0)
//     {
//         return 0xFFFFFFFFFFFFFFFF;
//     }
//     
//     size_t position = binary_search(str, 0, length-1);
//     
//     if (position == 0xFFFFFFFFFFFFFFFF)
//     {
//         return position;
//     }
//     return sa[position];
// }


// size_t SuffixArray::FindMatch(const string& str) const
// {
//     if ( str == "" )
//     {
//         cerr << "WARN: search of an empty string!\n";
//         return 0xFFFFFFFFFFFFFFFF;
//     }
//     
//     size_t index = binary_search(str, 0, length-1);
//     
//     if (index == 0xFFFFFFFFFFFFFFFF)
//     {
//         return index;
//     }
//     
//     return sa[index];
// }

