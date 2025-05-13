#include "../hdrs/bisection.hpp"
#include "../hdrs/args.hpp"
#include <cmath>
#include <iostream>

long double bisection_step(long double& E_left, long double& E_right, long double (*func)(void* args), void* args)
{
    Args fn_args_left = *(static_cast<Args*>(args));
    Args fn_args_right = *(static_cast<Args*>(args));
    Args fn_args_mid = *(static_cast<Args*>(args));
    Args* fn_args = static_cast<Args*>(args);

    long double E_mid = (E_left + E_right) / 2.0l;
    fn_args->E = E_mid;

    fn_args_left.E = E_left;
    fn_args_right.E = E_right;
    fn_args_mid.E = E_mid;

    long double f_left = func(&fn_args_left);
    long double f_mid = func(&fn_args_mid);

    if (std::abs(f_mid) < EPSILON) 
    {
        E_left = E_mid;
        E_right = E_mid;
        return E_mid;
    }

    if (std::signbit(f_left) != std::signbit(f_mid)) 
    {
        E_right = E_mid;
    } 
    else 
    {
        E_left = E_mid;
    }

    return E_mid;
}

long double bisection(int max_iter, long double (*func)(void* args), void* args)
{
    Args* fn_args = static_cast<Args*>(args);
    
    long double k = std::floor(fn_args->M / (2 * M_PI));
    fn_args->M = fn_args->M - 2 * k * M_PI;

    long double E = fn_args->E;

    long double left = 0.0l;
    long double right = 2.0l * M_PIl;

    fn_args->E = left;
    if (std::abs(func(args)) < EPSILON) return left;

    fn_args->E = right;
    if (std::abs(func(args)) < EPSILON) return right;

    fn_args->E = E;

    for (int i = 0; i < max_iter; ++i)
    {
        E = bisection_step(left, right, func, args);

        if (std::abs(func(args)) < EPSILON) return (left + right) / 2.0l;
    }

    std::cerr << "Warning: Bisection method did not converge after " << max_iter << " iterations\n";

    return E;
}