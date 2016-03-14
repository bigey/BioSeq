#include "GenericSeq.h"

using namespace std;


template <typename T>
string NumberToString(T num)
{
    ostringstream oOStrStream;
    oOStrStream << num;
    return oOStrStream.str();
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
size_t alloc_size(size_t n)
{
    return n / 4 + (n % 4 != 0);
}


/**
 * Retourne l'indice de l'octet dans le tableau
 */
size_t get_table_index(size_t n)
{
    return (n / 4);
}


/**
 * Retourne l'indice du bit dans lequel stocker le character
 */
size_t get_bit_index(size_t n)
{
    return (3 -(n % 4)) * 2;
}


/**
 * Encoding method
 */
unsigned char encoding(unsigned char c)
{
    switch (c) {
        case('a'): return '\00';  // 0b00000000
        case('A'): return '\00';  // 0b00000000
        case('c'): return '\01';  // 0b00000001
        case('C'): return '\01';  // 0b00000001
        case('g'): return '\02';  // 0b00000010
        case('G'): return '\02';  // 0b00000010
        case('t'): return '\03';  // 0b00000011
        case('T'): return '\03';  // 0b00000011
    }
    
    return '\00';
}


/**
 * Decoding method
 */
char decoding(unsigned char c)
    {
    switch (c) {
        case('\00'): return 'A';
        case('\01'): return 'C';
        case('\02'): return 'G';
        case('\03'): return 'T';
        default: break;
    }
    return 'A';
}


/**
 * Default constructor
 */
GenericSeq::GenericSeq(): id(""), description(""), length(0), tab(NULL) {}


/**
 * Complete constructor
 */
GenericSeq::GenericSeq(string seq_id, string desc, const string& seq): id(seq_id), description(desc), length(seq.size()), tab(NULL)
{
    /* Dynamic table allocation */
    size_t size = alloc_size(length);
    tab = new unsigned char[size];

    /* Sequence encoding loop */
    for(size_t i = 0; i < length; i++)
    {
        if ( ! is_valid(seq[i]) )
            cerr << "WARN: invalid character '" << seq[i]
                 << "' found at position " << i+1
                 << ". Will be converted to 'A'" << endl;
        
        if ( i % 4 == 0)
            tab[get_table_index(i)] = 0;
             
        tab[get_table_index(i)] |= encoding(seq[i]) << get_bit_index(i);
    }
}


/**
 * Copy constructor
 */
GenericSeq::GenericSeq(GenericSeq const& gs): id(gs.id), description(gs.description), length(gs.length), tab(NULL)
{
    if( length )
    {
        size_t size = alloc_size(length);
        tab = new unsigned char[size];
    
        for(size_t i = 0; i < size; i++)
            tab[i] = gs.tab[i];
    }
}


/**
 * Affectation operator (=)
 */
GenericSeq& GenericSeq::operator=(GenericSeq const& gs)
{
    if(this != &gs)
    {
        size_t size = alloc_size(gs.length);
        
        if (length != gs.length)
        {
            if ( length )
            {
                delete[] tab;
                tab = NULL;
            }
            length = gs.length;
            
            if ( length )
                tab = new unsigned char[size];
        }
        
        if ( length )
        {
            for(size_t i = 0; i < size; i++)
                tab[i] = gs.tab[i];
        }
        
        id = gs.id;
        description = gs.description;
    }
    return *this;
}


/**
 * Default destructor
 */
GenericSeq::~GenericSeq()
{
    if( length )
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
    unsigned char octet, tmp;
    string sequence("");
    
    for(size_t i = 0; i < length; i++)
    {
        if ( (i % 4) == 0 )
            octet = tab[get_table_index(i)]; // get a new byte
        
        tmp = octet & ((1 << 7) | (1 << 6)); // apply 0b11000000 mask
        tmp = tmp >> 6;                      // right shift the result
        sequence += decoding(tmp);           // decode and concat
        octet = octet << 2;                  // left shift 'octet' and continue
    }
    
    return sequence;
}


void GenericSeq::print() const
{
    /* Set nb of chars per line */
    size_t step = 80;
    
    cout << ">" << get_id();
    
    if(get_description() != "")
        cout << " " << get_description();
    
    cout << " (length:" << get_length() << ")" << endl;

    string seq = get_seq();
    for(size_t i = 0; i < length; i += step)
        cout << seq.substr(i, step) << endl;
}


char GenericSeq::get_symbol_at(size_t pos) const
{
    if (pos > length) {
        cerr << "WARN: position " << pos
             << " is greater than sequence's length "
             << length << endl;
        return 'N';

    } else {
        size_t byte = get_table_index(pos-1);
        short shift = get_bit_index(pos-1);
        return decoding(tab[byte] >> shift & '\03'); // & 0b00000011
    }
}


char GenericSeq::operator[](size_t i) const
{
    return get_symbol_at(i);
}


GenericSeq GenericSeq::revcom() const
{
    GenericSeq newSeq;
    newSeq.id = id;
    newSeq.description = description + " (rev-comp)";
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
        tmp &= '\03';   // & 0b00000011
        tmp = tmp << left_shift;
        newSeq.tab[new_index] |= tmp;
    }
    return newSeq;
}


/**
 * Read sequence from a file stream
 */
// void GenericSeq::read(ifstream &is)
// {
//     if ( ! is.is_open() ) {
//         cerr << "unable to read from stream!" << endl;
//         return;
//     }
//     
//     if (length) {
//         length = 0;
//         delete[] tab;
//         tab = NULL;
//     }
//     
//     streampos p = is.tellg();
//     char c;
//     
//     while ( ! is.eof() && is.get(c) && is_valid(c) ) {
//         length += !isspace(c);
//     }
//     
//     cerr << " reading " << length << " characters...\n";
//     
//     if (length)
//     {
//         tab = new unsigned char[alloc_size(length)];
//         is.seekg(p);
//         // size_t  i = 0;
//         
//         while ( !is.eof() && is_valid(c = is.get()) ) {
//             /* TODO
//              * encoder le char et le stocker dans tab
//              */
//              
//         }
//     }
//     
//     return;
// }


/**
 * Write sequence to STDOUT
 */
void GenericSeq::write(ostream &os) const
{
    for (size_t i = 1; i <= length; i++) {
        os << get_symbol_at(i);
    }
}


/**
 * 
 * Non member methods
 * 
 */


/**
 * Extraction operator (>>) extracts as many characters as possible from the stream
 */
ostream& operator<<(ostream &os, const GenericSeq& gs)
{
    gs.write(os);
    return os;
}


/**
 * 
 */
// ifstream& operator>>(ifstream& ifs, GenericSeq& gs)
// {
//     gs.read(ifs);
//     return ifs;
// }


/**
 * TODO:
 *   void Seq::set_symbol_at(size_t position, char symbol);
 *   Seq Seq::get_sub_seq(size_t start, size_t end) const;
 */

