#ifndef FINDMATCH_H_
#define FINDMATCH_H_

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include "GenericSeq.h"
#include "SuffixArray.h"

#define MAX 0xFFFFFFFFFFFFFFFF

/* User defined types */
struct match_t
{
        std::vector<size_t> pos_f;             // position on strand +
        std::vector<size_t> pos_r;             // position on strand -
};


/* Class definition */
class FindMatch {

private:
    std::vector<match_t> match;       // kmer forward and reverse matches
    std::vector<size_t>  best_m;      // kmer best matches
    size_t Lk;                        // kmer length
    GenericSeq R;                     // read object
    size_t Lr;                        // read length
    SuffixArray SA;                   // suffix array object of reference
    size_t Lg;                        // reference length

public:
    FindMatch();
    FindMatch(const GenericSeq&, const SuffixArray&, const size_t);
    virtual ~FindMatch();
    void get_all_match(size_t, size_t, std::vector<size_t>&) const;
    void find_matches();
    void best_path();
    void analyze() const;

};


#endif /* FINDMATCH_H_ */
