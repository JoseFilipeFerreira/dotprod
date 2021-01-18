# AA - dotprod
* Jose Ferreira - a83683
* Joao Teixeira - a85504

# Performance Measuring

## Matrix Size
|            | L1 data | L2 data | L3 data  | RAM       |
|:-----------|:-------:|:-------:|:--------:|:---------:|
| cache size |  32768  | 262144  | 31457280 | >31457280 |
| max size   |  52     | 147     | 1619     | >1619     |
| used size  |  40     | 120     | 1500     | 4000      |

Formula: N²\*3\*4 = size

## execution time - CPU
| type      | L1 data | L2 data | L3 data | RAM       |
|:----------|:-------:|:-------:|:-------:|:---------:|
| ijk       | 66      | 1733    | 4239493 | 177553349 |
| ijk trans |         |         |         |           |
| ikj       | 71      | 1718    | 3194044 | 61695968  |
| jki       | 72      | 1744    | 8596300 | 324751067 |
| jki trans | 69      | 1375    | 2273077 | 45375529  |
time in μs

### RAM accessses
| type      | L1 data    | L2 data       | L3 data            | RAM                     |
|:----------|:----------:|:-------------:|:------------------:|:-----------------------:|
| ijk       | 7/529759   | 8/13962496    | 157510/27021113743 | 3999942878/512150216402 |
| ijk trans |            |               |                    |                         |
| ikj       | 125/460398 | 885/12203520  | 61438/23641610705  | 138308042/448118098445  |
| jki       | 192/525999 | 1172/13945920 | 424240/27018866530 | 4024883519/512134361578 |
| jki trans | 164/480100 | 1059/12377782 | 213621/23668627809 | 241300131/448310130045  |

Formula: L3 misses / Total instructions

## execution time - CPU
| type                 | L1 data | L2 data | L3 data | RAM      |
|:---------------------|:-------:|:-------:|:-------:|:--------:|
| block                | -       | -       | -       | 64385275 |
| block + vec          |         |         | -       | -        |
| block + vec + OpenMP | -       | -       | -       | 2407551  |
time in μs

## execution time - GPU
| type | L1 data | L2 data | L3 data | RAM |
|:-----|:-------:|:-------:|:-------:|:---:|
| CUDA | -       | -       | -       |     |
time in μs

# roofline graph

# global% miss rate

## global% miss rate L1

| type      | L1 data | L2 data | L3 data | RAM |
|:----------|:-------:|:-------:|:-------:|:---:|
| ijk       |         |         |         |     |
| ijk trans |         |         |         |     |
| jki       |         |         |         |     |
| jki trans |         |         |         |     |

Formula: 100\*L1 misses / Total accessses

## global% miss rate L2

| type      | L1 data | L2 data | L3 data | RAM |
|:----------|:-------:|:-------:|:-------:|:---:|
| ijk       |         |         |         |     |
| ijk trans |         |         |         |     |
| jki       |         |         |         |     |
| jki trans |         |         |         |     |

Formula: 100\*L2 misses / Total accessses

## global% miss rate L3

| type      | L1 data | L2 data | L3 data | RAM |
|:----------|:-------:|:-------:|:-------:|:---:|
| ijk       |         |         |         |     |
| ijk trans |         |         |         |     |
| jki       |         |         |         |     |
| jki trans |         |         |         |     |

Formula: 100\*L3 misses / Total accessses
