#include "Dna.h"

using namespace std;

Dna::Dna(): GenericSeq(), type("dna") {}
Dna::Dna(string id, string desc, const string& seq): GenericSeq(id, desc, seq), type("dna") {}

Dna::~Dna()
{
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
