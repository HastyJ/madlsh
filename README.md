madlsh
======

Implement a custom threshold LSH implementation for similarity search of real-valued vector. 
This LSH implementation is supposed to approximate the L1 norm for real vectors 
(http://www.cs.princeton.edu/cass/papers/sketch_sigmetrics07.pdf and  http://www.cs.princeton.edu/cass/papers/LCL04.pdf).
A basic hash function h in this context is the random selection of both a dimension i and a threshold T. 
For every input vector V, if V[i] >= T, then h(V) = 1, and 0 otherwise.

License
-------

MIT License

Examples
--------

Load a table of real vectors (init contains the C interface and a few utilities): 
```lua
require 'init'
local similarityTable = torch.load('./SimilarityTable.1.m')
```
Compute some statistics on it or a portion of it, including the minvector and the maxvector:

```lua
local dataTensor = madlsh.getDataTensor(similarityTable)
local N = dataTensor:size(1)
local stats = madlsh.buildThresholdLSHStatistics(dataTensor)

local dim = dataTensor:size(2)
local max_vect = stats.max_vector
```
Create a ThresholdIndex: 

```lua
local L = 50
local M = 40
local min_vect = stats.min_vector
local index = madlsh.clib.init(L, M, dim, min_vect, max_vect)
```
This will create the following index : 
* L amplification functions defined by M basic hash functions each, i.e. [h^1_1, ..., h^1_M], ..., [h^L_1, ..., h^L_M].
* h^j_i each has a randomly chosen dimension and a randomly chosen threshold.
 
