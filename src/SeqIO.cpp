#include "SeqIO.h"

using namespace std;

SeqIO::SeqIO(string f, const char d): file(f), direction(d), fd(0), counter(0), countLine(0), numOfSequence(0), state(start)
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
    
    /* Compilation of regex */
    regcomp(&notDna, "[^acgtACGTnN ]", REG_EXTENDED);
    regcomp(&notQual, "[^!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ ]", REG_EXTENDED);
}

SeqIO::~SeqIO()
{
    regfree(&notDna);
    regfree(&notQual);
    fd.close();
}

GenericSeq SeqIO::next()
{    
    line = seq = qual = "";
    
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
            desc = line.substr(pos+1, string::npos);
            numOfSequence++;
            state = fastaSeq;
            
        } else if ( state == start && c == '@' ) {
            
            pos = line.find(" ");
            id  = line.substr(1, pos-1);
            numOfSequence++;
            state = fastqSeq;
            
        } else if ( state == fastaSeq && c == '>' ) {
            
            //cout << id << endl;
            //cout << "sq[" << seq << "]" << endl;
            
            GenericSeq gs(id, desc, seq);
            
            /* New sequence */
            pos  = line.find(" ");
            id   = line.substr(1, pos-1);
            
            if ( pos != 0 ) {
                desc = line.substr(pos+1, string::npos);
            } else {
                desc = "";
            }
            
            /* Return last sequence */
            numOfSequence++;
            //cout << seq << endl;
            return gs;
            
        } else if ( state == fastqSeq && c == '+' ) {
            
            state = fastqQual;
            
        } else if ( state == fastqQual && c == '@' && countLine == 0 ) {
            
            if( countLine != 0 ) {
                cerr << "ERROR: not same number of lines for sequence and quality at line " << counter;
                cerr << line << endl;
                throw 3;
            }
            
            /* Return last sequence */
            //cout << id << endl;
            //cout << "sq[" << seq  << "]" << endl;
            //cout << "ql[" << qual << "]" << endl;
            
            GenericSeq gs(id, desc, seq);
            
            /* New read */
            pos = line.find(" ");
            id  = line.substr(1, pos-1);
            numOfSequence++;
            countLine = 0;
            state = fastqSeq;
            return gs;
            
        } else if ( state == fastaSeq ) {
            
            if( regexec(&notDna, line.c_str(), MAX_MATCHES, matches, 0) == 0 ) {
                cerr << "ERROR: illegal sequence char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 1;
                
            } else {
                seq += line;
            }
            
        } else if ( state == fastqSeq ) {
            
            if( regexec(&notDna, line.c_str(), MAX_MATCHES, matches, 0) == 0 ) {
                cerr << "ERROR: illegal sequence char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 1;
                
            } else {
                seq += line;
                countLine++;
            }
            
        } else if ( state == fastqQual ) {
            
            if( regexec(&notQual, line.c_str(), MAX_MATCHES, matches, 0) == 0 ) {
                cerr << "ERROR: illegal quality char found at line " << counter;
                cerr << ", position " << matches[0].rm_so + 1 << endl;
                cerr << line << endl;
                throw 2;
                
            } else {
                countLine--;
                qual += line;
            }
            
        } else {
            /* Malformed file */
            // TODO: à faire...
        }
    }
    
    /* Process last file sequence */
    if( state == fastaSeq ) {
        
        //cout << id << endl;
        //cout << "sq[" << seq << "]" << endl;
        //cout << seq << endl;
        
        state = stop;
        desc = seq == "" ? desc : "";
        GenericSeq gs(id, desc, seq);
        return gs;
        
    } else if ( state == fastqQual ) {
        
        //cout << id << endl;
        //cout << "sq[" << seq  << "]" << endl;
        //cout << "ql[" << qual << "]" << endl;
        
        state = stop;
        GenericSeq gs(id, desc, seq);
        return gs;
    }
    
    return GenericSeq("", "", "");
}

bool SeqIO::is_eof() const
{
    return fd.eof();
}
