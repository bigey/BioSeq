#ifndef SUFFIXARRAY_H_
#define SUFFIXARRAY_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>
#include "GenericSeq.h"


class SuffixArray {

private:
    size_t length;
    const GenericSeq gs;
    size_t* sa;
    size_t* lcp;
    
public:
    SuffixArray();
    SuffixArray(const GenericSeq&);
    SuffixArray(SuffixArray const&);
    SuffixArray& operator=(SuffixArray const&);
    virtual ~SuffixArray();
    size_t operator[] (size_t) const;
    void buildSa();
    void buildLcp();
    size_t get_lcp(size_t) const;
    size_t get_length() const;
    short int strCmp(const std::string&, size_t) const;
    size_t compute_lcp(size_t, size_t) const;
    size_t find_match(const std::string&) const;
    std::vector<size_t> get_all_positions(size_t, size_t) const;
};

struct SaComp;

#endif /* SUFFIXARRAY_H_ */
