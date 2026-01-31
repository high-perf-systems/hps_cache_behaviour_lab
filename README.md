# hps_cache_behaviour_lab
Empirical Exploration of Cache Behaviour via Memory Access Pattern

Motivation : 
High performance systems like AI inference engines, robotics frameworks, databases , search engines fail or succeed on memory 
behaviour and not algorithms.
Modern CPUs are fast, memory systems are slow. Understanding caches becomes critical to build and maintain reliable high performance systems.

Core question of the project : 
How does memory layout affect performance and why?

This project is an attempt to answer the question above and in the process record data, code and benchmarking results.

## Summary of Experiments

This repository contains a series of experiments designed to explore the impact of memory access patterns on application performance.

### [Experiment 1: Sequential vs. Random Memory Access](./exp1_random_v_seq/notes.md)

-   **Objective:** To quantify the performance difference between sequential and random memory access patterns as the data set size grows beyond the capacity of the CPU caches.
-   **Key Takeaway:** Sequential access consistently outperforms random access, especially with large data sets. The benefits of sequential access extend beyond the cache hierarchy to the DRAM subsystem, thanks to hardware prefetchers, DRAM row locality, and more efficient TLB usage.

### [Experiment 2: The Impact of Memory Stride](./exp2_stride_access/notes.md)

-   **Objective:** To understand how the performance of sequential access degrades as the access stride increases.
-   **Key Takeaway:** The performance of strided access collapses in distinct phases. The bottleneck shifts from being CPU-bound to cache-bandwidth-bound, and finally to being limited by TLB misses and page walks as the stride increases. This demonstrates that "sequential" access is not a binary concept.

### [Experiment 3: Array of Structures (AoS) vs. Structure of Arrays (SoA)](./exp3_AoSvSoA/notes.md)

-   **Objective:** To compare the performance of AoS and SoA data layouts, particularly in scenarios with partial data access.
-   **Key Takeaway:** The performance difference between AoS and SoA is primarily determined by cache-line utilization. SoA is significantly faster when accessing a subset of data fields if the AoS layout would cause cache-line waste (i.e., loading unused data). If the accessed fields in an AoS layout are contiguous and fit within a single cache line, the performance is comparable to SoA.