Execution Environment :

All experiments in this project are conducted on the following system -
Operating system : MacOS
Kernel : Darwin Kernel version 24.6.0
Architecture : arm64 (Apple Silicon)

CPU : 
Processor : Apple M2 Max 
Total cores : 12 
 Performance cores (P-cluster) : up to 3.33 Ghz
 Efficiency cores (E-cluster) : up to 1.5 Ghz

The Apple Mac M2 uses a heterogeneous core architecture. Performance characteristics widely differ between the P-cluster and E-cluster CPU cores.
Unless specified otherwise, the benchmarks are intended to reflect execution on performance cores. No explicit CPU affinity is enforced at this stage in this project 
and it is acknowledges that this could be a source of variability in results.

Cache hierarchy : 
 L1 cache :
  Instruction cache : 128 kB
  Data cache : 64 kb
 L2 cache : 4 MB
Apple silicon employs a non-inclusive cache and also has a system level cache (SLC) whose exact behaviour is not modeled in the project.
We are more interested in the relative trends rather than on absolute numbers in our project.

Memory : 
 Usable RAM : 32 GB

Compiler and build configuration : 
 Compiler : clang version 15.0.0
 Target : arm64 apple Darwin
 Optimization level : O2 with compiler vectorization disabled.

Runtime constraints :
Benchmarks are single threaded
No explicit control over frequency scheduling or power management.
Background system activity is minimal but not eliminated.

Out of Scope : 
Multithreaded execution
NUMA effects
SIMD intrinsics
GPU or accelerator usage.

These aspects are intentionally excluded since this projects only concerns itself with memory access behaviour on a single core CPU.



