#include <assert.h>
#include <iomanip>
#include "../src/SuffixArray.h"

using namespace std;

int main(int argc, char** argv)
{
    /* Simulation parameters */
    char base[] = {'A','C','G','T'};
    size_t simulation = 1;            // number of simulations
    size_t seqLength = 100000;      // average sequence length
    size_t kmer_size = 21;            // kmer size

    cout << "Begining test...\n";
    size_t total = 0;

    /* Initialize timing */
    clock_t start, start_step, end;
    double cpu_time_used;
    start = clock();

    /* Initialize random seed */
    srand( time(NULL) );

    /* Generating suffix array on random sequences */
    for (size_t i = 0; i < simulation; i++)
    {
        string seq("");
        size_t length = rand() % seqLength/4 + seqLength;
        total += length;

        cout << "[Test: " << i+1 << "] \n";

        /* Generating random sequences */
        cout << "Generating random sequence of length " << length << "...\n";
        for(size_t j = 0; j < length; j++)
        {
            seq += base[rand() % 4];
        }

        /* GenericSeq object construction */
        cout << "GenricSeq object encoding...";
        GenericSeq gs("id"+0,"test description", seq);
        cout << " -> ok\n";

        /* Computing suffix array and LCP array */
        cout << "Computing suffix array and LCP array...\n";

        start_step = clock();
        SuffixArray sa(gs);
        end = clock();

        cpu_time_used = ((double) (end - start_step)) / CLOCKS_PER_SEC;
        cout << "   done in " << cpu_time_used << " sec";
        cout << " -> ok\n";

        //return 0;

        /* Testing */
        cout << "Validating suffix array...\n";

        //cout << "\n";
        //cout << setw(5) << "i" << setw(7) << "sa[i]" << " "
        //     << setw(7) << "lcp[i]" << " suffix\n" << endl;

        for(size_t k = 0; k < sa.get_length(); k++)
        {
            //cout << setw(5) << k << setw(7) << sa[k] << " "
            //    << setw(7);

            if (k == 0)
            {
                //cout << "undef" << " ";
            }
            else
            {
                //cout << sa.get_lcp(k) << " ";
            }

            //cout << seq.substr(sa[k], string::npos);

            /* testing if string at SA[i] > SA[i-1] */
            if( k > 0 ) {
                //cout << " > " << seq.substr(sa[k-1], string::npos) << " ";
                assert(seq.substr(sa[k], string::npos) >= seq.substr(sa[k-1], string::npos));
            }

            //cout << " -> ok\n";
        }
        cout << "Suffix array -> ok\n";

        /* Validation of LCP array construction */
        cout << "Validating LPC array...";

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


        /* Validation of find_match() */
        cout << "Validating find_match()..." << endl;

        size_t begin = rand() % (length - kmer_size +1);
        string query = seq.substr(begin, kmer_size);

        //cout << seq << endl;
        //cout << query << endl;
        cout << "   query take at position " << begin << endl;

        start_step = clock();
        size_t match_found = sa[ sa.find_match(query)] ;
        cpu_time_used = ((double) (clock() - start_step)) / CLOCKS_PER_SEC;

        if (match_found == 0xFFFFFFFFFFFFFFFF)
        {
            cout << "   query NOT found!" << endl;
        }
        else
        {
            cout << "   query found at position "
                 << match_found << endl;

        }

        assert( begin == match_found );
        cout << "   done in " << cpu_time_used << " sec";
        cout << " -> ok\n";
        cout << "[Test: " << i+1 << "] passed\n\n";
    }

    /* compute runing time */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "All tests passed!\n";
    cout << total << " nucleotides proceed in " << cpu_time_used << " sec\n";

    return 0;
}
