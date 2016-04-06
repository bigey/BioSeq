#include "FindMatch.h"

using namespace std;


/**
 * Default constructor
 */
FindMatch::FindMatch() {}


/**
 * Constructor using: GenericSeq, SuffixArray and k-mer size
 */
FindMatch::FindMatch(const GenericSeq& read, const SuffixArray& sa, const size_t k_size):
    Lk(k_size), R(read), Lr(read.get_length()), SA(sa), Lg(sa.get_length()),
    strand(0), cigar("")
{
    /* If read length is longer than k-mer length */
    if (Lr-Lk+1 > 0)
    {
        nb_of_kmer = Lr-Lk+1;
        Rname = R.get_id();
        Gname = sa.get_id();

        match.reserve(nb_of_kmer);
        make_kmer();

        vector< vector<size_t> > k_match_f, k_match_r;
        size_t nb_kmer_located_f(0), nb_kmer_located_r(0);

        // cerr << "nb of k-mer expected:" << nb_of_kmer << endl;


        /* Try to locate on forward strand */
        cerr << "\nforward strand\n";
        for (size_t i = 0; i < nb_of_kmer; i++) {
            k_match_f.push_back(match[i].pos_f);
        }

        nb_kmer_located_f = locate_read(k_match_f, k_pos_f);

        // for (size_t i = 0; i < nb_of_kmer; i++)
        // {
        //     if(k_pos_f[i] == NONE)
        //     {
        //         cerr << ".";
        //     }
        //     else
        //     {
        //         cerr << k_pos_f[i] << ",";
        //     }
        // }
        // cerr << endl;
        //
        // if (nb_kmer_located_f)
        //     cerr << "\n" << nb_kmer_located_f
        //          << " k-mer localized\n";


        /* Try to locate on reverse strand */
        cerr << "\nreverse strand\n";
        for (size_t i = 0; i < nb_of_kmer; i++) {
            k_match_r.push_back(match[i].pos_r);
        }

        nb_kmer_located_r = locate_read(k_match_r, k_pos_r);

        // for (size_t i = 0; i < nb_of_kmer; i++)
        // {
        //     if (k_pos_r[i] == NONE) {
        //         cerr << ".";
        //     }
        //     else
        //     {
        //         cerr << k_pos_r[i] << " ";
        //     }
        // }
        // cerr << endl;
        //
        // if (nb_kmer_located_r)
        //     cerr << "\n" << nb_kmer_located_r
        //          << " k-mer localized\n";

        /* If nb of matches on forward strand greater */
        if (nb_kmer_located_f > nb_kmer_located_r)
        {
            strand = +1;

            size_t i = 0;
            while (i < nb_of_kmer && k_pos_f[i] == NONE) { i++; }
            match_begin = k_pos_f[i];

            i = Lr-Lk;
            while (i > 0 && k_pos_f[i] == NONE) { i--; }
            match_end = k_pos_f[i]+Lk-1;
        }
        else if (nb_kmer_located_r > 0)
        {
            strand = -1;

            size_t i = 0;
            while (i < nb_of_kmer && k_pos_r[i] == NONE) { i++; }
            match_begin = k_pos_r[i];

            i = Lr-Lk;
            while (i > 0 && k_pos_r[i] == NONE) { i--; }
            match_end = k_pos_r[i]+Lk-1;
        }
    }
    else
    {
        cerr << "ERROR: sequence length shouldn't be shorter than k-mer!\n";
    }
}


/**
 * Destructor: no dynamic memory allocation here
 */
FindMatch::~FindMatch() {}


/*
 * Search for identical prefixes in adjacent suffixes in SA
 *     i: index in the SA,
 *     l: length of prefixes,
 *   pos: vector to store corresponding positions.
 */
void FindMatch::kmer_search(size_t i, size_t l, vector<size_t>& pos) const
{
    /* Store position found at i */
    pos.push_back( SA[i] );

    /* Search for positions found above i in SA */
    size_t k = 1;
    while ( i+k < Lg && SA.get_lcp(i+k) >= l )
    {
        pos.push_back( SA[i+k] );
        k++;
    }

    /* Search for positions below i in SA */
    k = 0;
    while ( i-k > 0 && SA.get_lcp(i-k) >= l )
    {
        pos.push_back( SA[i-k-1] );
        k++;
    }

    /* Sorting in ascending positions */
    sort(pos.begin(), pos.end());
}


/**
 * Search all k-mer positions on reference
 */
void FindMatch::make_kmer()
{
    /* Strings used to store k-mer sequence */
    string kmer_f(Lk, 'X');
    string kmer_r(Lk, 'X');

    /*
     * Generate k-mer from the read sequence
     */
    for (size_t i = 0; i < nb_of_kmer; i++)
    {
        match_t k_match;  // structure of two vector<size_t>

        /* Forward strand k-mer search */
        kmer_f = R.get_sub_seq(i, i+Lk-1);
        size_t index_f = SA.find_match(kmer_f);

        if ( index_f != NONE )
        {   // at least one match found
            kmer_search(index_f, Lk, k_match.pos_f);
        }

        /* Reverse strand k-mer search */
        kmer_r = R.revcom().get_sub_seq(i, i+Lk-1);
        size_t index_r = SA.find_match(kmer_r);

        if ( index_r != NONE )
        {   // at least one match found
            kmer_search(index_r, Lk, k_match.pos_r);
        }

        match.push_back(k_match);
    }
}


/**
 * Locate read on the reference
 */
size_t FindMatch::locate_read(vector< vector<size_t> >& k_match,
    vector<size_t>& k_pos)
{
    long int gap_max = 10;
    long int gap_comput;
    long int gap_observ;
    size_t counter(0);

    /* Fill the k-mer vector with NONE */
    for (size_t i = 0; i < nb_of_kmer; i++)
        k_pos.push_back(NONE);


    /* For each k-mer from the left of the read */
    for (size_t k = 0; k < (Lr-Lk)/2+1; k++)
    {
        size_t begin = k;

        /* For each k-mer from the right of the read */
        for (size_t l = 0; l < (Lr-Lk)/2+1; l++)
        {
            size_t end   = Lr-Lk-l;
            gap_comput = end-begin-Lk;

            if (k_pos[begin] != NONE && k_pos[end] != NONE)
                continue;

            /* For each match position on left k-mer */
            for (size_t i = 0; i < k_match[begin].size(); i++)
            {
                /* For each match positions on right k-mer */
                for (size_t j = 0; j < k_match[end].size(); j++)
                {
                    /* Calculate distance in between */
                    gap_observ = k_match[end].at(j) - k_match[begin].at(i);

                    /* Left k-mer matching pos. should be > right k-mer pos. */
                    if (k_match[end].at(j) >= k_match[begin].at(i))
                    {
                        /* If -max < |distance| < +max */
                        if (gap_observ - gap_comput > -gap_max ||
                            gap_observ - gap_comput < gap_max)
                        {
                            // Good relative positions
                            // cerr <<  "(" << begin << "," << end
                            //      << ")-(" << match[begin].pos_f[i] << ","
                            //      << match[end].pos_f[j] << ")\n";

                            /* Store positions */
                            k_pos[begin] = k_match[begin].at(i);
                            k_pos[end]   = k_match[end].at(j);
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < nb_of_kmer; i++)
    {
        if (k_pos[i] != NONE) {
            counter++;
        }
    }

    return counter;
}


/**
 * Output to STDOUT
 */
void FindMatch::output() const
{
    // read name
    // read length
    // reference name
    // reference length
    // match begin
    // match end
    // strand

    if (strand)
    {
        cout << Rname << "\t"
             << Lr    << "\t"
             << Gname << "\t"
             << SA.get_length() << "\t"
             << match_begin << "\t"
             << match_end << "\t"
             << strand;

        if (cigar != "")
            cout << "\t" << cigar;

        cout << endl;
    }
}


/**
 * Try to impute variation type
 */
void FindMatch::analyze()
{
    vector<size_t> pos;

    if (strand == 0)
    {
        return;
    }
    else if (strand == 1)
    {
        for (size_t i = 0; i < nb_of_kmer; i++) {
            pos.push_back(k_pos_f[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < nb_of_kmer; i++) {
            pos.push_back(k_pos_r[i]);
        }
    }

    size_t x_p(NONE);
    size_t y_p(NONE);
    size_t i(0);

    while (i < nb_of_kmer ) {

        if (pos[i] == NONE) {
            i++;
            continue;
        }

        size_t x_n = i;
        size_t y_n = pos[i];

        if (x_p != NONE || y_p != NONE)
        {
            long int dx  = x_n - x_p;
            long int dy  = y_n - y_p;
            long int dxdy = dx - dy;

            cerr << x_n << "-" << x_p << "=dx=" << dx << " "
                 << y_n << "-" << y_p << "=dy=" << dy
                 << " dx-dy:" << dxdy << endl;

            // Perfect Match
            if ( dx == 1 && dy == 1 )
            {
             cigar += "M";
             //cout << "M";
            }

            // Substitution
            else if ( dx - (long int) Lk > 1 && dy - (long int) Lk > 1 && dx - dy == 0 )
            {
                long int Lsub = dx - (long int) Lk - 1;
                assert (Lsub >= 0);
                cigar += string(Lsub, 'S');
                // cigar += string("S");
            }

            // Insertion
            else if ( dx - (long int) Lk > 0 && dy - (long int) Lk == 0 )
            {
                long int Lins = dx - (long int) Lk;
                assert (Lins >= 0);
                cigar += string(Lins, 'I');
                // cigar += string("I");
            }

            // Deletion
            else if ( dx - (long int) Lk == 0 && dy - (long int) Lk > 0 )
            {
                long int Ldel = dy - (long int) Lk;
                cerr  << Ldel << " " << dy - (long int) Lk << endl;
                assert (Ldel >= 0);
                cigar += string(Ldel, 'D');
                // cigar += string("D");
            }

            // Complex
            else
            {
                cigar += "X";
            }
        }

        // cerr << "cigar:" << cigar << endl;
        // if (p_char != c)
        // {
        //     cigar += string(count) + c;
        //     p_char = c;
        //     count = 0;
        // }

        x_p = x_n;
        y_p = y_n;
        i++;
    }

    return;

    // size_t x_p = rd[0];
    // size_t y_p = rf[0];
    //
    // for (size_t i = 1; i < rd.size(); i++)
    // {
    //     size_t x_n = rd[i];
    //     size_t y_n = rf[i];
    //
    //     long int dx  = x_n - x_p;
    //     long int dy  = y_n - y_p;
    //     long int dxdy = dx - dy;
    //
    //     cerr << x_n << "-" << x_p << "=dx=" << dx << " "
    //          << y_n << "-" << y_p << "=dy=" << dy
    //          << " dx-dy:" << dxdy << endl;
    //
    //     // Perfect Match
    //     if ( dx == 1 && dy == 1 )
    //     {
    //         cigar += "M";
    //         //cout << "M";
    //     }
    //
    //     // Substitution
    //     else if ( dx - Lk > 1 && dy - Lk > 1 && dx - dy == 0 )
    //     {
    //         long int Lsub = dx - Lk - 1;
    //         // assert (Lsub >= 0);
    //         // cigar += string(Lsub, 'S');
    //          cigar += string("S");
    //     }
    //
    //     // Insertion
    //     else if ( dx - Lk > 0 && dy - Lk == 0 )
    //     {
    //         long int Lins = dx - Lk;
    //         // assert (Lins >= 0);
    //         // cigar += string(Lins, 'I');
    //          cigar += string("I");
    //     }
    //
    //     // Deletion
    //     else if ( dx - Lk == 0 && dy - Lk > 0 )
    //     {
    //         long int Ldel = dy - Lk;
    //         // assert (Ldel >= 0);
    //         // cigar += string(Ldel, 'D');
    //         cigar += string("D");
    //     }
    //
    //     // Complex: Substitution + Indel
    //     if ( dx > dx )
    //     {
    //         long int Lsub = dy - Lk - 1;
    //         long int Lins = dxdy;
    //         // assert (Lsub+Lins >= 0);
    //         // cigar += string(Lsub + Lins, 'X');
    //         cigar += string("X");
    //     }
    //     else if ( dx < dy )
    //     {
    //         long int Lsub = dx - Lk - 1;
    //         long int Ldel = - dxdy;
    //         // assert (Lsub+Ldel >= 0);
    //         // cigar += string(Lsub + Ldel, 'Y');
    //         cigar += string("Y");
    //     }
    //
    //     x_p = x_n;
    //     y_p = y_n;
    // }
}
