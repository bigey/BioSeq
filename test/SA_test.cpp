#include <assert.h>
#include <iomanip>
#include "../src/SA_LCP.h"

using namespace std;

int main(int argc, char** argv)
{
    cerr << "Begining test...\n";
    
    /* initialize timing */
    clock_t start, start_step, end;
    double cpu_time_used;
    start = clock();
    
    /* initialize random seed */
    srand( time(NULL) );
    
    char base[] = {'A','C','G','T'};
    size_t simulation = 10;             // number of simulations
    size_t seqLength = 10000;
    size_t total = 0;
        
    /* Generating suffix array on random sequences */
    for (size_t i = 0; i < simulation; i++)
    {    
        string seq("");
        size_t length = rand() % seqLength/2 + seqLength;
        total += length;
        
        /* Generating random sequences */
        cerr << "[Test: " << i+1 << "] \n";
        cerr << "Generating random sequence of length " << length << "...\n";
        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
        
        seq += "$";
        //cout << seq << endl;
        
        /* Computing suffix array */
        cerr << "Computing suffix array... ";
        start_step = clock();
        vector<size_t> sa = buildSA(seq);
        end = clock();
        cpu_time_used = ((double) (end - start_step)) / CLOCKS_PER_SEC;
        cerr << "done in " << cpu_time_used << " sec\n";

        /* Computing longest common prefix array (LCP array) */
        cerr << "Computing LCP array... ";
        start_step = clock();
        vector<size_t> lcp = buildLCP(seq, sa);
        end = clock();
        cpu_time_used = ((double) (end - start_step)) / CLOCKS_PER_SEC;
        cerr << "done in " << cpu_time_used << " sec\n";
        
        /* Testing */
        cerr << "Testing if suffix array is correctly sorted...\n";
        for(size_t k = 0; k < sa.size(); k++)
        {
//             cout << setw(5) << k << setw(5) << sa.at(k) << " " 
//                  << setw(5) << lcp[k]   << " " 
//                  << seq.substr(sa.at(k), string::npos);
            
            /* testing if string at SA[i] < SA[i+1] */
            if( k < sa.size() - 1 )
                assert(seq.substr(sa.at(k), string::npos) <= seq.substr(sa.at(k+1), string::npos));
            
//             cout << " ok\n";
        }
        
        /* Validation of LCP array construction */
        cerr << "Testing LPC array result...\n";
//         cout << setw(5) << 0 << setw(5) << 0 << " " << "undefined" << endl;
        for(size_t p = 1; p < sa.size(); p++)
        {
            //cout << setw(5) << sa.at(k)
            string si = seq.substr(sa.at(p-1), string::npos);
            string sj = seq.substr(sa.at(p), string::npos);
            
//             cout << setw(5) << p << setw(5) << lcp[p] << " " 
//                  << seq.substr(sa.at(p-1), string::npos) << endl;
            
            assert( si.substr(0,lcp[p]) == sj.substr(0,lcp[p]) );
            assert( si.substr(0,lcp[p]+1) != sj.substr(0,lcp[p]+1) );
        }
        
        cerr << "[Test: " << i+1 << "] passed\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << "all tests passed!\n";
    cerr << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
