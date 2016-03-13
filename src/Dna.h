#ifndef DNA_H_
#define DNA_H_

#include <string>
#include <iostream>
#include "GenericSeq.h"

class Dna: public GenericSeq
{

private:
	std::string type;

public:
	Dna();
	Dna(std::string, std::string, const std::string&);
	virtual ~Dna();
	std::string get_type() const;
	void print() const;
	Dna* revcomp() const;
};


#endif /* DNA_H_ */
