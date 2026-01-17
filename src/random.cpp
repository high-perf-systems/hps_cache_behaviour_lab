#include <vector>
#include <chrono>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

using clock_type = chrono::steady_clock;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage : ./random <Number> \n";
        return 1;
    }
    const size_t N = stoull(argv[1]);
    vector<int64_t> data(N);
    vector<size_t> indices(N);

    for(size_t i=0;i<N;i++){
        data[i] = static_cast<int64_t>(i);
        indices[i] = i;
    }

    // shuffle indices
    mt19937_64 rng(42);
    shuffle(indices.begin(), indices.end(), rng);

    // warm-up
    int64_t warm_up = 0;
    for(size_t i=0;i<N;i++) warm_up += data[indices[i]];

    auto start = clock_type::now();
    int64_t sum = 0;
    for(size_t i=0;i<N;i++) sum += data[indices[i]];

    auto end = clock_type::now();

    asm volatile("" : : "r"(sum) : "memory");

    chrono::duration<double> elapsed = end-start;
    cout << "N = " << N << " Time = " << elapsed.count() << " Sum = " << sum << endl;
    return 0;
}
