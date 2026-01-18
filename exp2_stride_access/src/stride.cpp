#include <vector>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <array>
using namespace std;
using namespace chrono;

using clock_type = chrono::steady_clock;

volatile uint64_t sink = 0; // volatile sink to make the compiler complete loops instead of eliminating it prematurely

int main()
{
    const size_t N = (64*1024*1024) / sizeof(int32_t); // 64 mb of data
    vector<int32_t> data (N);
    for(size_t i=0;i<N;i++) data[i] = static_cast<int32_t>(i);

    array<size_t, 8> strides = {1,2,4,8,16,32,64,128}; // strides for memory access
    
    for (auto stride : strides){
        uint64_t warm_up = 0; // warm_up to push the cpu to steady state and prefill the pages, tlb and caches.
        for (size_t i=0; i<N; i++) warm_up += data[i];

        sink = warm_up;

        auto start = clock_type::now();
        uint64_t sum = 0;
        for(size_t offset = 0; offset < stride; offset++)
        {
            for (size_t i = offset; i < N; i+=stride) sum += data[i];
        }

        auto end = clock_type::now();

        sink = sum; // to prevent pre-mature compiler optimizations.
        
        duration<double> elapsed = end - start;

        cout << "Stride = " << stride << 
            " Time = " << elapsed.count() << 
            " Sum = " << sum << endl;
    }
    return 0;
}



