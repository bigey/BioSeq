#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <regex.h>

#define MAX_MATCHES 1 // maximum number of matches

using namespace std;

int main(int argc, char* argv[])
{
    cerr << "Processing " << argv[1] << "..." << endl;
    
    /* States of the automata */
    enum state_t { start, fastaSeq, fastqSeq, fastqQual } state;
    state = start;
    
    /* File descriptor to the input file */
    string file = argv[1];
    fstream fd;
    fd.open(file.c_str(), ios::in);
    
    /* Declaration and compilation of regex */ 
    regex_t notDna, notQual;
    regcomp(&notDna, "[^acgtACGTnN ]", REG_EXTENDED);
    regcomp(&notQual, "[^!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ ]", REG_EXTENDED);
    regmatch_t matches[MAX_MATCHES];
    
    char c;
    string line, seq, qual, id, desc;
    size_t pos;
    size_t counter(0), countLine(0);
    size_t numOfSequence(0);

    while ( getline(fd, line) )
    {
        if ( line != "" )
            c = line.at(0);
        
        counter++;
        
        if( state == start && c == '>' ) {
            
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
            
            /* Return last sequence */
            cout << id << endl;
            cout << "sq[" << seq << "]" << endl;
            
            seq  = "";
            pos  = line.find(" ");
            id   = line.substr(1, pos-1);
            desc = line.substr(pos+1, string::npos);
            numOfSequence++;
            
        } else if ( state == fastqSeq && c == '+' ) {
            
            state = fastqQual;
            
        } else if ( state == fastqQual && c == '@' && countLine == 0 ) {
            
            if( countLine != 0 ) {
                cerr << "ERROR: not same number of lines for sequence and quality at line " << counter;
                cerr << line << endl;
                throw 3;
            }
            
            /* Return last sequence */
            cout << id << endl;
            cout << "sq[" << seq  << "]" << endl;
            cout << "ql[" << qual << "]" << endl;
            
            /* New read */
            seq = qual = "";
            pos = line.find(" ");
            id  = line.substr(1, pos-1);
            numOfSequence++;
            countLine = 0;
            
            state = fastqSeq;
            
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
        }
    }
    
    /* Process last file sequence */
    if( state == fastaSeq ) {
        
        cout << id << endl;
        cout << "sq[" << seq << "]" << endl;
        
    } else if ( state == fastqQual ) {
        
        cout << id << endl;
        cout << "sq[" << seq  << "]" << endl;
        cout << "ql[" << qual << "]" << endl;
        
    }
    
    /* Free and close */
    cerr << "Number of sequences processed: " << numOfSequence << endl;
    cerr << "Closing file " << argv[1] << endl;
    regfree(&notDna);
    regfree(&notQual);
    fd.close();
    
    return 0;
}
