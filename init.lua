local ffi = require "ffi"
require 'torchffi'
local async = require 'async'

madlsh = {}
ffi.cdef
[[ 
    typedef void ThresholdIndex;
    
    ThresholdIndex *init(int l, int m, int data_dim, float* min_vect, float* max_vect);
    float** query(ThresholdIndex *index, float* data, int N, int* res_number);
    void add_to_group(ThresholdIndex *index, float* data, int group);
    void insert(ThresholdIndex *index, float* data);
    char* hash_key(ThresholdIndex *index, float* data, int group);
    float** test();
]]

torch.setdefaulttensortype("torch.FloatTensor")

local luash = {}

madlsh.clib = ffi.load("./libmadlsh.so")
madlsh.getDataTensor = function(similarityTable)
   local N = similarityTable.public_vectors:size(1)
   local dim = similarityTable.public_vectors:size(2)

   local data_tensor = torch.FloatTensor(dim * N):copy(similarityTable.public_vectors):resize(similarityTable.public_vectors:size())
   local multipliers = similarityTable.public_multipliers

   multipliers:resize(N,1)

   data_tensor:cmul(multipliers:expandAs(data_tensor))

   
   return data_tensor, similarityTable
end

madlsh.buildThresholdLSHStatistics = function(data_tensor)

   local zeroPercentageVector = torch.FloatTensor(data_tensor:size(2) * data_tensor:size(1)):copy(torch.gt(data_tensor, 0.)):resize(data_tensor:size())
   -- print("zeroPercentageVector ", zeroPercentageVector[{{1,100},{}}])
   local data_tensor_t = data_tensor:t()
   local mean_tensor = data_tensor_t:mean(2)
   local mean_tensor = torch.FloatTensor(mean_tensor:size(1)):copy(mean_tensor[{{},1}])
   local std_tensor = data_tensor_t:std(2)
   local std_tensor = torch.FloatTensor(std_tensor:size(1)):copy(std_tensor[{{},1}])
--   zeroPercentageVector = zeroPercentageVector:t()
   -- print("zeroPercentageVector:sum() ", zeroPercentageVector:sum(2))
--   print("Non Zero Percentage ", zeroPercentageVector:sum(2))
--   local nonZeroPercentageVectorByFeature = torch.div(zeroPercentageVector:sum(2),zeroPercentageVector:size(2))
--   nonZeroPercentageVectorByFeature = torch.FloatTensor(nonZeroPercentageVectorByFeature:size(1)):copy(nonZeroPercentageVectorByFeature[{{},1}])
--   print("Non Zero Percentage By Feature", nonZeroPercentageVectorByFeature)
--   print("zeroPercentageVector:size() ", zeroPercentageVector:size())
--   print("Global Non Zero Percentage ", zeroPercentageVector:sum()/(zeroPercentageVector:size(1)*zeroPercentageVector:size(2)))
--   print("MAX Non Zero Percentage ", nonZeroPercentageVectorByFeature:max())
--   print("MIN Non Zero Percentage ", nonZeroPercentageVectorByFeature:min())
--   print("data_tensor:size() = ", data_tensor:size())
--   print("similarityTable.public_multipliers ", similarityTable.public_multipliers)
   local result = {}
   result.mean_tensor = mean_tensor
   result.std_tensor = std_tensor
   result.min_vector = torch.FloatTensor(4096):fill(0.)
   result.max_vector = torch.add(mean_tensor, torch.mul(std_tensor, 2.))
   return result
end



--print("Loading similarity table")
--local similarityTable = torch.load('./SimilarityTable.1.m')
--   
--local N = similarityTable.public_vectors:size(1)
--local dataTensor = getDataTensor(similarityTable)
--local stats = buildThresholdLSHStatistics(dataTensor)
--
--local dim = dataTensor:size(2)
--local L = 1
--local M = 20
--local min_vect = stats.min_vector
--local max_vect = stats.max_vector
--
--local index = clib.init(L, M, dim, torch.data(min_vect), torch.data(max_vect))
--local res_number = torch.IntTensor(1):fill(0)
--
--print("inserting data load...")
--for i=1, N do
--   clib.insert(index, torch.data(dataTensor[i]))
--end
--print("insertion performed !")
--
--print("performing query...")
--local hk = ""
--
--qres = clib.query(index, torch.data(dataTensor[1]), 100000, torch.data(res_number))
--print("query performed !")
--
--for i=0, res_number[1]-1 do
--  local ptr = tonumber( ffi.cast('intptr_t', qres[i]) )
--  s = torch.Storage(dim,ptr)
----  print("s = ", s)
--end
--print("res_number[1] = ", res_number[1])



---- Insert the random load
--for i=1, test_number do
--   local_matrix[i] = torch.rand(dim)
--   clib.insert(index, torch.data(local_matrix[i]))
--end
--
--
--test_res = clib.test()
--local ptr = tonumber( ffi.cast('intptr_t', test_res[0]) )
--
--
--qres = clib.query(index, torch.data(local_matrix[1]), 10000, torch.data(res_number))
--
--print("local_matrix[1] = ", local_matrix[1])
--for i=0, res_number[1]-1 do
--  local ptr = tonumber( ffi.cast('intptr_t', qres[i]) )
--  s = torch.Storage(dim,ptr)
--  print("s = ", s)
--end
--print("res_number[1] = ", res_number[1])


return madlsh