require 'init'

require 'unsup'
torch.setdefaulttensortype('torch.FloatTensor')

-- memory map files:
local multipliers = torch.FloatTensor(torch.FloatStorage('multipliers_map.raw'))
local dim = 4096
local nvectors = multipliers:size(1)
local vectors = torch.ByteTensor( torch.ByteStorage('vectors_map.raw') ):resize(nvectors, dim)

-- data obtained:
print('complete dataset:')
print{vectors,multipliers}

-- function to get a vector:
function get(idx, dst)
   local tmp = dst or torch.FloatTensor(dim)
   tmp:copy( vectors[idx] )
   tmp:mul( multipliers[idx] )
   return tmp
end

-- get example vectors:
vector1 = get(1)
vector2 = get(2)
print('example vectors:')
print{vector1,vector2}

-- compute PCA whitening transform on subset of vectors:
local npca = 1e4
local dataTensor = torch.FloatTensor(npca,dim)
for i = 1,npca do
   get(i, dataTensor[i])
end

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