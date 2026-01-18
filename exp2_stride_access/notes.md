Project 2 — Stride, Cache Lines, and Hardware Prefetchers

Objective

The goal of this experiment is to understand what “sequential access” means to a modern CPU, and more importantly, when and why it stops helping.

This project studies how changing the stride of memory access—while keeping the total memory footprint constant—affects performance, and how the system bottleneck shifts across:

CPU execution limits

Cache bandwidth limits

Address translation (TLB) limits

Experimental Context

Similar to Project 1, we do not enforce control over:

Which CPU core executes the program

Whether execution happens on performance or efficiency cores (Apple Silicon)

Apple Silicon has heterogeneous cores (P-cluster and E-cluster) with different frequency limits and cache structures.
Absolute timings are therefore not meaningful across runs or devices.

The focus is strictly on:

Relative performance trends within the same execution environment.

Compiler Configuration and Rationale

The compiler can significantly distort microbenchmarks if not constrained carefully.
We use the following flags:

-O2
Optimized enough to remove artifacts, but avoids aggressive transformations such as excessive unrolling or speculative restructuring seen in -O3.

-fno-vectorize
Disables automatic loop vectorization to prevent SIMD from masking memory effects.

-fno-slp-vectorize
Disables superword-level parallelism for the same reason.

Additionally:

volatile is used where necessary to prevent dead-code elimination.

A warm-up phase is included to bring caches, TLBs, and page tables into a steady state.

Experimental Invariants (Non-Negotiable)

To ensure the experiment is uncontaminated:

1. Constant memory footprint
All stride values must touch the same total number of bytes.

2. Controlled compiler behavior
No vectorization, no dead-code elimination.

3. Warm-up phase
Avoid cold-start effects in caches and TLBs.

4. Multiple iterations
Average results are used instead of relying on single measurements.

Experiment Description : 

In stride.cpp, we iterate over a large array using increasing strides : stride ∈ {1, 2, 4, 8, 16, 32, 64, 128}
All other parameters are kept constant.

```
Stride = 1   Time = 0.0109287
Stride = 2   Time = 0.00971154
Stride = 4   Time = 0.00979238
Stride = 8   Time = 0.00965863
Stride = 16  Time = 0.0171271
Stride = 32  Time = 0.0403845
Stride = 64  Time = 0.0384875
Stride = 128 Time = 0.0378612
```

Analysis and Inferences

Stride = 1, 2, 4, 8 → CPU-Bound Regime

These strides fall within a single cache line or nearby cache lines.

Key observations:

Cache lines are reused efficiently

Hardware prefetchers accurately predict access patterns

TLB hit rate is effectively 100%

Data is almost always available in L1/L2

The system bottleneck here is CPU execution throughput, not memory.

The slightly higher runtime for stride = 1 compared to 2/4/8 is likely due to:

Higher instruction retirement pressure

Load/store queue utilization

Backend execution limits

This is not a memory problem.

Stride = 16 → Cache Bandwidth-Bound Transition

At stride = 16:

Each cache line contains many elements, but only one is used

Cache line utilization drops sharply

Hardware prefetchers still fetch aggressively

Latency is mostly hidden

However:

L1/L2 bandwidth demand increases

Line Fill Buffers (LFBs) become saturated

Cache hierarchy throughput becomes the limiting factor

This marks the transition from CPU-bound to cache-bandwidth-bound execution.

Stride ≥ 32 → TLB and Memory-Subsystem Bound

At higher strides:

Each access frequently touches a new virtual page

TLB capacity is exceeded

TLB misses become common

Page table walks are triggered

Important distinction:

The dominant cost is no longer cache-line utilization

The dominant cost is address translation latency and serialization

Each page walk:

Consumes memory bandwidth

Pollutes caches

Stalls dependent loads

The CPU increasingly stalls waiting for valid physical addresses, leading to a sharp performance collapse.

Key Takeaways

Sequential access is not binary
It helps until a specific subsystem becomes the bottleneck.

Performance collapses in phases

CPU execution limits

Cache bandwidth saturation

TLB and page walk overheads

Latency is not the first failure mode
Bandwidth and buffer exhaustion usually break performance before raw latency becomes visible.

Systems must be reasoned about holistically
Optimizing one layer while ignoring others leads to incorrect conclusions.

Final Insight

This experiment reinforces a core systems principle:

Performance tuning is bottleneck hunting, not micro-optimization.
