#include <cmath>


float calc_factorial(int fac)
{
    float res = 1;
    for (int i = 1; i <= fac; i++)
    {
        res = res * i;
    }
    return res;
}

float calc_double_factorial(int fac)
{
    float res = 1;
    for (int i = fac; i >= 1; i -= 2)
    {
        res = res * i;
    }
    return res;
}

float legendre(int m, int l, float arg)
{

    float P_m_m = pow(-1.0f, m) * calc_double_factorial(2.0f * m - 1) * pow((1 - arg * arg), (m / 2.0f));
    float P_m_m1 = arg * (2 * m + 1) * P_m_m;

    if (l == m)
    {
        return P_m_m;
    };
    if (l == m + 1)
    {
        return P_m_m1;
    };

    for (int i = m + 2; i <= l; i++)
    {
        float temp = ((2 * i - 1) * arg * P_m_m1 - (i + m - 1) * P_m_m) / (i - m);
        P_m_m = P_m_m1;
        P_m_m1 = temp;
    }
    return P_m_m1;
};

float lagurre(int alpha, int k, float arg)
{
    float L_alpha_kZero = 1;
    float L_alpha_kOne = 1 + alpha - arg;

    if (k == 0)
    {
        return L_alpha_kZero;
    };
    if (k == 1)
    {
        return L_alpha_kOne;
    };

    for (int i = 1; i < k; i++)
    {
        float temp = ((2 * i + 1 + alpha - arg) * L_alpha_kOne - (i + alpha) * L_alpha_kZero) / (i + 1);
        L_alpha_kZero = L_alpha_kOne;
        L_alpha_kOne = temp;
    }
    return L_alpha_kOne;
}