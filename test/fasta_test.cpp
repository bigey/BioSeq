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
    
    /* Set nb of chars per line */
    size_t step = 80;
    
    char base[] = {'A','C','G','T'};
    size_t simulation = 50;           // number of simulations
    size_t maxLength = 1000;
    char fasta[] = "test.fasta";
    vector<string> Sequences;
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
        /* Random sequence generation */
        cerr << "random sequence generation...\n";
        string seq("");
        size_t length = rand() % maxLength + maxLength;
        total += length;

        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
      
        Sequences.push_back(seq);
        
        /* Writing to file */
        cerr << "writing sequence " << i << " of length: " << length << endl;
        ofs << ">id1|test1 sequence description\n";
        
        for(size_t i = 0; i < length; i += step) {
                ofs << seq.substr(i, step) << endl;
        }
        
        if ( ofs.bad() ) {
            cerr << "Error writing file! abort.";
            return 1;
        }
    }
    
    ofs.close();
    
    /* Reading sequences from file */
    cerr << "Reading sequences from file...\n";
    SeqIO seqio(fasta, 'r');
    
    for (size_t i = 0; i < simulation; i++)
    {
        cerr << "   reading sequence " << i << " of length: " 
             << Sequences[i].size() << "...\n";
             
        cerr << "   creating GenericSeq object...\n";
        GenericSeq gs = seqio.next();
        
        cerr << "   comparing original and stored sequences...\n";
        assert( gs.get_seq() == Sequences[i] );
        cerr << "   -> test ok\n\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << "all tests passed!\n";
    cerr << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
