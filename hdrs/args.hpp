#ifndef ARGS_HPP
#define ARGS_HPP

#define EPSILON 1e-9l
#define GRAVITATIONAL_CONST 6.6743015l * 1e-11l
#define N_ITTER 10000
#define COORDINATES_SHIFT 1e11l

struct Args
{   
    long double a;
    long double e;
    long double M;
    long double E;
    long double central_mass;
};

#define SUN_MASS 1.98892l * 1e30l // sun mass
#define EARTH_e 0.0167086l  // earth eccentricity
#define EARTH_a 1.495978707l * 1e11l  // earth semi-major axis

static Args earth_params = 
{
    .a            = EARTH_a, 
    .e            = EARTH_e, 
    .M            = 0.0l, 
    .E            = 0.0l,
    .central_mass = SUN_MASS
};

#define HELIUM_a 17.8l * 1e11l
#define HELIUM_e 0.967l

static Args helium_comet_params = 
{
    .a            = HELIUM_a, 
    .e            = HELIUM_e, 
    .M            = 0.0l, 
    .E            = 0.0l,
    .central_mass = SUN_MASS
};

#define DOUBLE_SYSTEM_MASS 1.0l * 1e29l
#define DOUBLE_SYSTEM_a 5.0l * 1e11l
#define DOUBLE_SYSTEM_e 0.5l

static Args double_system = 
{
    .a            = DOUBLE_SYSTEM_a, 
    .e            = DOUBLE_SYSTEM_e, 
    .M            = 0.0l, 
    .E            = 0.0l,
    .central_mass = DOUBLE_SYSTEM_MASS
};

enum class Method
{
    NEWTON,
    BISECTION
};

#endif