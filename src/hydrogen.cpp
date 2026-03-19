#include "probability_math.h"
#include "hydrogen.h"


#include <cmath>
#include <complex>
#include <random>



std::vector<Vertex> Hydrogen::compute_orbital_hydrogen(Orbital orbital, int count, float max_radius, int P_MAX_attempts)
{
    std::vector<Vertex> verts;
    verts.reserve(count);

    float P_MAX = compute_hydrogen_PMAX(orbital, max_radius, P_MAX_attempts);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> distRadial(0.0f, max_radius);
    std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

    std::uniform_real_distribution<float> distPhi(0.0f, 2 * M_PI);
    std::uniform_real_distribution<float> distU(0.0f, P_MAX);


    int i = 0;
    while (i < count)
    {
        float r = distRadial(rng);

        float z = 2.0f * dist01(rng) -1.0f;
        float r_xy = sqrt(1- z*z);

        float theta = acos(z);

        float phi = distPhi(rng);
        float u = distU(rng);


        

        float radial_wave = hydrogen_radial_prob(r, orbital.n, orbital.l);
        float angular_wave = hydrogen_angular_prob(theta, phi, orbital.m, orbital.l);

        float psi_sign = radial_wave * angular_wave;
        
        float radial_prob_density = r*r * radial_wave * radial_wave;
        float angular_prob_density = angular_wave * angular_wave;
        
        float prob = radial_prob_density * angular_prob_density;

        if (u < prob)
        {   
            float red, green, blue;

            if (psi_sign < 0) {
                red = 0.2f;
                green = 0.3f;
                blue = 1.0f;
            } else {
                red = 1.0f;
                green = 0.3f;
                blue = 0.2f;
            }
            float x = r * r_xy * cos(phi);
            float y = r * r_xy * sin(phi);
            float zpos = r * z;

            float threshold = 0.0f;
            if (prob / P_MAX < threshold) continue;
            float normProb = sqrt(prob / P_MAX);
            
            verts.push_back(Vertex{x, y, zpos, red, green, blue, normProb});
            i++;
        }

    }
    return verts;

}

float Hydrogen::hydrogen_radial_prob(float r, int n, int l)
{
    float a0 = 3.0f;
    // 2/na0 ^3
    float temp1 = pow((2.0 / (n * a0)), 3);

    // n-l-1 fac  /  2n(n+l) fac
    float temp2 = (calc_factorial(n - l - 1.0f) / (2.0f * n * calc_factorial(n + l)));

    // sqrt of 2 terms above
    float temp3 = sqrt(temp1 * temp2);

    // e ^ -r/n*a0
    float temp4 = std::exp(-r / (n * a0));

    //(2r / na0)^l
    float temp5 = pow((2.0f * r) / (n * a0), l);

    // Lagurre(2l+1, n-l-1)
    float temp6 = lagurre(2.0f * l + 1, n - l - 1, (2 * r) / (n * a0));

    float radial_wave = temp3 * temp4 * temp5 * temp6;
    //float radial_prob_density = r * r * radial_wave * radial_wave;
    //return radial_prob_density;
    return radial_wave;
}

float Hydrogen::hydrogen_angular_prob(float theta, float phi, int m, int l)
{
    //-1^m
    float temp1 = pow(-1, m);

    // (2l+1)/4pi
    float temp2 = (2 * l + 1) / (4 * M_PI);

    // l-m fac  / l+m fac
    float temp3 = calc_factorial(l - m) / calc_factorial(l + m);

    // sqrt of 2 values above
    float temp4 = sqrt(temp2 * temp3);
    if (m != 0)
        temp4 *= sqrt(2.0f);

    // legendre(m,l, cos theta)
    float temp5 = legendre(m, l, cos(theta));

    // e ^ im*phi
    float temp6 = cos(m * phi);

    float angular_wave = temp1 * temp4 * temp5 * temp6;
    return angular_wave;

}

float Hydrogen::compute_hydrogen_PMAX(Orbital orbital, float max_radius, int P_MAX_attemps)
{
    std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<float> distRadial(0.0f, max_radius);
    std::uniform_real_distribution<float> distTheta(0.0f, M_PI);
    std::uniform_real_distribution<float> distPhi(0.0f, 2 * M_PI);

    float P_MAX = 0;
    for (int i = 0; i < P_MAX_attemps; i++)
    {

        float r = distRadial(rng);
        float theta = distTheta(rng);
        float phi = distPhi(rng);

        float P = hydrogen_radial_prob(r, orbital.n, orbital.l) * std::norm(hydrogen_angular_prob(theta, phi, orbital.m, orbital.l));
        if (P > P_MAX)
        {
            P_MAX = P;
        };
    }
    return P_MAX;
}

