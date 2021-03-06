// #include <assert.h>
// #include <string>  // std::string, std::stoi
#include "../src/GenericSeq.h"
#include "../src/SuffixArray.h"
#include "../src/SeqIO_b.h"
#include "../src/FindMatch.h"

using namespace std;


void usage();

void usage()
{
    cerr << endl
         << "    USAGE: GameSetMatch <k-mer> <in-fasta> <in-fastq>"
         << endl;
}

int main(int argc, char** argv)
{
    /* User input parameters */
    // char fasta[] = "/home/fbigey/Projets/BioSeq/data/sequence.fasta";
    // char fasta[] = "/home/fbigey/Projets/BioSeq/data/short_seq.fasta";
    // char fastq[] = "/home/fbigey/Projets/BioSeq/data/run1.fastq";
    // char fastq[] = "/home/fbigey/Projets/BioSeq/data/short_read.fastq";

    /* Select k-mer length here */
    // size_t kmer_size = 21;

    if (argc < 4)
    {
        usage();
        return 1;
    }

    char* pEnd;
    size_t kmer_size = strtol (argv[1], &pEnd, 10);
    string fasta = argv[2];
    string fastq = argv[3];

    cerr << "k-mer size: " << kmer_size << endl;
    cerr << "reference file: " << fasta << endl;
    cerr << "read file: " << fastq << endl;


    /* Declaration of timing variables */
    clock_t prog_start = clock();
    clock_t start;
    double cpu_time;

    /* Testing sequence indexation */
    start = clock();
    cerr << "(Step 1): indexing reference file...\n";
    SeqIO seqio(fasta, 'r');
    seqio.index();

    cerr << "(Step 2): indexing read file...\n";
    SeqIO read_io(fastq, 'r');
    read_io.index();

    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cerr << " total time: " << cpu_time << " sec -> ok\n";

    /* Reading file */
    cerr << "(Step 3): reading reference file...\n";
    start = clock();
    GenericSeq gs;
    seqio.read(gs);
    cerr << "   length: " << gs.get_length() << endl;
    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cerr << "   total time: " << cpu_time << " sec -> ok\n";

    /* Computing suffix array and LCP array */
    cerr << "(Step 4): computing suffix array and LCP array...\n";
    start = clock();
    SuffixArray sa(gs);
    cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    cerr << "   total time: " << cpu_time << " sec -> ok\n";

    /* Opening and indexing read file */
    // cerr << "(Step 4): indexing read file..." << endl;
    // start = clock();
    // SeqIO read_io(fastq, 'r');
    // read_io.index();
    // cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    // cerr << "   total time: " << cpu_time << " sec -> ok\n";

    /* Reading read sequences */
    cerr << "(Step 5): processing reads..." << endl;
    size_t i(1);
    GenericSeq gs_read;

    while ( read_io.read(gs_read) )
    {
        cerr << "[Read: " << i << "] " << gs_read.get_id()
             << ", " << gs_read.get_length() << " bp\n";

        /* If read to short skeep */
        if (gs_read.get_length() < kmer_size * 2)
        {
            cerr << "   Read to short! Skeep" << endl;
            continue;
        }

        /* Use FindMatch class to locate read on reference */
        start = clock();
        FindMatch fm(gs_read, sa, kmer_size);
        fm.analyze();
        fm.output();

        cpu_time = ((double) (clock() - start)) / CLOCKS_PER_SEC;
        cerr << "      done in " << cpu_time << " sec -> ok\n";

        /* Next read now */
        cerr << "[Read: " << i << "] processed\n\n";
        i++;
    }

    /* Overall runing time */
    cpu_time = ((double) (clock() - prog_start)) / CLOCKS_PER_SEC;
    cerr << "Job completed in " << cpu_time << " sec\n";
    cerr << i << " reads processed.";

    return 0;
}
