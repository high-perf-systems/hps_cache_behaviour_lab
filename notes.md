Important points to remember :

Upon getting hw data from the computer, we see that the code and benchmark is done on Apple m2 silicon which uses arm64 architecture. 
there are some nuances associated with the m2 silicon, the cache is not a traditional hierarchy like the x86 systems. there is a slc (system level cache) and l1, l2 caches. 
Also, the CPU cores are split into efficiency cores (E-cluster) and Performance cores (P-cluster) each of which have different frequency limits. 
We do not enforce control over which CPU or which cache is used. So, we acknowledge that the results are not completely reproducible every time. we are not interested in the 
absolute numbers, we are only interested in the relative trends as to how the performance changes when there is cache hit vs cache miss , difference in performance seen with changes in 
data layout etc.

Compiler flags:
For this project, we typically add a few compiler flags to benchmark the differences between memory access patterns properly without the m2 cpu playing tricks
1. O2 - optimized enough to remove artifacts, but not aggressive enough like O3 to unroll loops, remove entire loops etc.
2. no-vectorize - compiler's auto vectorization of loops is disabled to prevent it interfering with our analyses. 
3. no-slp-vectorize - compiler's superword level parallelism disabled.
To prevent the compiler from removing loops or dead code, we also volatile where required.

Warmup :
In our code studying memory access patterns, we include warm-up code which pushes the machine's CPU to a steady-state, so that we get the steady-state runtime and not cold start runtime. The warm-up does many things, it allows the OS to populate page tables, page faults, TLB entries are created etc. Also, the hardware pre-fetchers are hot, caches are active and do not need to be warmed up during the computations. this specially affects the sequential access pattern code. Also, this helps power stabilization and the cpu to reach a steady clock frequency starting from usually lower frequency. 

Random vs Sequential memory access pattern raw results : 

In both the scenarios, the objective was to compute the sum of numbers from 0 to N. In sequential access, the sum was computed iterating from indices 0 to N,
sequentially whereas in the case of random access pattern, the indices were shuffled using a random number generator algorithm and summation iterated over the 
shuffled indices.

The benchmark results were collected for 4 values of N keeping it in line with the approximate estimated cache size available and code was executed for 5 times
and the result was estimated.

N : 128 kb

Sequential                          Random
Sum = 134209536                     Sum = 134209536
Min = 1.4708 e^-5                   Min = 1.7708 e^-5
Max = 1.8917 e^-5                   Max = 2.133 e^-5
Avg = 1.6866 e^-5                   Avg = 1,9998 e^-5

N : 2 MB
Sequential                          Random 
Sum = 34359607296                   Sum = 34359607296 
Min = 0.00020195                    Min = 0.0004025
Max = 0.00029004                    Max = 0.000447125
Avg = 0.00025207                    Avg = 0.00042508

N : 8MB
Sequential                          Random 
Sum = 549755289600                  Sum = 549755289600 
Min = 0.000796458                   Min = 0.0011705
Max = 0.00122488                    Max = 0.001976
Avg = 0.00104155                    Avg = 0.001592

N : 64MB
Sequential                          Random 
Sum = 35184367894528                Sum = 35184367894528 
Min = 0.00483                       Min = 0.02142
Max = 0.00577                       Max = 0.02264
Avg = 0.00506                       Avg = 0.02190

Observations and inferences from the experiment :

1. Our l1 data cache is 64 kb and l2 cache is around 2 MB. with the dataset being 128 kb, for both random and sequential access, the data does not fit in 
l1 cache. but they both fit in l2 cache. So, there is no need to touch DRAM and Translation lookup buffer is not pressurised much to read the physical memory mapping from the virtual memory. page table entries are already done and there is no need to disturb them. The interesting idea here is that random access is not always slow, if the cache size is ample,both sequential and random give the same performance. Cache size matters more than pattern alone.

2. With N = 2MB , we overflow L2 cache and within the limits of L3 cache. the random access pattern is 2x slower than sequential. We are now stressing L3 cache, TLB capacity, hardware prefetchers. Sequential is faster because, hardware prefetchers have already fetched data, cache lines are fully utilized and there is very little TLB misses. In the case of random access, the TLB misses are high, thus page walks are required to be performed and this causes delay. prefetcher is mostly useless, and all this is due to poor spatial locality.

3. with N = 8MB, we see a 1.5 x speedup of sequential pattern over random. in this case, for both the cases, the memory perhaps slightly exceeds the l3 cache. both the patterns suffer misses since they exceed cache sizes. DRAM  latency dominates and hence even with the sequential pattern, the advantage is diluted due to DRAM latency dominance. 

4. N = 64MB, here we see a whopping 4x speedup of sequential pattern over random in spite of 64 MB clearly exceeding the cache sizes. The advantage of spatial locality is no more limited to the caches alone. the speedup is due to multiple factors. hardware prefetching works better with spatial locality. the hardware prefetchers request memory early and keep them ready. with sequential pattern, this can be used, but with random access pattern, the prefetched memory is discarded. each access stalls. Even when getting memory from DRAM, sequential access reuses DRAM rows and there are fewer activations. in case of random access, there is row thrashing, high DRAM penalty. TLB thrashing : TLB usually has 100-300 entries , with sequential access, the TLB stores the mapping of next 100-300 entries and these can be reused. but with random access, the TLB entries cannot be reused and they need to be updated with every new index requested. page walks are more common in random access. Memory Level Parallelism (MLP) also plays a role here, it is the mechanism by which a processor sends multiple memory requests concurrently. it needs some hardware enablers to work properly, like look ahead of registers etc. with sequential access, the misses are predictable and thus the cpu can pipeline loads, but with random access, the misses are unpredictable, the cpu cannot pipeline load and thus slows down.

Conclusions from experiment 1 : 
The experiment demonstrates that memory access performance is governed not only by cache capacity but by a combination of spatial locality, hardware prefetching effectiveness, TLB behavior, and DRAM-level locality. Sequential access patterns benefit from prefetching and predictable page access even when the working set exceeds cache sizes, leading to near-linear scaling. Random access patterns degrade sharply once working sets exceed cache and TLB capacities, causing increased TLB misses, page table walks, and inefficient DRAM access.
