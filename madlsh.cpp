#include "madlsh.h"


ThresholdIndex *init(int l, int m, int data_dim, float* min_vect, float* max_vect)
{
	ThresholdIndex*	my_index = new ThresholdIndex;
	my_index->L = l;
	my_index->M = m;
	my_index->dim = data_dim;
	my_index->min_vector = min_vect;
	my_index->max_vector = max_vect;
	my_index->bucket_values = new unordered_map<string, unordered_map<float*, float*>>[my_index->L];
	my_index->dimension_array = new int*[my_index->L];
	my_index->threshold_array = new float*[my_index->L];
	seed_seq seq{1, 2, 3, 4, 5};
	mt19937 eng(seq);
//	mt19937 eng;
	uniform_real_distribution<float> unif(0, data_dim);
	unif(eng);

	for(int i = 0; i < my_index->L; i++)
	{
		my_index->dimension_array[i] = new int[my_index->M];
		my_index->threshold_array[i] = new float[my_index->M];

		for(int j = 0; j < my_index->M; j++)
		{
			int local_dim = int(unif(eng));
			uniform_real_distribution<float> local_unif(my_index->min_vector[local_dim], my_index->max_vector[local_dim]);
			float local_threshold = local_unif(eng);
			my_index->dimension_array[i][j] = local_dim;
			my_index->threshold_array[i][j] = local_threshold;
		}
	}
	return	my_index;
};

void add_to_group(ThresholdIndex *index, float* data, int group)
{
	unordered_map<string, unordered_map<float*, float*> >& local_bucket_values = index->bucket_values[group];
	string hk = hash_key(index, hk, data, group);
	local_bucket_values[hk][data] = data;
}

void insert(ThresholdIndex *index, float* data)
{
	//Test if this data has already been inserted or not
    unordered_set<float*>::iterator data_values_ite = index->data_values.find(data);
    if(data_values_ite != index->data_values.end())
    	return;
    else
    	index->data_values.insert(data);

	for(int i=0;i<index->L; i++)
	{
		add_to_group(index, data, i);
    }
}
int hash_value(ThresholdIndex *index, float* data, int i, int j)
{
	if(data[index->dimension_array[i][j]] > index->threshold_array[i][j])
		return 1;
	else
		return 0;
}
void hash_key(ThresholdIndex *index, string& hk, float* data, int group)
{
	hk = "";
	for(int i = 0; i < index->M; i++)
	{
		hk += to_string(hash_value(index, data, group, i));
	}
}

//char* hash_key(ThresholdIndex *index, float* data, int group)
//{
//	string hk = "";
//	for(int i = 0; i < index->M; i++)
//	{
//		hk += to_string(hash_value(index, data, group, i));
//	}
//	return hk.c_str();
//}

const string& hash_key(ThresholdIndex *index, float* data, int group)
{

	for(int i = 0; i < index->M; i++)
	{
		local_char_array += hash_value(index, data, group, i);
	}
	string hk(local_char_array);
	return hk;
}

float** query(ThresholdIndex *index, float* data, int N, int* res_number)
{
	float**	res = new float*[N];
	unordered_map<string, unordered_map<float*, float*> >::iterator local_ite;
	unordered_map<float*, float*>::iterator local_local_ite;
	unordered_map<float*> local_res;
	unordered_map<float*>::iterator local_res_ite;
	int query_counter = 0;
	for(int i = 0; i < index->L; i++)
	{
		unordered_map<string, unordered_map<float*, float*> >& local_bucket_values = index->bucket_values[i];
		string hk = "";
		hash_key(index, hk, data, i);
		local_ite = local_bucket_values.find(hk);

		if(local_ite != local_bucket_values.end())
		{
			for(	local_local_ite = local_ite->second.begin();
					local_local_ite != local_ite->second.end();
					local_local_ite++)
			{

				local_res_ite	= local_res.find();
				if(local_res_ite == local_res.end())
				{
					res[query_counter] = local_local_ite->second;
					query_counter += 1;
					local_res.insert(local_local_ite->second);
				}

				if(query_counter >= N)
				{
					res_number[0] = N;
					return res;
				}
			}
		}
	}
	res_number[0] = query_counter;
	return	res;
}

float** test()
{
	float**	res = new float*[1];
	res[0] = new float[1];
	res[0][0] = 50.;
	return	res;
}
