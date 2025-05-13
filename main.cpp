#include <iostream>
#include "hdrs/args.hpp"
#include "hdrs/kepler.hpp"
#include "hdrs/newton.hpp"
#include "hdrs/simulation.hpp"
#include <cmath>


int main(int argc, char* argv[])
{
    init_window();
    simulate_system(0.0l, 86400.0l * 10.0l, Method::NEWTON, &double_system);
    return 0;
}