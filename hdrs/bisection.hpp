#ifndef BISECTION_HPP
#define BISECTION_HPP

long double bisection_step(long double&, long double&, long double (*func)(void* args), void*);

long double bisection(int, long double (*func)(void* args), void*);

#endif