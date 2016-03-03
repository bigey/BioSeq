#include "GenericSeq.h"

using namespace std;

template <typename T>
string NumberToString(T num)
{
 ostringstream oOStrStream;
 oOStrStream << num;
 return oOStrStream.str();
}

string error()
{
	return "WARN: ";
}

bool is_valid(char n)
{
    return (n == 'A') || (n == 'a') ||
    	   (n == 'C') || (n == 'c') ||
		   (n == 'G') || (n == 'g') ||
		   (n == 'T') || (n == 't');
}

/**
 * Retourne la taille à allouer au tableau de données
 *
 */
size_t alloc_size(size_t l)
{
	return l/4 + (l%4 != 0);
}

/**
 * Retourne l'indice de l'octet dans lequel stocker 4 mots
 */
size_t get_table_index(size_t n)
{
    return (n / 4);
}

/**
 * Retourne l'indice du bit dans lequel stocker un mot
 */
short get_bit_index(size_t n)
{
    return (3 -(n % 4)) * 2;
}

/**
 * Encoding method
 */
char encoding(char c)
{
	switch (c) {
		case('a'): return 0b00000000;
		case('A'): return 0b00000000;
		case('c'): return 0b00000001;
		case('C'): return 0b00000001;
		case('g'): return 0b00000010;
		case('G'): return 0b00000010;
		case('t'): return 0b00000011;
		case('T'): return 0b00000011;
		default: break;
	}
	return 0;
}

/*
 * Decoding method
 */
char decoding(char c)
{
	switch (c) {
		case(0b00000000): return 'A';
		case(0b00000001): return 'C';
		case(0b00000010): return 'G';
		case(0b00000011): return 'T';
		default: break;
	}
	return 'A';
}

/*
 * Default constructor
 */
GenericSeq::GenericSeq(): id(""), description(""), length(0), tab(0) {
}

/*
 * Complete constructor
 */
GenericSeq::GenericSeq(string seq_id, string desc, unsigned char* seq): id(seq_id), description(desc), length(0), tab(0)
{
	if (seq != NULL)
	{
		/* Sequence's length determination */
		while ( seq[length] != '\0' )
		{
			if (! is_valid(seq[length]) ) {
				cerr << error()
					 << "invalid character '"
					 << seq[length]
					 << "' found at position "
					 << length+1
					 << ". Will be converted to 'A'"
					 << endl;
			}
			length++;
		}
	}

	/* Dynamic table allocation */
	size_t size = alloc_size(length);
	tab = new unsigned char[size];

	/* Sequence encoding */
	for(size_t i = 0; i < length; i++) {
		tab[get_table_index(i)] |= encoding(seq[i]) << get_bit_index(i);
	}
}

/*
 * Copy constructor
 */
GenericSeq::GenericSeq(GenericSeq const& gs): id(gs.id), description(gs.description), length(gs.length), tab(0)
{
	size_t size = alloc_size(length);
	tab = new unsigned char[size];
	for(size_t i = 0; i < size; i++) {
		tab[i] = gs.tab[i];
	}
}

/*
 * Affectation operator
 */
GenericSeq& GenericSeq::operator=(GenericSeq const& gs)
{
	if(this != &gs)
	{
		id = gs.id;
		description = gs.description;
		length = gs.length;

		size_t size = alloc_size(length);
		tab = new unsigned char[size];

		for(size_t i = 0; i < size; i++) {
			tab[i] = gs.tab[i];
		}
	}
	return *this;
}

/*
 * Default destructor
 */
GenericSeq::~GenericSeq()
{
	//tab = 0;
	delete[] tab;
}

void GenericSeq::set_id(string s)
{
	id = s;
}

string GenericSeq::get_id() const {
	return id;
}

void GenericSeq::set_description(string s)
{
	description = s;
}

string GenericSeq::get_description() const
{
	return description;
}

size_t GenericSeq::get_length() const
{
	return length;
}

string GenericSeq::get_defline() const
{
	string s = get_id() + "|" + get_description()
			          + ", length:" + NumberToString(get_length());
	return s;
}

string GenericSeq::get_seq() const
{
	unsigned char octet(0), tmp(0);
	string sequence("");

	for(size_t i = 0; i < length; i++)
	{
		if ( (i % 4) == 0 ) {
			octet = tab[get_table_index(i)];	// Get a new byte
		}
		tmp = octet & 0b11000000;				// Apply a 6 bits mask on the less significant bits
		tmp = tmp >> 6;							// shift of 6 bits to right align 'tmp'
		sequence = sequence + decoding(tmp);	// Decoding of resulting byte 'tmp'
		octet = octet << 2;						// left shift 'octet' (2 bits) to process remaining
	}
	return sequence;
}


void GenericSeq::print() const
{
	cout << ">" << get_id();

	if(get_description() != "")
		cout << " " << get_description();

	cout << " (length:" << get_length() << ")" << endl;

	string seq = get_seq();
	size_t step = 80;		// step is the nb of chars per line

	for(size_t i = 0; i < length; i += step)
		cout << seq.substr(i, step) << endl;
}

char GenericSeq::get_symbol_at(size_t pos) const {
	if (pos > length) {
		cerr << error() << "position " << pos
			 << " is greater than sequence's length "
			 << length << endl;
		return 'N';

	} else {
		size_t byte = get_table_index(pos-1);
		short shift = get_bit_index(pos-1);
		return decoding(tab[byte] >> shift & 0b00000011);
	}
}

char GenericSeq::operator[](size_t i) const
{
	return get_symbol_at(i);
}

/*
void Seq::set_symbol_at(size_t position, char symbol);
Seq Seq::get_sub_seq(size_t start, size_t end) const;
*/

GenericSeq GenericSeq::revcom() const
{
	GenericSeq newSeq;
	newSeq.id = id;
	newSeq.description = description + " (reverse-complement)";
	newSeq.length = length;

	/* Dyn. allocation of table char tab[] */
	newSeq.tab = new unsigned char[alloc_size(length)];

	/* Reversing sequence loop */
	for(size_t i = 0; i < length; i++)
	{
		size_t index = get_table_index(i);
		size_t new_index = get_table_index(length -i -1);

		short right_shift = get_bit_index(i);
		short left_shift = get_bit_index(length -i -1);
		unsigned char tmp = 0;

		/* Using '~' for complementing 'tab' */
		tmp = ~tab[index] >> right_shift;
		tmp &= 0b00000011;
		tmp = tmp << left_shift;
		newSeq.tab[new_index] |= tmp;
	}
	return newSeq;
}

ostream &operator<<(ostream &os, const GenericSeq &gs)
{
    for (size_t i = 1; i <= gs.get_length() ; i++) {
        os << gs.get_symbol_at(i);
    }
    return os;
}
