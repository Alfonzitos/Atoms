#include <random>

struct Orbital
{
    int n, l, m;
};
struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

class Hydrogen
{
public:
    std::vector<Vertex> compute_orbital_hydrogen(Orbital orbital,
                                                 int count, 
                                                 float max_radius,
                                                 int P_MAX_attempts
                                                );

private:
    float hydrogen_radial_prob(float r, int n, int l);

    float hydrogen_angular_prob(float theta, float phi, int m, int l);

    float compute_hydrogen_PMAX(Orbital orbital, float max_radius, int P_MAX_attempts);
};
