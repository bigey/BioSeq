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
    bool compare(const std::string&, size_t, size_t) const;
    short int strCmp(const std::string&, size_t) const;
    size_t compute_lcp(const std::string&, size_t) const;
    size_t binary_search(const std::string&, size_t, size_t) const;
    size_t find_match(const std::string&) const;
    size_t FindMatch(const std::string&) const;
};

struct SaComp;

#endif /* SUFFIXARRAY_H_ */
