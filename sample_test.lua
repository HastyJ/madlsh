require 'init'


print("Loading similarity table")
local similarityTable = torch.load('./SimilarityTable.1.m')
   
local dataTensor = madlsh.getDataTensor(similarityTable)
local N = dataTensor:size(1)
local stats = madlsh.buildThresholdLSHStatistics(dataTensor)

local dim = dataTensor:size(2)
local L = 1
local M = 20
local min_vect = stats.min_vector
local max_vect = stats.max_vector

local index = madlsh.clib.init(L, M, dim, torch.data(min_vect), torch.data(max_vect))
local res_number = torch.IntTensor(1):fill(0)

print("inserting data load...")
for i=1, N do
   madlsh.clib.insert(index, torch.data(dataTensor[i]))
end
print("insertion performed !")

print("performing query...")
local hk = ""

qres = madlsh.clib.query(index, torch.data(dataTensor[1]), 100000, torch.data(res_number))
print("query performed !")

for i=0, res_number[1]-1 do
  local ptr = tonumber( ffi.cast('intptr_t', qres[i]) )
  s = torch.Storage(dim,ptr)
--  print("s = ", s)
end
print("res_number[1] = ", res_number[1])