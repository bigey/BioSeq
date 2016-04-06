#ifndef FINDMATCH_H_
#define FINDMATCH_H_

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include "GenericSeq.h"
#include "SuffixArray.h"

#define NONE 0xFFFFFFFFFFFFFFFF

/* User defined types */
struct match_t
{
        std::vector<size_t> pos_f;             // position on strand (+)
        std::vector<size_t> pos_r;             // position on strand (-)
};


/* Class definition */
class FindMatch {

private:
    std::vector<match_t> match;       // kmer for and rev matching pos.
    std::vector<size_t> k_pos_f, k_pos_r;  // best matching positions
    size_t Lk;                        // kmer length
    GenericSeq R;                     // read object (GenericSeq)
    size_t Lr;                        // read length
    SuffixArray SA;                   // suffix array object (SuffixArray)
    size_t Lg;                        // reference length
    std::string Rname;                // read id (name)
    std::string Gname;                // reference id (name)
    size_t match_begin;               // match begin position
    size_t match_end;                 // match end position
    short int strand;                 // read strand {-1,+1}
    std::string cigar;                // variation string
    size_t nb_of_kmer;                // nb of possible k-mer on the read

    /* private methods */
    void kmer_search(size_t, size_t, std::vector<size_t>&) const;
    void make_kmer();

public:
    FindMatch();
    FindMatch(const GenericSeq&, const SuffixArray&, const size_t);
    virtual ~FindMatch();
    size_t locate_read(std::vector< std::vector<size_t> >&,
        std::vector<size_t>&);
    void output() const;
    size_t best_path();
    void analyze();

};


#endif /* FINDMATCH_H_ */
