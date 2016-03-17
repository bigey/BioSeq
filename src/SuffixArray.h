#ifndef SUFFIXARRAY_H_
#define SUFFIXARRAY_H_

#include <iostream>
#include <algorithm>
#include "GenericSeq.h"

class SuffixArray {

private:
    size_t length;
    GenericSeq gs;
    size_t* sa;
    size_t* lcp;
    
public:
    SuffixArray();
    SuffixArray(GenericSeq&);
    SuffixArray(SuffixArray const&);
    SuffixArray& operator=(SuffixArray const&);
    virtual ~SuffixArray();
    size_t operator[] (size_t) const;
    void buildSa();
    void buildLcp();
    size_t get_length() const;
    size_t get_lcp(size_t) const;
};

struct SaComp;

#endif /* SUFFIXARRAY_H_ */
