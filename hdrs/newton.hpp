#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "../hdrs/args.hpp"

long double newton_step(long double, long double (*func)(void* args), long double (*func_deriv)(void* args), void*);

long double newton_itterations(int, long double, long double (*func)(void* args), long double (*func_deriv)(void* args), void*);

#endif