#include <unordered_map>
#include <unordered_set>
#include <string>
#include <random>
#include <iostream>

using namespace std;

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

    vector<int>	hash_table;


} ThresholdIndex;


extern "C" {
	ThresholdIndex *init(int l, int m, int data_dim, float* min_vect, float* max_vect);
    float** query(ThresholdIndex *index, float* data, int N, int* res_number);
    void add_to_group(ThresholdIndex *index, float* data, int group);
    void insert(ThresholdIndex *index, float* data);
    int hash_value(ThresholdIndex *index, float* data, int i, int j);
    void hash_key(ThresholdIndex *index, string& hk, float* data, int group);
    char* hash_key(ThresholdIndex *index, float* data, int group);
    float** test();
}
