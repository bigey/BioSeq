#ifndef SEQIO_H_
#define SEQIO_H_

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <regex.h>
#include "GenericSeq.h"

#define MAX_MATCHES 1 // maximum number of matches

class SeqIO {

private:
    const std::string file;
    std::string line, seq, qual, id, desc;
    const char direction;
    std::fstream fd;
    char c;
    size_t counter, countLine, numOfSequence;
    size_t pos;
    
    /* States of the automata */
    enum state_t { start, fastaSeq, fastqSeq, fastqQual, stop } state;
    
    /* Regex declaration */
    regex_t notDna, notQual;
    regmatch_t matches[MAX_MATCHES];
    
    /* Private function returning a GenericSeq object */
    //GenericSeq gs(std::string, std::string, std::string&);
    
public:
    SeqIO(std::string, const char);
    virtual ~SeqIO();
    GenericSeq& operator=(GenericSeq &gs);
    GenericSeq next();
    bool is_eof() const;
};


#endif /* SEQIO_H_ */
