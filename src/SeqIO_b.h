#ifndef SEQIO_H_
#define SEQIO_H_

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <regex.h>
#include "GenericSeq.h"

#define MAX_MATCHES 1 // maximum number of matches

class SeqIO {

private:
    const std::string file;
    const char direction;
    std::fstream fd;
    std::vector<std::streampos> begin;
    std::vector<std::string> identifier;
    size_t pointer;

    /* States of the automata */
    enum state_t { start, fastaSeq, fastqSeq, fastqQual, stop } state;

    /* Regex declaration */
    regex_t notDna, notQual;
    regmatch_t matches[MAX_MATCHES];

public:
    SeqIO(const std::string &, const char);
    virtual ~SeqIO();
    void index();
    bool read(GenericSeq&);
    bool is_eof() const;
};


#endif /* SEQIO_H_ */
