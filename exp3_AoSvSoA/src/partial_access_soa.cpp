// experiment 3b - see diff in soa and aos with forced cache utilization miss
// and increase in working set size to offset Apple Silicon's strong pre-fetchers and 
// large SLC cache.

#include <iostream>
#include <chrono>
#include <memory>

using clock_type = std::chrono::steady_clock;

struct Particles {
    std::unique_ptr<float[]> x, y, z, vx, vy, vz;
    float padding[26];
};

volatile double sink;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: soa_partial <N>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    Particles ps;
    ps.vz = std::make_unique<float[]>(N);

    for (size_t i = 0; i < N; i++)
        ps.vz[i] = static_cast<float>(i);

    // Warm-up
    double warm = 0;
    for (size_t i = 0; i < N; i++)
        warm += ps.vz[i];
    sink = warm;

    // Timed
    double sum = 0;
    auto start = clock_type::now();
    for (size_t i = 0; i < N; i++)
        sum += ps.vz[i];
    auto end = clock_type::now();

    sink = sum;

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "SoA Time = " << elapsed.count()
              << " Sum = " << sum << "\n";
}
