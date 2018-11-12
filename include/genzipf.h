#ifndef _GENZIPF_H_
#define _GENZIPF_H_

double  rand_val(int seed);         // Jain's RNG
void    gen_zipf_distribution(double alpha, int N, int num_values, int* out_array);
void    gen_zipf_update_seed();

#endif // _GENZIPF_H_
