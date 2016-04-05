#include "SeqIO_b.h"

using namespace std;


SeqIO::SeqIO(const string& f, const char d): file(f), direction(d), fd(0), begin(), identifier(), pointer(0)
{
    if( direction == 'r' ) {

        /* Opening for reading */
        fd.open(file.c_str(), ios::in);

    } else if ( direction == 'w') {

        /* Opening for writing...
        *  fd.open(file.c_str(), ios::out);
        *  Todo...
        */
    }

    if ( ! fd.is_open() ) {
        std::cerr << "" << std::endl;
        cerr << "ERROR: couldn't open file: " << file << endl;
        throw 1;
    }

    /* Compilation of regex */
    regcomp(&notDna, "[^acgtACGTnN]", REG_EXTENDED);
    regcomp(&notQual, "[^!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ]", REG_EXTENDED);
}


SeqIO::~SeqIO()
{
    regfree(&notDna);
    regfree(&notQual);
    fd.close();
}


void SeqIO::index()
{
    char c = 0;
    string line, id;
    state = start;
    fd.seekg(0, fd.beg);
    size_t pos, countLine = 0, counter;
    counter = 0;


    /* Parsing input file */
    while ( getline(fd, line) )
    {
        if ( line != "" )
            c = line.at(0);

        counter++;

        if( state == start && c == '>' ) {

            /* New sequence */
            pos  = line.find(" ");
            id   = line.substr(1, pos-1);
            state = fastaSeq;

            begin.push_back( fd.tellg() );
            identifier.push_back( id );

            // cerr << id << endl;

        } else if ( state == start && c == '@' ) {

            /* New read */
            pos = line.find(" ");
            id  = line.substr(1, pos-1);
            state = fastqSeq;

            begin.push_back( fd.tellg() );
            identifier.push_back( id );

            // cerr << id << endl;

        } else if ( state == fastaSeq && c == '>' ) {

            // cerr << id << endl;
            // cerr << "sq[" << seq << "]" << endl;

            /* New sequence */
            pos  = line.find(" ");
            id   = line.substr(1, pos-1);

            begin.push_back( fd.tellg() );
            identifier.push_back( id );

            // cerr << id << endl;

        } else if ( state == fastqSeq && c == '+' ) {

            /* New quality string */
            state = fastqQual;

        } else if ( state == fastqQual && c == '@' && countLine == 0 ) {

            if( countLine != 0 )
            {
                cerr << "ERROR: not same number of lines for sequence and quality at line " << counter;
                cerr << line << endl;
                throw 3;
            }

            /* New read */
            pos = line.find(" ");
            id  = line.substr(1, pos-1);
            countLine = 0;
            state = fastqSeq;

            begin.push_back( fd.tellg() );
            identifier.push_back( id );

            // cerr << id << endl;

        } else if ( state == fastaSeq ) {

            if ( regexec(&notDna, line.c_str(), MAX_MATCHES, matches, 0) == 0 )
            {
                cerr << "ERROR: illegal sequence char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 1;
            }

        } else if ( state == fastqSeq ) {

            if ( regexec(&notDna, line.c_str(), MAX_MATCHES, matches, 0) == 0 )
            {
                cerr << "ERROR: illegal sequence char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 1;
            }
            else
                countLine++;

        } else if ( state == fastqQual ) {

            if ( regexec(&notQual, line.c_str(), MAX_MATCHES, matches, 0) == 0 )
            {
                cerr << "ERROR: illegal quality char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 2;
            }
            else
                countLine--;

        } else {
            /* Malformed file */
            cerr << "ERROR: file malformed at line " << counter << endl;
            throw;
        }
    }

    fd.clear();
    fd.seekg(0, ios::beg);
    // return;
}


bool SeqIO::read(GenericSeq& gs)
{
    /* Get the last sequence ? */
    if ( pointer >= begin.size() )
        return false;

    /* Is file open ? */
    if ( ! fd.is_open() )
    {
        cerr << "ERROR: unable to read the file!" << endl;
        throw;
        return false;
    }

    size_t length = 0;
    streampos start, end, eof;

    fd.seekg(0, ios::end);
    eof = fd.tellg();

    start = begin[pointer];
    fd.seekg(start, ios::beg);

    /* Pointer to the end of the sequence */
    if ( pointer+1 < begin.size() )
        end = begin[pointer+1];
    else
        end = eof;

    /* Compute sequence length */
    fd.seekg(start, ios::beg);
    string line;

    while (!fd.eof() && getline(fd, line) && line[0] != '>'  && line[0] != '@' && line[0] != '+')
    {
        length += line.length();
    }


    /* Reading the sequence */
    fd.clear();
    fd.seekg(start, ios::beg);

    if (length)
    {
        gs = GenericSeq (length);
        gs.set_id(identifier[pointer]);
        size_t i = 0;

        while (!fd.eof() && getline(fd, line) && line[0] != '>' && line[0] != '+')
        {
            for (size_t x = 0; x < line.length(); x++) {
                gs.set_symbol_at(i++, line[x]);
            }
        }
    }


    /* Point to the next sequence */
    pointer++;
    return true;
}


bool SeqIO::is_eof() const
{
    return fd.eof();
}
