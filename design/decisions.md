The objective of the project is to study the effects of memory access pattern on a single CPU core on the performance of an piece of code or software.

Important decisions : 

1. We are not bothered by the algorithmic complexity. We choose a piece of code and only modify the memory access patterns and observe the changes in CPU
cache utilization and performance of the system.
2. We are not interested in multi-threaded execution or SIMD , compiler auto vectorization etc. We are only interested in the single core execution. So, during compilation
we set O2 flag (to make the code fast enough but prevent O3's aggressive optimizations) and we disable vectorization completely to study the single core scalar behaviour.
3. We do not get into NUMA or other concepts in this project. this is solely devoted to memory access pattern study.
