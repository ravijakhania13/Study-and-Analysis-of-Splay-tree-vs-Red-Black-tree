#include <iostream>
#include <unordered_set>
#include <iomanip>      // std::setw
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "splay_tree.h"
#include "red_black_tree.h"
#include "genzipf.h"
#include "timer.h"

using namespace std;

#define MAX_TREE_SIZE (4096 * 4)
#define E9            1000000000

// range - [min, max)
int random(int min, int max)
{
    return min + rand() % (( max + 1 ) - min);
}

void test_activity_ratio(SplayTree& stree_TDS, SplayTree& stree_BUS, RBTree& rbtree, int APT[], unordered_set<int> &uset,
                         int nUpdates, double alpha)
{
    int zipf_index_array[MAX_TREE_SIZE];
    int nDeletions = nUpdates / 2;      // = num of insertions
    int nSearches = 100 - nUpdates;
    int index, elem, cycles = 1000;

    double stree_TDS_search_time = 0.0, stree_BUS_search_time = 0.0, rbtree_search_time = 0.0;

    Timer timer;

    gen_zipf_update_seed();
    stree_TDS.readingsInit();
    stree_BUS.readingsInit();
    rbtree.readingsInit();
    while(cycles--)
    {
        for(int i = 0; i < nDeletions; ++i)
        {
            /* delete a randomly selected element from the
               Access Probability Table */
            index = random(0, MAX_TREE_SIZE);
            stree_TDS.remove(APT[index]);
            stree_BUS.remove(APT[index]);
            rbtree.remove(APT[index]);

            do
            {
                elem = random(1, E9 + 1);
            }while(uset.find(elem) != uset.end());
            uset.erase(APT[index]);

            /* insert a randomly selected element in the SplayTree and
               at the same index in the Access Probability Table */
            stree_TDS.insert(elem);
            stree_BUS.insert(elem);
            rbtree.insert(elem);

            APT[index] = elem;
            uset.insert(elem);
        }

        gen_zipf_distribution(alpha, MAX_TREE_SIZE, nSearches, zipf_index_array);

        timer.start();
        for(int i = 0; i < nSearches; ++i)
            stree_TDS.search(APT[zipf_index_array[i]]);

        timer.stop();
        stree_TDS_search_time += timer.duration_in_us();

        timer.start();
        for(int i = 0; i < nSearches; ++i)
            stree_BUS.search(APT[zipf_index_array[i]]);

        timer.stop();
        stree_BUS_search_time += timer.duration_in_us();

        timer.start();
        for(int i = 0; i < nSearches; ++i)
            rbtree.search(APT[zipf_index_array[i]]);

        timer.stop();
        rbtree_search_time += timer.duration_in_us();
    }
    stree_TDS_search_time /= 1000.0;
    stree_TDS.num_del_rots /= (double) nDeletions * 1000.0;
    stree_TDS.num_del_comps /= (double) nDeletions * 1000.0;

    stree_TDS.num_ins_rots /= (double) nDeletions * 1000.0;
    stree_TDS.num_ins_comps /= (double) nDeletions * 1000.0;

    stree_TDS.num_search_rots /= (double) nSearches * 1000.0;
    stree_TDS.num_search_comps /= (double) nSearches * 1000.0;

    
    stree_BUS_search_time /= 1000.0;
    stree_BUS.num_del_rots /= (double) nDeletions * 1000.0;
    stree_BUS.num_del_comps /= (double) nDeletions * 1000.0;

    stree_BUS.num_ins_rots /= (double) nDeletions * 1000.0;
    stree_BUS.num_ins_comps /= (double) nDeletions * 1000.0;

    stree_BUS.num_search_rots /= (double) nSearches * 1000.0;
    stree_BUS.num_search_comps /= (double) nSearches * 1000.0;


    rbtree_search_time /= 1000.0;
    rbtree.num_del_rots /= (double) nDeletions * 1000.0;
    rbtree.num_del_comps /= (double) nDeletions * 1000.0;

    rbtree.num_ins_rots /= (double) nDeletions * 1000.0;
    rbtree.num_ins_comps /= (double) nDeletions * 1000.0;

    rbtree.num_search_rots /= (double) nSearches * 1000.0;
    rbtree.num_search_comps /= (double) nSearches * 1000.0;


    cout << fixed << setprecision(2);
    cout << alpha << "\t\tBU-splay\t" << stree_BUS.num_ins_comps << "\t" << stree_BUS.num_ins_rots << "\t" << stree_BUS.num_del_comps << "\t" << stree_BUS.num_del_rots << "\t" << stree_BUS.num_search_comps << "\t" << stree_BUS.num_search_rots << "\t"  << stree_BUS_search_time << "\n";
    cout << "\t\tTD-splay\t" << stree_TDS.num_ins_comps << "\t" << stree_TDS.num_ins_rots << "\t" << stree_TDS.num_del_comps << "\t" << stree_TDS.num_del_rots << "\t" << stree_TDS.num_search_comps << "\t" << stree_TDS.num_search_rots << "\t"  << stree_TDS_search_time << "\n";
    cout << "\t\tRed-Black\t" << rbtree.num_ins_comps << "\t" << rbtree.num_ins_rots << "\t" << rbtree.num_del_comps << "\t" << rbtree.num_del_rots << "\t" << rbtree.num_search_comps << "\t" << rbtree.num_search_rots << "\t"  << rbtree_search_time << "\n\n";
}


// APT stands for Access Probability Table
void generate_APT(SplayTree& stree_TDS, SplayTree& stree_BUS, RBTree& rbtree, int APT[], unordered_set<int>& uset)
{
    int elem, pos;

    stree_TDS.clear();
    stree_BUS.clear();
    rbtree.clear();
    uset.clear();
    memset(APT, 0, MAX_TREE_SIZE * sizeof(int));

    srand(time(NULL));

    for(int i = 0; i < MAX_TREE_SIZE; ++i)
    {
        elem = random(1, E9 + 1);

        // for keeping unique numbers in the tree
        if (uset.find(elem) != uset.end())
        {
            --i;
            continue;
        }

        pos = (random(1, E9 + 1)) % MAX_TREE_SIZE;
        while (APT[pos])
        {
            pos = (pos + 1) % MAX_TREE_SIZE;
        }
        APT[pos] = elem;
        stree_TDS.insert(elem);
        stree_BUS.insert(elem);
        rbtree.insert(elem);
        uset.insert(elem);
    }
}


int main(int argc, char* argv[])
{
    SplayTree splay_tree_TDS, splay_tree_BUS(Splaying::BOTTOM_UP);
    RBTree rbtree;
    unordered_set<int> uset;
    int access_probability_table[MAX_TREE_SIZE] = {0};
    double alpha[] = {0, 0.516, 0.687, 0.892, 0.975, 1.058, 1.257, 1.42};
    int alpha_size = sizeof(alpha) / sizeof(alpha[0]);

    int activity_ratio[] = {0, 20, 50, 80};

    for(int j = 0; j < 4; ++j)
    {
        cout << "\n\nActivity Ratio: " << activity_ratio[j] << endl;
        cout << "Skew Factor\tMethod Used\tInsert\t\tDeletion\tSearch\t\tCPU Time\n";
        for(int i = 0; i < alpha_size; ++i)
        {
            generate_APT(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset);
            test_activity_ratio(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset, activity_ratio[j], alpha[i]);
        }
    }
}
