// experiment 3c - full access Array of Structures

#include <vector>
#include <iostream>
#include <chrono>

using clock_type = std::chrono::steady_clock;

volatile double sink;

struct Particle {
    float x, y, z, vx, vy, vz;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: aos_partial <N>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    std::vector<Particle> data(N);

    for (size_t i = 0; i < N; i++) {
        data[i].x = 1;
        data[i].y = 2;
        data[i].z = (float)i;
        data[i].vx = -1;
        data[i].vy = -(float)i;
        data[i].vz = static_cast<float>(i);
    }

    // Warm-up
    double warm = 0;
    for (size_t i = 0; i < N; i++)
        warm += (data[i].x + data[i].y + data[i].z + data[i].vx + data[i].vy
             + data[i].vz);
    sink = warm;

    // Timed
    double sum = 0;
    auto start = clock_type::now();
    for (size_t i = 0; i < N; i++)
        sum += (data[i].x + data[i].y + data[i].z + data[i].vx + data[i].vy
             + data[i].vz);
    auto end = clock_type::now();

    sink = sum;

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "AoS Time = " << elapsed.count()
              << " Sum = " << sum << "\n";
}
