#include <assert.h>
#include <iomanip>
#include "../src/SA.h"

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
    size_t simulation = 1;           // number of simulations
    size_t seqLength = 1000000;
    size_t total = 0;
        
    /* Generating suffix array on random sequences */
    for (size_t i = 0; i < simulation; i++)
    {    
        string seq("");
        size_t length = rand() % seqLength + 1;
        total += length;
        
        /* Generating random sequences */
        cerr << "[Test: " << i+1 << "] \n";
        cerr << "Generating random sequence of length " << length << "...\n";
        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
        
        seq += "$";
        
        /* Computing suffix array */
        cerr << "Computing suffix array... ";
        vector<size_t> sa = buildSA(seq);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cerr << "done in " << cpu_time_used << " sec\n";

        /* Testing */
        cerr << "Testing if suffix array is correctly sorted...\n";
        for(size_t k = 0; k < sa.size(); k++)
        {
            //cout << setw(5) << sa.at(k) << " " << seq.substr(sa.at(k), string::npos);
            
            /* testing if string at SA[i] < SA[i+1] */
            if( k < sa.size() - 1 )
                assert(seq.substr(sa.at(k), string::npos) <= seq.substr(sa.at(k+1), string::npos));
            
            //cout << " ok\n";
        }
        
        cerr << "[Test: " << i+1 << "] sorted ok\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << "all tests passed!\n";
    cerr << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
