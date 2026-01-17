#include <vector>
#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std;
using namespace chrono;

using clock_type = chrono::steady_clock;

int main(int argc, char* argv[])
{
    if (argc < 2){
        cerr << "Usage : ./sequential <num_elements> \n";
    }
    const size_t N = stoull(argv[1]);
    vector<int64_t> data (N);

    // initialize data
    for (size_t i=0;i<N;i++) data[i] = static_cast<int64_t>(i);

    // Warm-up
    // 1. to push the cpu to steady state
    // 2. activate the hw prefetchers and instruction caches
    // 3. let the OS take care of page faults, TLB entries, page table population etc.
    volatile int64_t warm_up = 0;
    for(size_t i=0;i<N;i++) warm_up += data[i];
    // start timer
    auto start = clock_type::now();
    int64_t sum = 0;
    for(size_t i=0;i<N;i++) sum += data[i];
    auto end = clock_type::now();
    // Prevent dead-code elimination
    asm volatile("" : : "r"(sum) : "memory");
    chrono::duration<double> elapsed = end - start;
    cout << "N = " << N << " Time = " << elapsed.count() << " Sum = " << sum << endl;
    return 0;
}
