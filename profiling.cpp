#include <iostream>
#include <unordered_set>
#include <iomanip>      // std::setw
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "splay_tree.h"
#include "red_black_tree.h"
#include "genzipf.h"
#include "timer.h"

using namespace std;

#define MAX_TREE_SIZE 16384     // (2 ^ 14)
#define E9            1000000000
#define NUM_TRIALS    10

double alpha_arr[] =     {0, 0.516, 0.687, 0.892, 0.975, 1.058, 1.257, 1.42};
double skew_factor[] = {0.01,  0.10,  0.20,  0.40,  0.50,  0.60,  0.80, 0.90};

double stree_BUS_values[7];
double stree_TDS_values[7];
double rbtree_values[7];

// range - [min, max)
int random(int min, int max)
{
    static bool first = true;
    if(first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

void test_activity_ratio(SplayTree& stree_TDS, SplayTree& stree_BUS, RBTree& rbtree, int APT[], unordered_set<int> &uset,
                         int nUpdates, double alpha)
{
    int zipf_index_array[MAX_TREE_SIZE];
    int nDeletions = nUpdates / 2;      // = num of insertions
    int nSearches = 100 - nUpdates;
    int index, elem, cycles = 1000;

    double stree_TDS_search_time = 0.0,
           stree_BUS_search_time = 0.0,
           rbtree_search_time = 0.0;

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
            stree_BUS.search(APT[zipf_index_array[i]]);

        timer.stop();
        stree_BUS_search_time += timer.duration_in_us();

        timer.start();
        for(int i = 0; i < nSearches; ++i)
            stree_TDS.search(APT[zipf_index_array[i]]);

        timer.stop();
        stree_TDS_search_time += timer.duration_in_us();

        timer.start();
        for(int i = 0; i < nSearches; ++i)
            rbtree.search(APT[zipf_index_array[i]]);

        timer.stop();
        rbtree_search_time += timer.duration_in_us();
    }
    stree_TDS.num_ins_comps /= (double) nDeletions * 1000.0;
    stree_TDS_values[0] += stree_TDS.num_ins_comps;
    stree_TDS.num_ins_rots /= (double) nDeletions * 1000.0;
    stree_TDS_values[1] += stree_TDS.num_ins_rots;

    stree_TDS.num_del_comps /= (double) nDeletions * 1000.0;
    stree_TDS_values[2] += stree_TDS.num_del_comps;
    stree_TDS.num_del_rots /= (double) nDeletions * 1000.0;
    stree_TDS_values[3] += stree_TDS.num_del_rots;

    stree_TDS.num_search_comps /= (double) nSearches * 1000.0;
    stree_TDS_values[4] += stree_TDS.num_search_comps;
    stree_TDS.num_search_rots /= (double) nSearches * 1000.0;
    stree_TDS_values[5] += stree_TDS.num_search_rots;
    stree_TDS_search_time /= 1000.0;
    stree_TDS_values[6] += stree_TDS_search_time;

    
    stree_BUS.num_ins_comps /= (double) nDeletions * 1000.0;
    stree_BUS_values[0] += stree_BUS.num_ins_comps;
    stree_BUS.num_ins_rots /= (double) nDeletions * 1000.0;
    stree_BUS_values[1] += stree_BUS.num_ins_rots;

    stree_BUS.num_del_comps /= (double) nDeletions * 1000.0;
    stree_BUS_values[2] += stree_BUS.num_del_comps;
    stree_BUS.num_del_rots /= (double) nDeletions * 1000.0;
    stree_BUS_values[3] += stree_BUS.num_del_rots;

    stree_BUS.num_search_comps /= (double) nSearches * 1000.0;
    stree_BUS_values[4] += stree_BUS.num_search_comps;
    stree_BUS.num_search_rots /= (double) nSearches * 1000.0;
    stree_BUS_values[5] += stree_BUS.num_search_rots;
    stree_BUS_search_time /= 1000.0;
    stree_BUS_values[6] += stree_BUS_search_time;


    rbtree.num_ins_comps /= (double) nDeletions * 1000.0;
    rbtree_values[0] += rbtree.num_ins_comps;
    rbtree.num_ins_rots /= (double) nDeletions * 1000.0;
    rbtree_values[1] += rbtree.num_ins_rots;

    rbtree.num_del_comps /= (double) nDeletions * 1000.0;
    rbtree_values[2] += rbtree.num_del_comps;
    rbtree.num_del_rots /= (double) nDeletions * 1000.0;
    rbtree_values[3] += rbtree.num_del_rots;

    rbtree.num_search_comps /= (double) nSearches * 1000.0;
    rbtree_values[4] += rbtree.num_search_comps;
    rbtree.num_search_rots /= (double) nSearches * 1000.0;
    rbtree_values[5] += rbtree.num_search_rots;
    rbtree_search_time /= 1000.0;
    rbtree_values[6] += rbtree_search_time;


    #if 0
    cout << "\n\n";
    cout << fixed << setprecision(2);
    cout << alpha << "\t\tBU-splay\t" << stree_BUS.num_ins_comps << "\t" << stree_BUS.num_ins_rots << "\t" << stree_BUS.num_del_comps << "\t" << stree_BUS.num_del_rots << "\t" << stree_BUS.num_search_comps << "\t" << stree_BUS.num_search_rots << "\t"  << stree_BUS_search_time << "\n";
    cout << "\t\tTD-splay\t" << stree_TDS.num_ins_comps << "\t" << stree_TDS.num_ins_rots << "\t" << stree_TDS.num_del_comps << "\t" << stree_TDS.num_del_rots << "\t" << stree_TDS.num_search_comps << "\t" << stree_TDS.num_search_rots << "\t"  << stree_TDS_search_time << "\n";
    cout << "\t\tRed-Black\t" << rbtree.num_ins_comps << "\t" << rbtree.num_ins_rots << "\t" << rbtree.num_del_comps << "\t" << rbtree.num_del_rots << "\t" << rbtree.num_search_comps << "\t" << rbtree.num_search_rots << "\t"  << rbtree_search_time << "\n\n";
    #endif
}


void table_values_init()
{
    for (int i = 0; i < 7; ++i)
    {
        stree_BUS_values[i] = 0.0;
        stree_TDS_values[i] = 0.0;
        rbtree_values[i] = 0.0;
    }
}

void table_values_avg()
{
    for (int i = 0; i < 7; ++i)
    {
        stree_BUS_values[i] /= NUM_TRIALS;
        stree_TDS_values[i] /= NUM_TRIALS;
        rbtree_values[i] /= NUM_TRIALS;
    }
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
    int size_alpha_arr = sizeof(alpha_arr) / sizeof(alpha_arr[0]);

    int activity_ratio[] = {0, 20, 50, 80};
    int size_act_ratio = sizeof(activity_ratio) / sizeof(activity_ratio[0]);

    ofstream t1_out("table2.txt");
    ofstream t2_out("table3.txt");

    for(int i = 0; i < size_alpha_arr; ++i)
    {
        table_values_init();
        for(int j = 0; j < NUM_TRIALS; ++j)
        {
            generate_APT(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset);
            test_activity_ratio(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset, 20, alpha_arr[i]);
        }
        table_values_avg();

        t1_out << skew_factor[i];

        t1_out << "\n";
        for(int j = 0; j < 7; ++j)
        {
            t1_out << fixed << setprecision(2);
            t1_out << stree_BUS_values[j] << "\t";
        }

        t1_out << "\n";
        for(int j = 0; j < 7; ++j)
        {
            t1_out << fixed << setprecision(2);
            t1_out << stree_TDS_values[j] << "\t";
        }

        t1_out << "\n";
        for(int j = 0; j < 7; ++j)
        {
            t1_out << fixed << setprecision(2);
            t1_out << rbtree_values[j] << "\t";
        }

        t1_out << "\n";
    }

    for(int i = 0; i < size_act_ratio; ++i)
    {
        t2_out << activity_ratio[i] << "\n";

        for(int j = 0; j < size_alpha_arr; ++j)
        {

            table_values_init();
            for(int k = 0; k < NUM_TRIALS; ++k)
            {
                generate_APT(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset);
                test_activity_ratio(splay_tree_TDS, splay_tree_BUS, rbtree, access_probability_table, uset, activity_ratio[i], alpha_arr[j]);
            }
            table_values_avg();

            t2_out << fixed << setprecision(2);
            t2_out << skew_factor[j] << "\t" << rbtree_values[6] << "\t" << stree_BUS_values[6] << "\t" << stree_TDS_values[6] << "\n";
        }
    }

    return 0;
}
