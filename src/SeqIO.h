#ifndef SEQIO_H_
#define SEQIO_H_

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "GenericSeq.h"

class SeqIO {

private:
	const std::string file;
	const char direction;
	std::fstream fd;

public:
	SeqIO(std::string, const char);
	virtual ~SeqIO();
	GenericSeq& operator=(GenericSeq &gs);
	GenericSeq next();
};

std::istream& operator>>(std::istream&, GenericSeq&);


#endif /* SEQIO_H_ */
