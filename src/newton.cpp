#include "../hdrs/newton.hpp"
#include <iostream>
#include <cmath>

long double newton_step(long double E, long double (*func)(void* args), long double (*func_deriv)(void* args), void* args)
{
    long double f = func(args);
    long double f_prime = func_deriv(args);
    
    if (std::abs(f_prime) < std::pow(EPSILON, 2.0)) return E - copysign(1.0, f) * std::min(std::abs(f), 0.1l);


    return E - f/f_prime;
}

long double newton_itterations(int max_iter, long double init_value, long double (*func)(void* args), long double (*func_deriv)(void* args), void* args)
{
    Args* fn_args = static_cast<Args*>(args);
    long double E = init_value + fn_args->e * std::sin(init_value);
    
    for (int i = 0; i < max_iter; ++i)
    {
        long double E_new = newton_step(E, func, func_deriv, args);
        
        if (std::abs(E - E_new) < EPSILON || std::abs(func(args)) < EPSILON) return E_new;
        
        E = E_new;
        fn_args->E = E;
    }
    
    std::cerr << "Warning: Newton method did not converge after " << max_iter << " iterations\n";
    return E;
}