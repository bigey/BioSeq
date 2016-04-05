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
    Lk(k_size), R(read), Lr(read.get_length()), SA(sa), Lg(sa.get_length())
{
    /* Reserve the memory to store position of each kmer */
    if (Lr-Lk+1 > 0)
    {
        Rname = R.get_id();
        Gname = sa.get_id();

        match.reserve(Lr-Lk+1);
        make_kmer();

        vector< vector<size_t> > k_match_f, k_match_r;
        vector<size_t> k_pos_f, k_pos_r;
        size_t nb_kmer_located_f(0), nb_kmer_located_r(0);

        /* Try to locate on forward strand */
        cerr << "\nforward strand\n";
        for (size_t i = 0; i < Lr-Lk+1; i++) {
            k_match_f.push_back(match[i].pos_f);
        }

        nb_kmer_located_f = locate_read(k_match_f, k_pos_f);

        for (size_t i = 0; i < Lr-Lk+1; i++)
            cerr << k_pos_f[i] << ",";

        if (nb_kmer_located_f)
            cerr << "\n" << nb_kmer_located_f
                 << " k-mer localized\n";

        /* Try to locate on reverse strand */
        cerr << "\nreverse strand\n";
        for (size_t i = 0; i < Lr-Lk+1; i++) {
            k_match_r.push_back(match[i].pos_r);
        }

        nb_kmer_located_r = locate_read(k_match_r, k_pos_r);

        for (size_t i = 0; i < Lr-Lk+1; i++)
            cerr << k_pos_r[i] << ",";

        if (nb_kmer_located_r)
            cerr << "\n" << nb_kmer_located_r
                 << " k-mer localized\n";
    }
    else
    {
        cerr << "ERROR: sequence length shouldn't be shorter than k-mer!\n";
        throw;
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
     *   nb of Kmer = Lr-Lk+1, where:
     *     Lr is read length
     *     Lk is kmer length
     */
    for (size_t i = 0; i < Lr-Lk+1; i++)
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
    long int gap_comput = Lr-Lk;
    long int gap_observ;
    size_t counter(0);

    /* Fill the k-mer vector */
    for (size_t i = 0; i < Lr-Lk+1; i++)
        k_pos.push_back(NONE);

    /* For each k-mer pair in the read */
    for (size_t k = 0; k < (Lr-Lk)/2+1; k++)
    {
        size_t begin = k;
        size_t end   = Lr-Lk-k;

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
                        counter += 2;
                    }
                }
            }
        }
    }

    // cerr << "kmer positions: " << k_pos.size() << endl;

    // for (size_t i = 0; i < Lr-Lk+1; i++)
    //     cerr << k_pos[i] << ",";
    //
    // cerr << endl;

    return counter;
}


/**
 * Determine read location
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
}


/**
 * Determine read location
 */
// size_t FindMatch::locate_read()
// {
//     /* Generate k-mer and search for matching positions */
//     cerr << "   Number of kmer expected: " << Lr-Lk+1 << endl;
//     make_kmer();
//
//     vector<size_t> kmer_list;
//     string strand;
//     long int gap_max = 10;
//     long int gap_comput = Lr-Lk;
//     long int gap_observ;
//     size_t counter(0);
//
//     /* Fill the kmer_list vector */
//     for (size_t i = 0; i < Lr-Lk+1; i++)
//         kmer_list.push_back(NONE);
//
//     /* For each k-mer pair in the read */
//     for (size_t k = 0; k < (Lr-Lk)/2+1; k++)
//     {
//         size_t begin = k;
//         size_t end   = Lr-Lk-k;
//
//         /* For each match position on left k-mer */
//         for (size_t i = 0; i < match[begin].pos_f.size(); i++)
//         {
//             /* For each match positions on right k-mer */
//             for (size_t j = 0; j < match[end].pos_f.size(); j++)
//             {
//                 /* Calculate distance in between */
//                 gap_observ = match[end].pos_f[j] - match[begin].pos_f[i];
//
//                 /* Left k-mer matching pos. should be > right k-mer pos. */
//                 if (match[end].pos_f[j] >= match[begin].pos_f[i])
//                 {
//                     /* If -max < |distance| < +max */
//                     if (gap_observ - gap_comput > -gap_max ||
//                         gap_observ - gap_comput < gap_max)
//                     {
//                         // Good relative positions
//                         // cerr <<  "(" << begin << "," << end
//                         //      << ")-(" << match[begin].pos_f[i] << ","
//                         //      << match[end].pos_f[j] << ")\n";
//
//                         /* Store positions */
//                         kmer_list[begin] = match[begin].pos_f[i];
//                         kmer_list[end]   = match[end].pos_f[j];
//                         counter++;
//                     }
//                 }
//             }
//         }
//     }
//
//     cerr << "kmer positions: " << kmer_list.size() << endl;
//
//     for (size_t i = 0; i < Lr-Lk+1; i++)
//         cerr << kmer_list[i] << ",";
//
//     cerr << endl;
//
//     return counter;
// }


// size_t FindMatch::best_path()
// {
    /* Strand determination according to Kmer majority */
    // for (size_t i = 0; i < match.size(); i++)
    // {
    //     f_read.push_back(match[i].pos_f);
    //     r_read.push_back(match[i].pos_r);
    //
    //     size_t f = match[i].pos_f.size();
    //     size_t r = match[i].pos_r.size();
    //     f_pos += f;  //= match[i].pos_f.size();
    //     r_pos += r;  //= match[i].pos_r.size();
    //
    //     // if (f_pos > r_pos)
    //     //     strand_f++;
    //     // else if (f_pos < r_pos)
    //     //     strand_r++;
    // }

    // cerr << "total mathes: " << f_pos << "(+) and " << r_pos << "(-)"<< endl;
    //
    // if ( f_pos > r_pos )
    // {
    //     if ( f_pos < nb_mis_read )
    //     {
    //         cerr << "WARN: mapping is ambiguous! skeep.\n";
    //         return 0;
    //     }
    //     strand = 1;
    //     read = f_read;
    // }
    // else if (f_pos < r_pos)
    // {
    //     if ( r_pos < nb_mis_read )
    //     {
    //         cerr << "WARN: mapping is ambiguous! skeep.\n";
    //         return 0;
    //     }
    //     strand = -1;
    //     read = r_read;
    // }
    // else
    // {
    //     cerr << "WARN: mapping is ambiguous! skeep.\n";
    //     return 0;
    // }

    /* Set the default k-mer list according to strand */
    // read = strand > 0 ? f_read : r_read;

    /* Search the shorter Kmer path */
    // size_t first  = 0;
    // size_t second = 1;
    // size_t valid_p = NONE;
    //
    // cerr << "Strand: " << strand << endl;
    // cerr << "Starting while until kmer: " << read_size << endl;
    //
    // while ( first < read_size-1 && second < read_size )
    // {
    //     int dist_min(100);
    //     size_t best_first_p, best_second_p, fisrt_position;
    //     vector<size_t> first_p  = read[first];
    //     vector<size_t> second_p = read[second];
    //
    //     cerr << "start: " << first << " and " << second << endl;
    //
    //     if (first_p[0] == NONE) {
    //
    //         cerr << "first_p[0] fail\n";
    //         /* No match for first Kmer */
    //         best_m.push_back(NONE);
    //         first++; second++;
    //         continue;
    //     }
    //     else if (second_p[0] == NONE) {
    //         cerr << "second_p[0] fail\n";
    //         /* No matchh for second Kmer */
    //         best_m.push_back(NONE);
    //         second++;
    //         continue;
    //     }
    //
    //     std::cerr << "Kmer test:" << first << "/" << second << ": ";
    //
    //     /* For each positions found for the first Kmer */
    //     for (size_t i = 0; i < first_p.size(); i++)
    //     {
    //         if (valid_p == NONE)
    //             fisrt_position = first_p[i];
    //         else
    //             fisrt_position = valid_p;
    //
    //         /* Search the best position of the next Kmer */
    //         for (size_t j = 0; j < second_p.size(); j++)
    //         {
    //             /* The minimum distance between Kmer */
    //             int dist = second_p[j] - fisrt_position;  //first_p[i];
    //             cerr << dist << "<" << dist_min << endl;
    //
    //             /* If the distance is decreased: OK */
    //             if ( dist > 0 && dist < dist_min )
    //             {
    //                 dist_min = dist;
    //                 best_first_p  = fisrt_position;  //first_p[i];
    //                 best_second_p = second_p[j];
    //                 // cerr << "test1 ok " << dist << " " << dist_min << "\n";
    //             }
    //         }
    //     }
    //
    //     /* If respective positins found */
    //     if (dist_min < 50)
    //     {
    //         if (first == 0) {
    //             best_m.push_back(best_first_p);
    //         }
    //
    //         best_m.push_back(best_second_p);
    //         valid_p = best_second_p;
    //
    //         cerr << "validated: " << best_first_p << "/" << best_second_p << endl;
    //     }
    //     else
    //     {   cerr << "test nt valid, dist_min:" << dist_min << "\n";
    //         cerr << "not valid!" << endl;
    //         best_m.push_back(NONE);
    //     }
    //
    //     first = second;
    //     second++;
    // }
//
//     /* Return normally */
//     return 1;
// }


/**
 *
 */
void FindMatch::analyze() const
{

    // string cigar("");
    // size_t size = best_m.size();
    //
    // // cerr << "   Nb of best matches: " << size << endl;
    //
    // size_t x_p = 0;
    // size_t y_p = best_m[0];
    //
    // if (y_p == NONE) {
    //     return;
    // }
    //
    // for (size_t i = 1; i < size; i++)
    // {
    //     size_t x_n = i;
    //     size_t y_n = best_m[i];
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
    //
    // cerr << cigar << endl;
}
