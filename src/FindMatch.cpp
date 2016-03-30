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
    /* Reserve the memory to store each kmer search results */
    if (Lr - Lk + 1 > 0)
        match.reserve(Lr - Lk + 1);
    
}


/**
 * Destructor
 */
FindMatch::~FindMatch() {}


/*
 * Given a word of size 'l' found at index 'i' in sa[], return a 
 *   list of index positons where the word were also found in 
 *   the suffixes of the indexed text
 */
void FindMatch::get_all_match(size_t i, size_t l, vector<size_t>& pos) const
{   
    // ajouter i à la liste
    pos.push_back( SA[i] );
    
    // recherche amont de i
    cout << "         recherche en amont...\n";
    size_t k = 1;
    while ( i+k < Lg && SA.get_lcp(i+k) >= l )
    {
        cout << "         " << " pos[" << i+k << "]=" << SA[i+k] << ", lcp:" << SA.get_lcp(i+k) << endl;
        pos.push_back( SA[i+k] );
        cout << "         push_back(" << SA[i+k] << ")" << endl;
        k++;
    }
    
    cout << "         fin. " << i+k << endl;
    
    
    // recherche aval de i
    cout << "         recherche en aval...\n";
    k = 0;
    while ( i-k > 0 && SA.get_lcp(i-k) >= l )
    {
        cout << "         " << " pos[" << i-k << "]=" << SA[i-k-1] << ", lcp:" << SA.get_lcp(i-k) << endl;
        pos.push_back( SA[i-k-1] );
        k++;
    }
    
    cout << "         fin. " << i-k << endl;
    sort(pos.begin(), pos.end());
}


/**
 * Search of each Kmer on reference unsing Suffix Array
 */
void FindMatch::find_matches()
{
    
    string kmer_f(Lk, 'X');
    string kmer_r(Lk, 'X');
    
    cerr << R << endl;
    
    /* For each kmer, search matches on reference */
    for (size_t i = 0; i < Lr-Lk+1; i++)
    {
        vector<size_t> f;
        vector<size_t> r;
        
        cerr << endl;
        cerr << "      Search kmer " << i << endl;
        size_t index;
        
        /* forward strand */
        cerr << "      FORWARD: " << i << ".." << i+Lk-1 << endl;
        kmer_f = R.get_sub_seq(i, i+Lk-1);
        cerr << "         kmer_f:" << kmer_f << "\n";
        
        index = SA.find_match(kmer_f);
        cerr << "         index:" << index << endl;
        
        // if at least one match found
        if ( index != 0xFFFFFFFFFFFFFFFF )
        {   
            cerr << "            pos[" << index << "]=" << SA[index] << endl;
            // Get alternative positions in reference
            cerr << "         Get alternative positions...\n";
            get_all_match(index, Lk, f);
            
        }
        
        cerr << "         forward strand: " 
             << f.size() 
             << " position(s) found" << endl;
        
        
             
        /* reverse strand */
        cerr << "      REVERSE\n";
        
        kmer_r = R.revcom().get_sub_seq(i, i+Lk-1);
        cerr << "         kmer_r:" << kmer_r << endl;
        
        index = SA.find_match(kmer_r);
        cerr << "         index:" << index << endl;;
        
        // if at least one match found
        if ( index != 0xFFFFFFFFFFFFFFFF )
        {
            cerr << "            pos[" << index << "]=" << SA[index] << endl;
            
            // Get alternative positions in reference
            get_all_match(index, Lk, r);
        }
        
        cerr << "         reverse strand: " << r.size()
             << " position(s) found" << endl;
             
        
        cerr << "         next kmer...\n\n";
    }
    cerr << endl;
}









/**
 * Analyze the matching Kmer list
 */
// void FindMatch::analyze() const
// {
//     string cigar("");
//     cout << "   Number of kmer found: " << match.size() << endl;
//     
//     size_t x_p = match[0].r;
//     size_t y_p = match[0].g;
//     
//     for (size_t i = 1; i < match.size(); i++)
//     {
//         cerr << "kmer: " << i << endl;
//         //size_t x_p = match[i-1].r;
//         size_t x_n = match[i].r;
//         long int dx  = x_n - x_p;
//         
//         //size_t y_p = match[i-1].g;
//         size_t y_n = match[i].g;
//         long int dy  = y_n - y_p;
//         
//         long int dxdy = dx - dy;
//         
//         cerr << "dx:" << dx << " dy:" << dy << " dx-dy:" << dxdy << endl;
//         
//         // Perfect Match
//         if ( dx == 1 && dy == 1 )
//         {
//             cigar += "M";
//             //cout << "M";
//         }
//         
//         // Substitution
//         else if ( dx - Lk > 1 && dy - Lk > 1 && dx - dy == 0 )
//         {
//             long int Lsub = dx - Lk - 1;
//             assert (Lsub >= 0);
//             cigar += string(Lsub, 'M');
//         }
//         
//         // Insertion
//         else if ( dx - Lk > 0 && dy - Lk == 0 )
//         {
//             long int Lins = dx - Lk;
//             assert (Lins >= 0);
//             cigar += string(Lins, 'I');
//         }
//         
//         // Deletion
//         else if ( dx - Lk == 0 && dy - Lk > 0 )
//         {
//             long int Ldel = dy - Lk;
//             assert (Ldel >= 0);
//             cigar += string(Ldel, 'D');
//         }
//         
//         // Complex: Substitution + Indel
//         if ( dx > dx )
//         {
//             long int Lsub = dy - Lk - 1;
//             long int Lins = dxdy;
//             assert (Lsub+Lins >= 0);
//             cigar += string(Lsub + Lins, 'U');
//         }
//         else if ( dx < dy )
//         {
//             long int Lsub = dx - Lk - 1;
//             long int Ldel = - dxdy;
//             assert (Lsub+Ldel >= 0);
//             cigar += string(Lsub + Ldel, 'U');
//         }
//         
//         x_p = x_n;
//         y_p = y_n;
//     }
//     
//     cout << cigar << endl;
// 
// }

