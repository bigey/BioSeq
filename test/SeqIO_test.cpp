#include <assert.h>
#include <cstdlib>
#include <vector>
#include "../src/SeqIO_b.h"

using namespace std;

int main(int argc, char** argv)
{   
    /* Initialize random seed */
    srand( time(NULL) );
    
    /* Set nb of chars per line */
    size_t step = 80;
    
    char base[] = {'A','C','G','T'};
    size_t simulation = 1000; // number of simulation
    size_t maxLength = 10000;
    char fasta[] = "test.fasta";
    vector<string> Sequences;
    size_t total = 0;
    
    cerr << "Begining test...\n";
    
    /* Openning file */
    cerr << "Opening file...\n";
    ofstream ofs;
    ofs.open(fasta);
    if ( ! ofs.is_open() ) {
        cerr << "Error opening file!";
        return 1;
    }
        
    /* Generating random sequences */
    cerr << "Generating random sequences\n";
    for (size_t i = 0; i < simulation; i++)
    {    
        /* Random length */
        size_t length = rand() % maxLength + maxLength;
        cerr << "   (Sequence " << i << ") length: " << length << endl;
        total += length;
        
        /* Random sequence generation */
        cerr << "      generating random...\n";
        string seq(length, 'X');
        for (size_t j = 0; j < length; j++) {
            seq[j] = base[rand() & 3];
        }
        
        /* Store a copy for later comparison */
        cerr << "      storing a copy...\n";
        Sequences.push_back(seq);
        
        /* Writing sequence to file */
        cerr << "      writing sequence n." << i << " to file..." << endl;
        ofs << ">id" << i << " sequence n." << i << " description";
        for (size_t i = 0; i < length; i++)
        {
            if (!(i % step)) {
                ofs << endl;
            }
            ofs << seq[i];
        }
        ofs << endl;
        
        if ( ofs.bad() ) {
            cerr << "Error writing file! abort.";
            return 1;
        }
    }
    
    /* Close the output file */
    ofs.close();
      
    /* Initialize timing */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    /* Reading sequences from file */
    cerr << "Reading sequences from file...\n";
    SeqIO seqio(fasta, 'r');
    
    /* Testing sequences indexation */
    cerr << "   (Step 1): indexing...";
    seqio.index();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << " total time: " << cpu_time_used << " sec\n";
    
    /* For each sequence */
    cerr << "   (Step 2): reading sequences...\n"; 
    for (size_t i = 0; i < simulation; i++)
    {
        cerr << "      (Sequence " << i << ")\n";
             
        cerr << "         creating empty GenericSeq object...\n";
        GenericSeq gs;
        
        cerr << "         reading and encoding the sequence...";
        start = clock();
        assert( seqio.read(gs) );
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cerr << " total time: " << cpu_time_used << " sec\n";
        
        cerr << "         comparing from original...";
        assert( gs.get_seq() == Sequences[i] );
        cerr << " -> test ok\n\n";
    }
    
    cerr << "All tests passed!\n";
    return 0;
}
