#include <assert.h>
#include <cstdlib>
#include <vector>
#include "../src/SeqIO_b.h"

using namespace std;

int main(int argc, char** argv)
{   
    char base[] = {'A','C','G','T'};
    string qualSymbol = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ";
    int qual_size = qualSymbol.size();
    
    size_t simulation = 100000;  // number of simul
    char fasta[] = "test.fastq";
    vector<string> Sequences;
    vector<string> Qualities;
    size_t total = 0;
    
    cerr << "Begining test...\n";
    
    /* Initialize timing */
    clock_t begin, start, end;
    double cpu_time_used;
    begin = clock();
    
    /* initialize random seed */
    srand( time(NULL) );
    
    /* Openning file */
    cerr << "Opening file...\n";
    ofstream ofs;
    ofs.open(fasta);
    if ( ! ofs.is_open() ) {
        cerr << "Error opening file!";
        return 1;
    }
        
    /* Writting random sequences to file */
    for (size_t i = 0; i < simulation; i++)
    {    
        string seq(""), qual("");
        size_t length = rand() % 20 + 100;
        total += length;
        
        cerr << "Random sequence generation...\n";
        for(size_t j = 0; j < length; j++) {
            char c = base[ rand() % 4 ];
            seq += c;
        }
      
        cerr << "Random quality generation...\n";
        for(size_t j = 0; j < length; j++) {
            qual += qualSymbol[ rand() % qual_size ];
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
    
    
    /* Testing sequences indexation */
    start = clock();
    cerr << "   (Step 1): indexing...";
    seqio.index();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << " total time: " << cpu_time_used << " sec\n";
    
    
    /* Reading sequence from file */
    cerr << "   (Step 2): reading sequences...\n";
    for (size_t i = 0; i < simulation; i++)
    {
        cerr << "      (Sequence " << i << ")\n";
        cerr << "         reading sequence " << i << " of length: " 
             << Sequences[i].size() << "...\n";
             
        cerr << "         creating GenericSeq object...";
        GenericSeq gs;
        start = clock();
        assert( seqio.read(gs) );
        end = clock();
        
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cerr << " total time: " << cpu_time_used << " sec\n";
        
        cerr << "         comparing original and GenericSeq sequences...";
        assert( gs.get_seq() == Sequences[i] );
        cerr << " -> test ok\n\n";
    }
    
    
    cpu_time_used = ((double) (clock() - begin)) / CLOCKS_PER_SEC;
    cerr << "Total nucleotides processed: " << total << endl;
    cerr << " total time: " << cpu_time_used << " sec\n";
    cerr << "All tests passed!\n";
    return 0;
}
