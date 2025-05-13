#include "../hdrs/kepler.hpp"
#include "../hdrs/args.hpp"
#include <cmath>

long double kepler_equation(void* args)
{
    Args* fn_args = static_cast<Args*>(args);
    return fn_args->E - fn_args->e * std::sin(fn_args->E) - fn_args->M;
}

long double kepler_equation_deriv(void* args)
{
    Args* fn_args = static_cast<Args*>(args);
    return 1 - fn_args->e * std::cos(fn_args->E);
}