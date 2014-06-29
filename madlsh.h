#include <unordered_map>
#include <unordered_set>
#include <string>
#include <random>
#include <iostream>
#include <bitset>

using namespace std;

/*
 * ThresholdIndex Structure : for every hash function h of every amplification function j,
 * choose a random dimension i and an input threshold T from the real vector structure.
 * If the value of the input data for the dimension i is over the threshold T, then the hash function returns 1, and 0 otherwise.
 */
typedef struct
{
	//Number of amplification functions
    int L;

    //Number of hash functions per amplification functions.
    int M;

    //Input data dimension
    int dim;

    //Vector of maximum possible threshold values.
    float* min_vector;

    //Vector of maximum possible threshold values.
    float* max_vector;

    //The actual bucket container : an array of unordered map with string keys
    //(built from the custom hash table vectors).
    unordered_map<string, unordered_map<float*, float*> >* bucket_values;

    //Raw data pointer container so we don't store the same data twice.
    unordered_set<float*> data_values;

    //LxM array storing the dimension that have been randomly selected.
    int** dimension_array;

    //LxM array storing the threshold that have been randomly selected.
    float** threshold_array;

    //Local char array used to speed up hashing.
    char*	local_char_array;

    //Local int vector used to speed up hashing.
    vector<int>	hash_table;


} ThresholdIndex;


extern "C" {
	/*
	 * Initialize the ThresholdIndex with the number of amplification (aggregation) functions l,
	 * the number of hash functions per amplification function m,
	 * the dimension of the input data data_dim,
	 * the array of minimum values for every dimension min_vect,
	 * the array of maximum values for every dimension max_vect.
	 * init(...) chooses randomly m dimension for every amplification function,
	 * and randomly selects a threshold between min_vect[i] and max_vect[i] for every chosen dimension i.
	 */
	ThresholdIndex *init(int l, int m, int data_dim, float* min_vect, float* max_vect);

	/*
	 * Insert the array data in the index (computes a hash table for every amplification function).
	 */
    void insert(ThresholdIndex *index, float* data);

	/*
	 *
	 */
    float** query(ThresholdIndex *index, float* data, int N, int* res_number);
    void add_to_group(ThresholdIndex *index, float* data, int group);
    int hash_value(ThresholdIndex *index, float* data, int i, int j);
    void hash_key(ThresholdIndex *index, string& hk, float* data, int group);
//    char* hash_key(ThresholdIndex *index, float* data, int group);
    float** test();
}
