// experiment 3c - full access soa

#include <iostream>
#include <chrono>
#include <memory>

using clock_type = std::chrono::steady_clock;

struct Particles {
    std::unique_ptr<float[]> x, y, z, vx, vy, vz;
};

volatile double sink;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: soa_partial <N>\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    Particles ps;
    ps.x = std::make_unique<float[]>(N);
    ps.y = std::make_unique<float[]>(N);
    ps.z = std::make_unique<float[]>(N);
    ps.vx = std::make_unique<float[]>(N);
    ps.vy = std::make_unique<float[]>(N);
    ps.vz = std::make_unique<float[]>(N);

    for (size_t i = 0; i < N; i++)
    {
        ps.x[i] = 1;
        ps.y[i] = 2;
        ps.z[i] = static_cast<float>(i);
        ps.vx[i] = -1;
        ps.vy[i] = -static_cast<float>(i);
        ps.vz[i] = static_cast<float>(i);
    }

    // Warm-up
    double warm = 0;
    for (size_t i = 0; i < N; i++)
        warm += (ps.vz[i] + ps.x[i] + ps.y[i] + ps.z[i] + ps.vx[i]
             + ps.vy[i]);
    sink = warm;

    // Timed
    double sum = 0;
    auto start = clock_type::now();
    for (size_t i = 0; i < N; i++)
        sum += (ps.vz[i] + ps.x[i] + ps.y[i] + ps.z[i] + ps.vx[i]
             + ps.vy[i]);
    auto end = clock_type::now();

    sink = sum;

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "SoA Time = " << elapsed.count()
              << " Sum = " << sum << "\n";
}
