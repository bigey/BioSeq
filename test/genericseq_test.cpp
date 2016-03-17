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
    size_t sim = 50;
    
    for (size_t i = 0; i < sim; i++)
    {    
        string seq("");
        size_t length = rand() % 100 + 1;
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
        cout << "testing operator[]()..." << endl;
        for(size_t k = 0; k < length; k++) {
            //cout << gs.get_symbol_at(k);
            assert( gs.get_symbol_at(k) == seq[k] );
        }
        cout << " -> test ok\n";
        
        /* insertion operator */
        cout << "testing operator<<() to stdout..." << endl;
        cout << gs;
        cout << " -> test ok\n";
    }
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "all tests passed!\n";
    cout << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
