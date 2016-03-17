#include <assert.h>
#include <iomanip>
#include "../src/SuffixArray.h"

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
    size_t simulation = 20;             // number of simulations
    size_t seqLength = 100000;
    size_t total = 0;
        
    /* Generating suffix array on random sequences */
    for (size_t i = 0; i < simulation; i++)
    {    
        string seq("");
        size_t length = rand() % seqLength/2 + seqLength;
        total += length;
        
        cerr << "[Test: " << i+1 << "] \n";
        
        /* Generating random sequences */
        cerr << "Generating random sequence of length " << length << "...\n";
        for(size_t j = 0; j < length; j++) {
            seq += base[rand() % 4];
        }
        
        /* GenericSeq object construction */
        cerr << "GenricSeq object encoding...";
        GenericSeq gs("id"+0,"test description", seq);
        cerr << " -> ok\n";
        
        //cout << gs << endl;
        
        /* Computing suffix array and LCP array */
        cerr << "Computing suffix array and LCP array... ";
        start_step = clock();
        SuffixArray sa(gs);
        end = clock();
        cpu_time_used = ((double) (end - start_step)) / CLOCKS_PER_SEC;
        cerr << "done in " << cpu_time_used << " sec\n";
        
        /* Testing */
        cerr << "Validating suffix array...";
        
        //cout << setw(5) << "i" << setw(7) << "sa[i]" << " " 
        //     << setw(7) << "lcp[i]" << " suffix\n" << endl;
        
        for(size_t k = 0; k < sa.get_length(); k++)
        {
            //cout << setw(5) << k << setw(7) << sa[k] << " " 
            //     << setw(7) << sa.get_lcp(k) << " ";
                 
            //cout << seq.substr(sa[k], string::npos);
            
            /* testing if string at SA[i] > SA[i-1] */
            if( k > 0 )
                //cout << " > " << seq.substr(sa[k-1], string::npos) << " ";
                assert(seq.substr(sa[k], string::npos) >= seq.substr(sa[k-1], string::npos));
            
            //cout << " -> ok\n";
        }
        cout << " -> ok\n";
        
        /* Validation of LCP array construction */
        cerr << "Validating LPC array...";
        
        //cout << setw(5) << 0 << setw(5) << 0 << " " << "undefined" << endl;

        for(size_t p = 1; p < sa.get_length(); p++)
        {
            //cout << setw(5) << sa[k];
            string si = seq.substr(sa[p-1], string::npos);
            string sj = seq.substr(sa[p], string::npos);
            
            //cout << setw(5) << p << setw(5) << sa.get_lcp(p) << " " 
            //     << seq.substr(sa[p-1], string::npos) << endl;
            
            assert( si.substr(0,sa.get_lcp(p)) == sj.substr(0,sa.get_lcp(p)) );
            assert( si.substr(0,sa.get_lcp(p)+1) != sj.substr(0,sa.get_lcp(p)+1) );
        }
        
        cout << " -> ok\n";
        cerr << "[Test: " << i+1 << "] passed\n\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cerr << "All tests passed!\n";
    cerr << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
