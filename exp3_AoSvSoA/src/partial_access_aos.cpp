// experiment 3b - forced cache utilization miss

#include <vector>
#include <iostream>
#include <chrono>

using clock_type = std::chrono::steady_clock;

volatile double sink;

struct Particle {
    float x, y, z, vx, vy, vz;
    float padding[26];
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: aos_partial <N>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    std::vector<Particle> data(N);

    for (size_t i = 0; i < N; i++) {
        data[i].vz = static_cast<float>(i);
    }

    // Warm-up
    double warm = 0;
    for (size_t i = 0; i < N; i++)
        warm += data[i].vz;
    sink = warm;

    // Timed
    double sum = 0;
    auto start = clock_type::now();
    for (size_t i = 0; i < N; i++)
        sum += data[i].vz;
    auto end = clock_type::now();

    sink = sum;

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "AoS Time = " << elapsed.count()
              << " Sum = " << sum << "\n";
}
