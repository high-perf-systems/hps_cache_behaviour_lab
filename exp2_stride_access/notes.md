# Experiment 2: Stride, Cache Lines, and Hardware Prefetchers

## Objective

This experiment aims to deconstruct the meaning of "sequential access" from a modern CPU's perspective. By systematically increasing the memory access stride while keeping the total memory footprint constant, we can identify when and why the benefits of sequential access diminish. The goal is to observe how the system bottleneck shifts from CPU execution to cache bandwidth, and finally to address translation (TLB) limits.

## Experimental Setup

### Hardware and Environment

The benchmarks were executed on Apple M2 silicon (ARM64 architecture), which features:

-   **Heterogeneous Cores:** A mix of Performance-cores (P-cluster) and Efficiency-cores (E-cluster).
-   **System Level Cache (SLC):** A shared cache for all cores.

As with the previous experiment, we do not control core selection. Therefore, the analysis focuses on *relative performance trends* rather than absolute timings.

### Compiler Configuration

To isolate the effects of memory access, the following compiler flags were used:

-   `-O2`: Standard optimizations without aggressive, potentially confounding transformations.
-   `-fno-vectorize`: Disables automatic loop vectorization (SIMD).
-   `-fno-slp-vectorize`: Disables superword-level parallelism.
-   `volatile`: Prevents dead-code elimination.

### Experimental Invariants

To ensure a clean and fair experiment, the following conditions were maintained:

1.  **Constant Memory Footprint:** The total number of bytes accessed is the same for all stride values.
2.  **Controlled Compiler Behavior:** Vectorization and dead-code elimination are disabled.
3.  **Warm-up Phase:** A warm-up period is used to mitigate cold-start effects in caches and TLBs.
4.  **Averaged Results:** Multiple iterations are run, and the results are averaged to reduce noise.

## Experiment Description

The benchmark iterates over a large array, summing its elements. The stride of access is varied across the set {1, 2, 4, 8, 16, 32, 64, 128}. All other parameters are held constant.

## Results

| Stride | Time (s)     |
| :----- | :----------- |
| 1      | 0.0109287    |
| 2      | 0.00971154   |
| 4      | 0.00979238   |
| 8      | 0.00965863   |
| 16     | 0.0171271    |
| 32     | 0.0403845    |
| 64     | 0.0384875    |
| 128    | 0.0378612    |

## Analysis and Inferences

### Strides 1, 2, 4, 8: The CPU-Bound Regime

At small strides, the accesses are confined to a single or adjacent cache lines.

-   **Efficient Cache Usage:** Cache lines are reused effectively.
-   **Predictable Access:** Hardware prefetchers work perfectly.
-   **High TLB Hit Rate:** The TLB hit rate is near 100%.
-   **Data in L1/L2:** Data is almost always available in the L1 or L2 cache.

In this regime, the system bottleneck is the **CPU's execution throughput**, not memory access. The slightly higher runtime for a stride of 1 is likely due to instruction retirement pressure or load/store queue contention, not a memory issue.

### Stride 16: The Cache Bandwidth-Bound Transition

At a stride of 16, a transition occurs:

-   **Reduced Cache Line Utilization:** Each access uses only a fraction of the fetched cache line.
-   **Increased Bandwidth Demand:** While latency is still hidden by prefetching, the demand on L1/L2 cache bandwidth increases significantly as more cache lines are fetched.
-   **LFB Saturation:** The Line Fill Buffers (LFBs), which handle outstanding cache misses, may become saturated.

This marks the shift from a CPU-bound to a **cache-bandwidth-bound** workload.

### Strides ≥ 32: The TLB and Memory-Subsystem Bound Regime

At larger strides, the performance bottleneck shifts again:

-   **TLB Misses:** Each access is more likely to fall on a new virtual page, exceeding the TLB's capacity and causing frequent TLB misses.
-   **Page Table Walks:** Each TLB miss triggers a costly page table walk, which involves multiple memory accesses.

The dominant cost is no longer cache-line utilization but **address translation latency**. The CPU spends an increasing amount of time stalled, waiting for the memory management unit (MMU) to translate virtual to physical addresses.

## Key Takeaways

1.  **Sequential access is not a monolithic concept.** Its benefits degrade as different subsystems (CPU, cache, MMU) become the bottleneck.
2.  **Performance collapses in distinct phases:** from CPU-bound to cache-bandwidth-bound, and finally to TLB/memory-bound.
3.  **Bandwidth and buffer exhaustion often precede latency issues.** Performance breaks due to saturated buffers (like LFBs) or translation overhead before raw DRAM latency becomes the primary problem.
4.  **Holistic system reasoning is crucial.** Optimizing one part of the system in isolation can lead to incorrect conclusions.

## Final Insight

This experiment reinforces a core principle of systems performance engineering: **performance tuning is about identifying and alleviating bottlenecks, not about micro-optimization.**