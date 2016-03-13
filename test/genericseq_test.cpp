#include <assert.h>
#include <cstdlib>
#include "../src/GenericSeq.h"

using namespace std;

int main(int argc, char** argv)
{   
    
    /* initialize timing */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    /* initialize random seed */
    srand( time(NULL) );
    
    char base[] = {'A','C','G','T'};
    size_t total = 0;
    
    for (size_t i = 0; i < 50; i++)
    {    
        string seq("");
        size_t length = rand() % 10000000 + 1;
        total += length;

        for(size_t j = 0; j < length; j++) {
            char c = base[rand() % 4];
            seq += c;
        }
      
        cout << "testing a sequence of length: " << length << endl;
        GenericSeq gs("", "", seq);
        assert( gs.get_seq() == seq );
    }
    
    
    
    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "all tests passed!\n";
    cout << total << " nucleotides proceed in " << cpu_time_used << " sec\n";
    
    return 0;
}
