#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/ext.hpp>

#include <vector>

#include "../hdrs/args.hpp"

#define BUFFER_CAPACITY 2048

#define WIDTH 1280
#define HEIGHT 720

#define SENSETIVITY 0.01f

#define SUN_POLY 2048
#define EARTH_POLY (SUN_POLY/2)

#define SUN_RADIUS 0.5l
#define EARTH_RADIUS (SUN_RADIUS/11.0l)

extern glm::vec3 src;
extern glm::vec3 target;
extern glm::vec3 up;
extern std::pair<long double, long double> points_buffer[];
extern int point_counter;
extern int buffer_index;
extern GLFWwindow* window;

extern bool left_button_pressed;
extern double last_x, last_y;
extern float camera_distance, camera_angle_y, camera_angle_x;


void simulate_system(long double, long double, Method, void*);

void render_system(long double, long double);

void render_sun();

void render_earth(long double, long double);

void init_window();

void setup_camera();

void setup_projection();

void mouse_button_callback(GLFWwindow*, int, int, int);

void cursor_position_callback(GLFWwindow*, double, double);

void scroll_callback(GLFWwindow*, double, double);

void draw_axes();


#endif