#include <assert.h>
#include <cstdlib>
#include <vector>
#include "../src/SeqIO.h"

using namespace std;

int main(int argc, char** argv)
{   
    cerr << "Begining test...\n";
    
    /* initialize timing */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    /* initialize random seed */
    srand( time(NULL) );
    
    char base[] = {'A','C','G','T'};
    string qualSymbol = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ ";
    size_t simulation = 100000;           // number of simulations
    char fasta[] = "test.fastq";
    vector<string> Sequences;
    vector<string> Qualities;
    size_t total = 0;
    
    /* Openning file */
    cerr << "Opening file...\n";
    ofstream ofs;
    ofs.open(fasta);
    
    if ( ! ofs.is_open() ) {
        cerr << "Error opening file!";
        return 1;
    }
        
    /* Writting random sequences to file */
    cerr << "Writing sequences to file...\n";
    for (size_t i = 0; i < simulation; i++)
    {    
        string seq(""), qual("");
        size_t length = rand() % 200 + 1;
        total += length;

        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
      
        for(size_t j = 0; j < length; j++) {
            qual += qualSymbol[rand() % 43];
        }
      
        Sequences.push_back(seq);
        Qualities.push_back(qual);
        
        /* Writing to file */
        cerr << "writing sequence " << i << " of length: " << length << endl;
        ofs << "@id" << i << " test read\n";
        ofs << seq << endl;
        
        if ( ofs.bad() ) {
            cerr << "Error writing file! abort.";
            return 1;
        }
        
        ofs << "+id" << i << " test quality\n";
        ofs << qual << endl;
    }
    
    ofs.close();
    
    /* Reading sequences from file */
    cerr << "Reading sequences from file...\n";
    SeqIO seqio(fasta, 'r');
    
    for (size_t i = 0; i < simulation; i++)
    {
        cerr << "reading sequence " << i << " of length: " 
             << Sequences[i].size();
             
        GenericSeq gs = seqio.next();
        assert( gs.get_seq() == Sequences[i] );
        cerr << " test ok\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << "all tests passed!\n";
    cerr << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
