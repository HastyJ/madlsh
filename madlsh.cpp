#include <unordered_map>
#include <string>
#include <random>

using namespace std;



struct Index{
    Index(int l, int m, int data_dim, double* min_vect, double* max_vect)
    {
	L = l;
	M = m;
	dim = data_dim;
        min_vector = min_vect;
        max_vector = max_vect;
        bucket_values = new unordered_map<string, double*>[L];
        dimension_array = new int*[L];
        threshold_array = new double*[L];
	mt19937 eng;  
	uniform_real_distribution<double> unif(0, data_dim);
	unif(eng);
	
	for(int i = 0; i < L; i++)
	{
	    dimension_array[i] = new int[M];
	    threshold_array[i] = new double[M];
	    
	    for(int j = 0; j < M; j++)
	    {
	        int local_dim = int(unif(eng));
		uniform_real_distribution<double> local_unif(min_vector[local_dim], max_vector[local_dim]);
                double local_threshold = local_unif(eng);
		dimension_array[i][j] = local_dim;
		threshold_array[i][j] = local_threshold;
	    }
	}
	local_hash_vector = new int[M];
    };    
    int L;
    int M;
    int dim;
    int* local_hash_vector;
    double* min_vector;
    double* max_vector;
    unordered_map<string, double*>* bucket_values;
    int** dimension_array;
    double** threshold_array;   
    double** query(double* data, int N);

    void add_to_group(double* data, int group)
    {
	unordered_map<string, double*>& local_bucket_values = bucket_values[group];
	string hk;
	hash_key(hk, data, group);
	unordered_map<string, double*>::iterator local_ite = local_bucket_values.find(hk);
	if(local_ite == local_bucket_values.end())
	{
	    local_bucket_values[hk] = data;
	}
	
    };

    void insert(double* data)
    {
	for(int i=0;i<L; i++)
	{
	    add_to_group(data, i);	
        }
    };

    int hash_value(double* data, int i, int j)
    {
	if(data[dimension_array[i][j]] > threshold_array[i][j])
	    return 1;
	else
	    return 0;
    }
    void hash_key(string& hk, double* data, int group)
    {
	hk = "";
	for(int i = 0; i < M; i++)
	{
	    hk += to_string(hash_value(data, group, i));
	}
    };
};

