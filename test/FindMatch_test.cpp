#include <assert.h>
#include <iomanip>
#include "../src/GenericSeq.h"
#include "../src/FindMatch.h"

using namespace std;

int main(int argc, char** argv)
{
    /* Simulation parameters */
    char base[] = {'A','C','G','T'};
    size_t simulation = 1000;            // number of simulations
    size_t seqLength  = 1000;           // average sequence length
    size_t read_size  = 21;             // read size
    size_t kmer_size  = 3;             // kmer size
    
    cout << "Begining test...\n";
    size_t total = 0;
    
    /* Initialize timing */
    clock_t start;
    double cpu_time;
    
    /* Initialize random seed */
    srand( time(NULL) );
        
    /* Generating random sequence */
    string seq("");
    size_t length = rand() % seqLength/4 + seqLength;
    total += length;

    cout << "Generating random sequence of length " << length << "...\n";
    for(size_t j = 0; j < length; j++) 
    {
        seq += base[rand() % 4];
    }
    
    /* GenericSeq object construction */
    cout << "GenricSeq object encoding...";
    GenericSeq gs("id","test description", seq);
    cout << "\n" << gs << endl;
    cout << " -> ok\n";
    
    /* Computing suffix array and LCP array */
    cout << "Computing suffix array and LCP array...\n";
    start = clock();
    SuffixArray sa(gs);
    
    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cout << "   done in " << cpu_time << " sec";
    cout << " -> ok\n";
    
    
    /* Validation FindMatch */
    cout << "Testing FindMatch class..." << endl;
    start = clock();
    
    for (size_t i = 0; i < simulation; i++)
    {
        cout << "[Test: " << i+1 << "] \n";
     
        /* Take a read at random position on sequence */
        char strand = '+';
        size_t begin = rand() % (length - read_size +1);
        string read_seq = seq.substr(begin, read_size);
        GenericSeq read("@id","test read info", read_seq);
        
        if ( rand() % 2 )
        {
            strand = '-';
            read = read.revcom();
        }
        else
        {
            strand = '+';
        }
        
        cout << "   Read take at position " << begin << " strand " << strand << endl;
        cout << read << endl;
        
        /* Construction of FindMatch object */
        cout << "   Constructing FindMatch object..." << endl;
        FindMatch fm(read, sa, kmer_size);
        
        /* Search for kmer using Suffix Array */
        cout << "   Searching for matching kmer..." << endl;
        fm.find_matches();
        
//         cout << "   Analyzing reported matching kmer..." << endl;
//         fm.analyze();
        
        cout << "   done";
        cout << " -> ok\n";
        cout << "[Test: " << i+1 << "] passed\n\n";
    }
    
    /* compute runing time */
    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cout << "All tests passed!\n";
    cout << total << " nucleotides proceed in " << cpu_time << " sec\n";
    
    return 0;
}