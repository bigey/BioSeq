#include <assert.h>
#include <cstdlib>
#include "../src/GenericSeq.h"

using namespace std;

int main(int argc, char** argv)
{   
    /* initialize timing variables */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    /* initialize random seed */
    srand( time(NULL) );
    
    char base[] = {'A','C','G','T'};
    size_t total = 0;
    size_t mean_length = 10000;
    size_t sim = 1000;
    
    for (size_t i = 0; i < sim; i++)
    {    
        cout << "[Test:" << i+1 << "]\n";
        
        string seq("");
        size_t length = rand() % mean_length + 1;
        total += length;

        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
        
        /* encoding */
        cout << "testing encoding (sequence length: " << length << ")";
        GenericSeq gs("", "", seq);
        cout << " -> test ok\n";
        
        /* decoding */
        cout << "testing decoding...";
        assert( gs.get_seq() == seq );
        cout << " -> test ok\n";
        
        /* copy constructor */
        cout << "testing copy constructor...";
        GenericSeq gs3(gs);
        assert( gs3.get_seq() == seq );
        cout << " -> test ok\n";
        
        /* affectation operator */
        cout << "testing operator=...";
        GenericSeq gs2 = gs;
        assert( gs2.get_seq() == seq );
        cout << " -> test ok\n";
        
        /* operator [] */
        cout << "testing operator[]()...";
        for(size_t k = 0; k < length; k++) {
            //cout << gs.get_symbol_at(k);
            assert( gs.get_symbol_at(k) == seq[k] );
        }
        cout << " -> test ok\n";
        
        /* insertion operator << */
        cout << "testing operator<<() to stdout..." << endl;
        cout << gs << endl;
        cout << " -> test ok\n";
        
        /* revcom() function */
        cout << "testing revcom()...\n";
        GenericSeq rev_gs = gs.revcom();
        //cout << rev_gs << endl;
        assert( gs.get_seq() == rev_gs.revcom().get_seq() );
        cout << " -> test ok\n";
        
        /* one base modification */
        cout << "testing set_symbol_at()...";
        //cout << "  changing random base";
        for (size_t l = 0; l < 100; l++)
        {
            //cout << "\n";
            
            size_t p = rand() % gs.get_length();
            char c = base[rand() % 4];
            
            //cout << gs << " " << p << " " 
            //     << seq[p] << ">" 
            //     << c << endl;
                 
            gs.set_symbol_at(p, c);
            seq[p] = c;
            
            //cout << seq << endl;
            //cout <<  gs << endl;
            
            assert( gs.get_seq() == seq );
        }
        cout << " -> tests ok\n";
        cout << "[Test:" << i+1 << "] all tests ok\n\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "all tests passed!\n";
    cout << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
