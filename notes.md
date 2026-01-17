Important points to remember :

Upon getting hw data from the computer, we see that the code and benchmark is done on Apple m2 silicon which uses arm64 architecture. 
there are some nuances associated with the m2 silicon, the cache is not a traditional hierarchy like the x86 systems. there is a slc (system level cache) and l1, l2 caches. 
Also, the CPU cores are split into efficiency cores (E-cluster) and Performance cores (P-cluster) each of which have different frequency limits. 
We do not enforce control over which CPU or which cache is used. So, we acknowledge that the results are not completely reproducible every time. we are not interested in the 
absolute numbers, we are only interested in the relative trends as to how the performance changes when there is cache hit vs cache miss , difference in performance seen with changes in 
data layout etc.


