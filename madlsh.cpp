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
	mt19937 eng;
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
	string hk;
	cout <<"add_to_group POSITION 1"<<endl;
	hash_key(index, hk, data, group);
	cout <<"add_to_group POSITION 2"<<endl;
	local_bucket_values[hk][data] = data;
	cout <<"add_to_group POSITION 3"<<endl;
}

void insert(ThresholdIndex *index, float* data)
{
//	cout << "data = "<<data<<endl;
	cout <<"insert POSITION BEGIN"<<endl;
	//Test if this data has already been inserted or not
    unordered_set<float*>::iterator data_values_ite = index->data_values.find(data);
    if(data_values_ite != index->data_values.end())
    	return;
    else
    	index->data_values.insert(data);

	cout <<"insert POSITION 0"<<endl;

	for(int i=0;i<index->L; i++)
	{
		cout <<"insert POSITION 0.1"<<endl;
		add_to_group(index, data, i);
		cout <<"insert POSITION 0.2"<<endl;
    }
	cout <<"insert POSITION END"<<endl;
}
int hash_value(ThresholdIndex *index, float* data, int i, int j)
{
//	if(index->dimension_array[i][j] >= 4096)
//	{
	cout <<"index->dimension_array[i][j] = "<<index->dimension_array[i][j]<<endl;
	cout <<"index->threshold_array[i][j] = "<<index->threshold_array[i][j]<<endl;
	cout <<"data[0] = "<<data[0]<<endl;
	cout <<"data[index->dimension_array[i][j]] = "<<data[index->dimension_array[i][j]]<<endl;
	cout <<"hash_value POSITION 0"<<endl;
//	}
	if(data[index->dimension_array[i][j]] > index->threshold_array[i][j])
		return 1;
	else
		return 0;
}
void hash_key(ThresholdIndex *index, string& hk, float* data, int group)
{
	hk = "";
	cout <<"hash_key POSITION BEGIN"<<endl;
	for(int i = 0; i < index->M; i++)
	{
		cout <<"hash_key POSITION 0.1"<<endl;
		cout <<"i = "<<i<<endl;
		hk += to_string(hash_value(index, data, group, i));
		cout <<"hash_key POSITION 0.2"<<endl;
	}
	cout <<"hash_key POSITION END"<<endl;
}

float** query(ThresholdIndex *index, float* data, int N, int* res_number)
{
	float**	res = new float*[N];
	unordered_map<string, unordered_map<float*, float*> >::iterator local_ite;
	unordered_map<float*, float*>::iterator local_local_ite;
	int query_counter = 0;
	for(int i = 0; i < index->L; i++)
	{
//		cout << "query POSITION 0.1"<<endl;
		unordered_map<string, unordered_map<float*, float*> >& local_bucket_values = index->bucket_values[i];
		string hk = "";
		hash_key(index, hk, data, i);
		local_ite = local_bucket_values.find(hk);
//		cout << "query POSITION 0.2"<<endl;
		if(local_ite != local_bucket_values.end())
		{
			for(	local_local_ite = local_ite->second.begin();
					local_local_ite != local_ite->second.end();
					local_local_ite++)
			{
//				cout << "query POSITION 0.2.1"<<endl;
				cout << "local_local_ite->second = "<<local_local_ite->second<<endl;
				res[query_counter] = local_local_ite->second;
//				cout << "query POSITION 0.2.2"<<endl;
				query_counter += 1;
//				cout << "query POSITION 0.2.3"<<endl;
				if(query_counter >= N)
				{
//					cout << "query POSITION 0.2.4"<<endl;
					res_number[0] = N;
//					cout << "res[query_counter -1] = "<<res[query_counter -1]<<endl;
//					cout << "query POSITION 0.2.5"<<endl;
					return res;
				}
			}
		}
	}
//	cout << "query POSITION 1"<<endl;
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
