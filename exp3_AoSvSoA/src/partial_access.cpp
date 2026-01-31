#include <vector>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <memory>

using namespace std;
using namespace chrono;
using clock_type = chrono::steady_clock;

volatile double sink_aos = 0.0;
volatile double sink_soa = 0.0;
// a particle containing position and velocity (AoS)
struct Particle
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
};
// positions and velocities of particles (SoA)
struct Particles
{
    unique_ptr<float[]> x;
    unique_ptr<float[]> y;
    unique_ptr<float[]>  z;
    unique_ptr<float[]> vx;
    unique_ptr<float[]> vy;
    unique_ptr<float[]> vz;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage : partial_access <num_elements> " << endl;
        return -1;
    }
    const size_t N = stoull(argv[1]);
    vector<Particle> AOS(N);
    Particles SOA;
    double sum_aos = 0;
    double sum_soa = 0;
    // init of AOS vector and SOA structure
    SOA.x = make_unique<float[]>(N);
    SOA.y = make_unique<float[]>(N);
    SOA.z = make_unique<float[]>(N);
    SOA.vx = make_unique<float[]>(N);
    SOA.vy = make_unique<float[]>(N);
    SOA.vz = make_unique<float[]>(N);
    // values init
    for (int i=0;i<N;i++){
        AOS.at(i).x = 2.0f * i;
        SOA.x[i] = 2.0f * i;
        AOS.at(i).y = 3.0f + i;
        SOA.y[i] = 3.0f + i;
        AOS.at(i).z = i;
        SOA.z[i] = i;
        AOS.at(i).vx = 4.4 + 2*i;
        SOA.vx[i] = 4.4 + 2*i;
        AOS.at(i).vy = 1.5f + 0.5f * i;
        SOA.vy[i] = 1.5f + 0.5f * i;
        AOS.at(i).vz = -i;
        SOA.vz[i] = -i;
    }
    

    // warm-up run
    double warm_up_aos = 0.0;
    double warm_up_soa = 0.0;
    for (size_t i = 0; i < N; i++)
    {
        warm_up_aos += AOS.at(i).x + AOS.at(i).vy;
        warm_up_soa += SOA.x[i] + SOA.vy[i];
    }

    sink_aos = warm_up_aos;
    sink_soa = warm_up_soa;

    // AOS partial summing
    auto start_aos = clock_type::now();
    for(size_t i=0;i<N;i++){
        sum_aos += AOS.at(i).x + AOS.at(i).vy;
    }
    auto end_aos = clock_type::now();

    sink_aos = sum_aos;

    // SOA partial summing
    auto start_soa = clock_type::now();
    for (size_t i=0;i<N;i++){
        sum_soa += SOA.x[i] + SOA.vy[i];
    }
    auto end_soa = clock_type::now();
    sink_soa = sum_soa;

    duration<double> elapsed_aos = end_aos-start_aos;
    duration<double> elapsed_soa = end_soa - start_soa;

    cout << "N = " << N << 
        " Time AOS = " << elapsed_aos.count() << ", Time SOA = " << elapsed_soa.count() << 
        " Sum AOS = " << sum_aos << ", Sum SOA = " << sum_soa << endl;
    return 0;
}



    




    


