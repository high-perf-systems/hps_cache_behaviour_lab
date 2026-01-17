Problem Statement: 
Modern high performance systems are heavily bottlenecked by memory subsystems. CPUs in modern computers can handle billions of instructions
per second, but the latency and bandwidth limitations introduced by memory subsystems dominate execution time in real world applications.

This project investigates how different memory access patterns influence operations in the modern CPUs. It does not concern itself with the 
algorithmic complexity, instead the core focus is on the data layout and memory access patterns and how these interact with the CPU cache
hierarchy.

Specifically, the following points are addressed : 
1. The performance difference in sequential memory access pattern and random memory access pattern.
2. Impact of strided memory access on CPU cache utilization.
3. difference in performance of Array of Structures (AoS) and Structure of Array (SoA) memory layouts.
4. Study in difference of performances with a relative difference in working set size with respect to the cache limits.

Hypotheses (Before experiments):
1. Sequential memory access patterns are better for performance due to CPU cache utilization and effective hardware pre-fetching
2. With increase in stride length, the performance worsens due to non-utilization of the cache.
3. SoA will outperform AoS when accessing a subset of data due to better spatial locality.
4. With the working set size increased beyond the L1, L2, L3 cache sizes, due to cache misses, a deterioration in performance is expected.

Scope : 
The scope of the project is limited to : 
1. Single threaded CPU execution (No multi-threading or GPU or accelerators)
2. Synthetic mirco-benchmarks designed to isolate memory behaviour.
NUMA, GPU execution, compiler auto-vectorization, SIMD vectorization, effects related to multi-threading are explicitly out of scope for this 
project and will be explored in later projects.

Goal : 
The goal of this project is to build intuition, grounded in empirical evidence, about how memory access patterns influence performance, and to 
reconcile the experimental results with well established theoretical foundations in cache behaviour.
