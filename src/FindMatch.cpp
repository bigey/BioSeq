#include "FindMatch.h"

using namespace std;


/**
 * Default constructor
 */
FindMatch::FindMatch() {}


/**
 * Constructor using: GenericSeq, SuffixArray and kmer size
 */
FindMatch::FindMatch(const GenericSeq& rd, const SuffixArray& sa, const size_t k_size): Lk(k_size), R(rd), Lr(rd.get_length()), SA(sa), Lg(sa.get_length())
{
    /* Reserve the memory to store position of each kmer */
    if (Lr - Lk + 1 > 0)
    {
        match.reserve(Lr - Lk + 1);
        best_m.reserve(Lr - Lk + 1);
    }
}


/**
 * Destructor: no dynamic memory allocation here
 */
FindMatch::~FindMatch() {}


/*
 * Given a word of size 'l' found at index 'i' in sa[], return a
 *   list of index positions where the word were also found in
 *   the suffixes of the indexed text
 */
void FindMatch::get_all_match(size_t i, size_t l, vector<size_t>& pos) const
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
 * For one Kmer, search all positions on reference
 */
void FindMatch::find_matches()
{
    /* Strings used to store Kmer sequence */
    string kmer_f(Lk, 'X');
    string kmer_r(Lk, 'X');

    /* Generate Kmer from the read sequence */
    for (size_t i = 0; i < Lr-Lk+1; i++)
    {
        match_t k_match;  // vector of two vectors

        /* Forward strand Kmer */
        kmer_f = R.get_sub_seq(i, i+Lk-1);
        size_t index_f = SA.find_match(kmer_f);

        if ( index_f != MAX )
        {   // at least one match found
            get_all_match(index_f, Lk, k_match.pos_f);
        }

        /* Reverse strand */
        kmer_r = R.revcom().get_sub_seq(i, i+Lk-1);
        size_t index_r = SA.find_match(kmer_r);

        if ( index_r != MAX )
        {   // at least one match found
            get_all_match(index_r, Lk, k_match.pos_r);
        }

        /* Store forward and reverse matches in member attribut match */
        if (index_f == MAX && index_r == MAX)
        {
            k_match.pos_f.push_back(MAX);
            k_match.pos_r.push_back(MAX);
        }

        match.push_back(k_match);
    }
}


/**
 * Determine the best Kmer path on the reference
 */
void FindMatch::best_path()
{
    // cerr << "   Number of kmer expected: " << Lr - Lk + 1 << endl;

    vector< vector<size_t> > read, f_read, r_read;
    int strand = 0;

    /* Strand determination according to Kmer majority */
    for (size_t i = 0; i < match.size(); i++)
    {
        f_read.push_back(match[i].pos_f);
        r_read.push_back(match[i].pos_r);
        size_t f_pos = match[i].pos_f.size();
        size_t r_pos = match[i].pos_r.size();

        if (f_pos > r_pos)
            strand++;
        else if (f_pos < r_pos)
            strand--;
    }

    /* Set the default Kmer list according to strand */
    read = strand > 0 ? f_read : r_read;

    /* Search the shorter Kmer path */
    size_t read_size = read.size();
    size_t first  = 0;
    size_t second = 1;
    size_t valid_p = MAX;

    while ( first < read_size-1 && second < read_size )
    {
        int dist_min(100);
        size_t best_first_p, best_second_p, fisrt_position;
        vector<size_t> first_p  = read[first];
        vector<size_t> second_p = read[second];

        if (first_p[0] == MAX) {
            /* No match for first Kmer */
            best_m.push_back(MAX);
            first++; second++;
            continue;
        }
        else if (second_p[0] == MAX) {
            /* No matchh for second Kmer */
            best_m.push_back(MAX);
            second++;
            continue;
        }

        //std::cerr << "Kmer test:" << first << "/" << second << ": ";

        /* For each positions found for the first Kmer */
        for (size_t i = 0; i < first_p.size(); i++)
        {
            if (valid_p == MAX)
                fisrt_position = first_p[i];
            else
                fisrt_position = valid_p;

            /* Search the best position of the next Kmer */
            for (size_t j = 0; j < second_p.size(); j++)
            {
                /* The minimum distance between Kmer */
                int dist = second_p[j] - fisrt_position;  //first_p[i];

                /* If the distance is decreased: OK */
                if ( dist > 0 && dist < dist_min )
                {
                    dist_min = dist;
                    best_first_p  = fisrt_position;  //first_p[i];
                    best_second_p = second_p[j];
                }
            }
        }

        /* If respective positins found */
        if (dist_min < 10)
        {
            if (first == 0) {
                best_m.push_back(best_first_p);
            }

            best_m.push_back(best_second_p);
            valid_p = best_second_p;

            //cerr << "validated: " << best_first_p << "/" << best_second_p << endl;
        }
        else
        {   //cerr << "not valid!" << endl;
            best_m.push_back(MAX);
        }

        first = second;
        second++;
    }
}


/**
 *
 */
void FindMatch::analyze() const
{
    string cigar("");
    size_t size = best_m.size();

    // cerr << "   Nb of best matches: " << size << endl;

    size_t x_p = 0;
    size_t y_p = best_m[0];

    for (size_t i = 1; i < size; i++)
    {
        size_t x_n = i;
        size_t y_n = best_m[i];

        long int dx  = x_n - x_p;
        long int dy  = y_n - y_p;
        long int dxdy = dx - dy;

        // cerr << x_n << "-" << x_p << "=dx=" << dx << " "
        //      << y_n << "-" << y_p << "=dy=" << dy
        //      << " dx-dy:" << dxdy << endl;

        // Perfect Match
        if ( dx == 1 && dy == 1 )
        {
            cigar += "M";
            //cout << "M";
        }

        // Substitution
        else if ( dx - Lk > 1 && dy - Lk > 1 && dx - dy == 0 )
        {
            long int Lsub = dx - Lk - 1;
            assert (Lsub >= 0);
            // cigar += string(Lsub, 'S');
             cigar += string("S");
        }

        // Insertion
        else if ( dx - Lk > 0 && dy - Lk == 0 )
        {
            long int Lins = dx - Lk;
            assert (Lins >= 0);
            // cigar += string(Lins, 'I');
             cigar += string("I");
        }

        // Deletion
        else if ( dx - Lk == 0 && dy - Lk > 0 )
        {
            long int Ldel = dy - Lk;
            assert (Ldel >= 0);
            // cigar += string(Ldel, 'D');
            cigar += string("D");
        }

        // Complex: Substitution + Indel
        if ( dx > dx )
        {
            long int Lsub = dy - Lk - 1;
            long int Lins = dxdy;
            assert (Lsub+Lins >= 0);
            // cigar += string(Lsub + Lins, 'X');
            cigar += string("X");
        }
        else if ( dx < dy )
        {
            long int Lsub = dx - Lk - 1;
            long int Ldel = - dxdy;
            assert (Lsub+Ldel >= 0);
            // cigar += string(Lsub + Ldel, 'Y');
            cigar += string("Y");
        }

        x_p = x_n;
        y_p = y_n;
    }

    // cerr << cigar << endl;
}
