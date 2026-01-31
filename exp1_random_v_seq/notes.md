# Experiment 1: Sequential vs. Random Memory Access

## Objective

This experiment investigates the performance implications of sequential versus random memory access patterns. The goal is to understand how cache hierarchies, hardware prefetchers, and memory subsystems interact with different access patterns, and how performance characteristics change as the data set size grows.

## Experimental Setup

### Hardware and Environment

The benchmarks were executed on Apple M2 silicon (ARM64 architecture). Key characteristics of this platform include:

-   **Heterogeneous Cores:** The CPU comprises both Performance-cores (P-cluster) and Efficiency-cores (E-cluster).
-   **System Level Cache (SLC):** A shared cache for all cores.
-   **Non-traditional Cache Hierarchy:** The L1/L2 cache structure differs from typical x86 systems.

Due to the lack of fine-grained control over core selection and cache usage, the absolute performance numbers are not the focus. Instead, this analysis emphasizes the *relative performance trends* between the two access patterns.

### Compiler Configuration

To ensure a fair comparison and prevent compiler optimizations from obscuring the memory access behavior, the following flags were used:

-   `-O2`: Provides a good balance of optimization without aggressive loop transformations (like unrolling) that could alter the access patterns.
-   `-fno-vectorize`: Disables auto-vectorization.
-   `-fno-slp-vectorize`: Disables superword-level parallelism.
-   `volatile`: Used where necessary to prevent the compiler from optimizing away loops or memory accesses.

### Warm-up Phase

A warm-up phase is included in the benchmark code to ensure the system reaches a steady state before measurements are taken. This helps to:

-   Populate page tables and TLB entries.
-   Activate hardware prefetchers.
-   Allow the CPU to reach a stable clock frequency.

## Experiment Description

The experiment calculates the sum of a large array of numbers. Two scenarios were tested:

1.  **Sequential Access:** The array is traversed linearly from index 0 to N.
2.  **Random Access:** The array indices are shuffled, and the array is traversed using the shuffled indices.

The benchmarks were run for several values of `N`, chosen to be in line with the estimated cache sizes of the test machine.

## Results

The following table summarizes the minimum, maximum, and average execution times for both access patterns across different data set sizes.

| Data Size (N) | Access Pattern | Min Time (s)  | Max Time (s)  | Avg Time (s)  |
| :------------ | :------------- | :------------ | :------------ | :------------ |
| **128 KB**    | Sequential     | 1.4708e-05    | 1.8917e-05    | 1.6866e-05    |
|               | Random         | 1.7708e-05    | 2.133e-05     | 1.9998e-05    |
| **2 MB**      | Sequential     | 0.00020195    | 0.00029004    | 0.00025207    |
|               | Random         | 0.0004025     | 0.000447125   | 0.00042508    |
| **8 MB**      | Sequential     | 0.000796458   | 0.00122488    | 0.00104155    |
|               | Random         | 0.0011705     | 0.001976      | 0.001592      |
| **64 MB**     | Sequential     | 0.00483       | 0.00577       | 0.00506       |
|               | Random         | 0.02142       | 0.02264       | 0.02190       |

## Analysis and Inferences

1.  **N = 128 KB:** The data set is larger than the L1 data cache (64 KB) but fits within the L2 cache. Both access patterns exhibit similar performance because the data resides entirely in the cache, and there is no need to access DRAM. This demonstrates that when the working set fits in a lower-level cache, the access pattern has a minimal impact.

2.  **N = 2 MB:** The data set overflows the L2 cache, stressing the L3 cache and the Translation Lookaside Buffer (TLB). The random access pattern is approximately **2x slower** than the sequential pattern. This is due to:
    -   **Hardware Prefetcher Inefficiency:** The prefetcher cannot predict the next memory access for the random pattern.
    -   **Poor Spatial Locality:** Cache lines are not fully utilized.
    -   **TLB Misses:** The random access pattern leads to a higher rate of TLB misses, requiring more frequent and costly page table walks.

3.  **N = 8 MB:** The sequential pattern is about **1.5x faster** than the random pattern. Both patterns now exceed the L3 cache size, and DRAM latency becomes a more significant factor for both. The advantage of sequential access is somewhat diluted because both patterns are now frequently accessing main memory.

4.  **N = 64 MB:** The sequential pattern is a significant **4x faster** than the random pattern. Even though the data set is much larger than the cache, the benefits of spatial locality extend to the DRAM subsystem:
    -   **Hardware Prefetching:** The prefetcher continues to be effective for the sequential pattern, hiding some of the DRAM latency.
    -   **DRAM Row Locality:** Sequential access patterns are more likely to hit open DRAM rows, reducing activation penalties. Random accesses can cause "row thrashing."
    -   **TLB Efficiency:** The TLB is more effective for sequential access, as it can cache translations for contiguous pages. Random access leads to TLB thrashing and frequent page walks.
    -   **Memory-Level Parallelism (MLP):** The CPU can pipeline memory requests more effectively for the predictable sequential access pattern, hiding latency. This is not possible for the unpredictable random access pattern.

## Conclusion

This experiment demonstrates that memory access performance is a complex interplay of cache capacity, spatial locality, hardware prefetching, TLB behavior, and DRAM characteristics.

-   **Sequential access** scales well, even when the data set exceeds cache sizes, due to the benefits of hardware prefetching and locality at both the cache and DRAM levels.
-   **Random access** performance degrades sharply when the working set exceeds the cache and TLB capacities. This is primarily due to TLB thrashing, inefficient prefetching, and poor DRAM locality.