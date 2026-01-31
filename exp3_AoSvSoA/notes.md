# Experiment 3: Array of Structures (AoS) vs. Structure of Arrays (SoA)

## Objective

This experiment investigates the performance trade-offs between two common data layout patterns: Array of Structures (AoS) and Structure of Arrays (SoA). The goal is to measure how data layout affects performance when:

1.  All fields of a structure are accessed.
2.  Only a subset of fields is accessed.

The key question is how cache-line utilization, memory bandwidth, and even CPU instruction processing are influenced by the choice of data layout.

## Hypotheses

1.  **AoS is optimal for full-field access:** When all fields in a structure are used, AoS should perform well due to good spatial locality.
2.  **SoA is optimal for partial access:** When only a subset of fields is used, SoA should be more efficient by avoiding the loading of unnecessary data into the cache.
3.  **Performance differences amplify with scale:** The performance gap between AoS and SoA should widen as the data set size increases, saturating successive levels of the cache hierarchy.

---

## Experiment 3A: Partial Access (Baseline)

In this first experiment, we test the baseline performance of AoS vs. SoA with partial data access where the accessed fields are contiguous in memory.

### Setup

-   **Structure Size:** 24 bytes per element.
-   **Accessed Fields:** Two fields (`x` and `vy`) are accessed in each iteration.
-   **Access Pattern:** Sequential access.

### Results

| N       | Layout | Time (s)      |
| :------ | :----- | :------------ |
| 2,730    | AoS    | 7.0e-06       |
|         | SoA    | 6.959e-06     |
| 87,381   | AoS    | 0.000220834   |
|         | SoA    | 0.000220667   |
| 2,796,202 | AoS    | 0.003736      |
|         | SoA    | 0.00348333    |

### Interpretation

Contrary to our initial hypothesis, **AoS and SoA exhibit nearly identical performance**. The reason is that the accessed fields (`x` and `vy`) are close enough in memory that they reside within the **same cache line**. In this configuration, the AoS layout does not waste cache-line bandwidth.

### Key Insight

The performance of AoS vs. SoA is determined by **cache-line utilization**, not simply by "partial vs. full access." If the accessed fields in an AoS layout are close enough to fit within a single cache line, the performance penalty is negligible.

---

## Experiment 3B: Partial Access with Forced Cache-Line Waste

This experiment is designed to force cache-line waste in the AoS layout by accessing only a single, non-contiguous field.

### Setup Changes

-   **Accessed Fields:** Only one field (`vz`) is accessed.
-   **Struct Padding:** The structure is padded to ensure that accessing one field does not bring others into the same cache line.

### Rationale for Working Set Sizes

-   **N = 1,000:** Fits within L1 cache.
-   **N = 12,000:** Stresses L2 cache.
-   **N = 24,000:** Stresses the TLB.
-   **N = 500,000:** Exceeds the System Level Cache, forcing DRAM access.

### Results

| N         | Layout | Time (s)      |
| :-------- | :----- | :------------ |
| **1,000**   | SoA    | 1.417e-06     |
|           | AoS    | 1.583e-06     |
| **12,000**  | SoA    | 1.625e-05     |
|           | AoS    | 2.2542e-05    |
| **24,000**  | SoA    | 3.2709e-05    |
|           | AoS    | 5.7584e-05    |
| **500,000** | SoA    | 0.000705375   |
|           | AoS    | 0.00156187    |

### Interpretation

As the working set size increases, **SoA significantly outperforms AoS**.

-   When the data fits in L1, the performance is similar because latency is low.
-   As the data spills into L2 and beyond, the wasted memory bandwidth of the AoS layout becomes a major bottleneck. AoS fetches entire cache lines containing mostly unused data, leading to cache pollution and higher miss rates. SoA, in contrast, only fetches the data for the accessed field, resulting in much better cache and memory efficiency.

---

## Experiment 3C: Full Data Access

This experiment revisits the AoS vs. SoA comparison, but with an algorithm that accesses *all* fields of the structure.

### Results

| N         | Layout | Time (s)      |
| :-------- | :----- | :------------ |
| **1,000**   | SoA    | 1.583e-06     |
|           | AoS    | 3.291e-06     |
| **12,000**  | SoA    | 1.6375e-05    |
|           | AoS    | 2.6125e-05    |
| **24,000**  | SoA    | 4.2208e-05    |
|           | AoS    | 4.4917e-05    |
| **50,000**  | SoA    | 6.8458e-05    |
|           | AoS    | 6.8125e-05    |
| **100,000** | SoA    | 0.000141625   |
|           | AoS    | 0.000147      |

### Interpretation

The results are nuanced and reveal that even with full access, AoS is not guaranteed to be the winner.

1.  **CPU-Bound at Small N:** For small data sets that fit in the L1 cache, **SoA is faster than AoS**. This is not a memory-bound effect. The bottleneck is the CPU's address generation unit.
    -   **SoA address calculation:** `base + i * sizeof(float)`
    -   **AoS address calculation:** `base + i * sizeof(struct) + field_offset`
    The more complex address calculation for AoS makes it slower when memory latency is not a factor.

2.  **Memory-Bound at Large N:** As the data set size grows and the workload becomes memory-bound, the performance of AoS and SoA converges. The cost of DRAM access dominates the cost of address calculation, and the two layouts perform almost identically.

## Final Conclusion

The choice between AoS and SoA is a complex decision that depends on the dominant bottleneck in the system.

-   **Partial Access:** SoA is generally superior when accessing a subset of data fields, as it minimizes cache-line waste. The exception is when the accessed fields in an AoS layout are contiguous and fit within a single cache line.
-   **Full Access:** When the workload is CPU-bound (i.e., the data fits in L1), SoA can be faster due to simpler address calculations. When the workload is memory-bound, the performance of AoS and SoA is comparable.

It is unwise to make blanket assumptions. Performance analysis must consider the entire pipeline, from CPU instruction complexity to cache utilization, TLB behavior, and DRAM access patterns. **The effectiveness of a data layout depends on the dominant bottleneck, not just the completeness of the data access.**