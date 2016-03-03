#include "Dna.h"

using namespace std;

Dna::Dna(): GenericSeq(), type("dna") {}
Dna::Dna(string id, string desc, unsigned char* tab): GenericSeq(id, desc, tab), type("dna") {}

Dna::~Dna()
{
	tab = 0;
	delete[] tab;
}

string Dna::get_type() const
{
	return type;
}

void Dna::print() const
{
	cout << ">" << get_defline() << ", type:" << get_type()	<< endl;
	cout << get_seq() << endl;
}
