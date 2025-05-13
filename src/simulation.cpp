#include "../hdrs/simulation.hpp"
#include "../hdrs/newton.hpp"
#include "../hdrs/bisection.hpp"
#include "../hdrs/kepler.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

// camera position
glm::vec3 src = {5.0, 5.0, 5.0};
glm::vec3 target = {0.0, 0.0, 0.0};
glm::vec3 up = {0.0, 1.0, 0.0};

bool left_button_pressed = false;
double last_x = 0.0, last_y = 0.0;
float camera_distance = glm::length(src);
float camera_angle_y = atan2(src.z, src.x);
float camera_angle_x = asin(src.y / camera_distance);

std::pair<long double, long double> points_buffer[BUFFER_CAPACITY];
int point_counter = 0;
int buffer_index = 0;

GLFWwindow* window = nullptr;

void setup_projection() 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(WIDTH)/static_cast<double>(HEIGHT), 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) 
    {
        if (action == GLFW_PRESS) 
        {
            left_button_pressed = true;
            glfwGetCursorPos(window, &last_x, &last_y);
        } 
        else if (action == GLFW_RELEASE) 
        {
            left_button_pressed = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (left_button_pressed) 
    {
        double dx = xpos - last_x;
        double dy = ypos - last_y;
        
        camera_angle_x -= dy * SENSETIVITY;

        camera_angle_y += dx * SENSETIVITY;
        
        camera_angle_x = glm::clamp(camera_angle_x, -glm::pi<float>()/2 + 0.1f, glm::pi<float>()/2 - 0.1f);
        
        if (camera_angle_y > glm::pi<float>()) camera_angle_y -= 2 * glm::pi<float>();
        if (camera_angle_y < -glm::pi<float>()) camera_angle_y += 2 * glm::pi<float>();
        
        last_x = xpos;
        last_y = ypos;
        
        src.x = camera_distance * std::cos(camera_angle_y) * std::cos(camera_angle_x);
        src.y = camera_distance * std::sin(camera_angle_x);
        src.z = camera_distance * std::sin(camera_angle_y) * std::cos(camera_angle_x);
        
        setup_camera();
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera_distance -= static_cast<float>(yoffset) * 0.5f;
    if (camera_distance < 2.0f) camera_distance = 2.0f;
    if (camera_distance > 50.0f) camera_distance = 50.0f;
    
    src.x = camera_distance * std::cos(camera_angle_y) * std::cos(camera_angle_x);
    src.y = camera_distance * std::sin(camera_angle_x);
    src.z = camera_distance * std::sin(camera_angle_y) * std::cos(camera_angle_x);
    
    setup_camera();
}

void setup_camera() 
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glm::vec3 direction = glm::normalize(target - src);
    glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 camera_up = glm::normalize(glm::cross(right, direction));
    
    glm::mat4 view_matrix = glm::lookAt(
        src,
        target,
        camera_up
    );
    glLoadMatrixf(glm::value_ptr(view_matrix));
}

void init_window()
{   
    if (!glfwInit())
    {
        throw std::runtime_error("failed initializing libglfw");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Kepler", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("failed creating window");
    }

    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 0.9f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_constant_attenuation = 1.0f;
    GLfloat light_linear_attenuation = 0.05f;
    GLfloat light_quadratic_attenuation = 0.01f;
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_constant_attenuation);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light_linear_attenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light_quadratic_attenuation);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    setup_camera();
    setup_projection();
}

void draw_axes() 
{
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(50.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 50.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 50.0);
    glEnd();
}

void render_sun() 
{
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    GLfloat sun_ambient[] = {0.4f, 0.4f, 0.1f, 1.0f};
    GLfloat sun_diffuse[] = {1.0f, 1.0f, 0.3f, 1.0f};
    GLfloat sun_specular[] = {1.0f, 1.0f, 0.7f, 1.0f};
    GLfloat sun_emission[] = {0.9f, 0.9f, 0.3f, 1.0f};
    GLfloat sun_shininess = 50.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, sun_shininess);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluSphere(quadric, SUN_RADIUS, SUN_POLY, SUN_POLY);
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (float size = 1.1f; size < 1.5f; size += 0.1f) 
    {
        float alpha = 1.0f - (size - 1.1f) / 0.4f;
        glColor4f(1.0f, 1.0f, 0.3f, alpha*0.5f);
        gluSphere(quadric, 0.4 * size, 32, 32);
    }
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    
    gluDeleteQuadric(quadric);
}

void render_earth(long double x, long double y)
{
    GLfloat planet_ambient[] = {0.1f, 0.2f, 0.1f, 1.0f};
    GLfloat planet_diffuse[] = {0.2f, 0.8f, 0.3f, 1.0f};
    GLfloat planet_specular[] = {0.5f, 0.8f, 0.5f, 1.0f};
    GLfloat planet_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat planet_shininess = 30.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, planet_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, planet_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, planet_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, planet_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, planet_shininess);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(x, 0.0f, y);
    gluSphere(quadric, EARTH_RADIUS, EARTH_POLY, EARTH_POLY);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}

void render_system(long double x, long double y) 
{    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // draw_axes();

    // path
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.1f, 0.2f, 0.1f);
    if (point_counter == BUFFER_CAPACITY) 
    {
        for (int i = 0; i < BUFFER_CAPACITY; ++i) 
        {
            int idx = (buffer_index + i) % BUFFER_CAPACITY;
            auto point = points_buffer[idx];
            glVertex3f(point.first, 0.0f, point.second);
        }
    } 
    else
    {
        for (int i = 0; i < point_counter; ++i) 
        {
            auto point = points_buffer[i];
            glVertex3f(point.first, 0.0f, point.second);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);

    render_sun();

    render_earth(x, y);

    glDisable(GL_LIGHTING);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void simulate_system(long double t, long double dt, Method solver, void* args)
{   
    Args* fn_args = static_cast<Args*>(args);
    long double n = std::sqrt((GRAVITATIONAL_CONST * fn_args->central_mass) / std::pow(fn_args->a, 3));
    long double E;

    while (!glfwWindowShouldClose(window))
    {   
        // calculating system
        long double M = n * t;
        fn_args->M = M;
        fn_args->E = M;

        switch (solver)
        {
            case Method::NEWTON:
            {
                E = newton_itterations(N_ITTER, M, kepler_equation, kepler_equation_deriv, args);
                break;
            }
            
            case Method::BISECTION:
            {
                E = bisection(N_ITTER, kepler_equation, args);
                break;
            }
            
            default:
            {
                E = bisection(N_ITTER, kepler_equation, args);            
            }
        }

        E = std::fmod(E, 2 * M_PI);
        if (E < 0) E += 2 * M_PI;

        long double x = fn_args->a * (std::cos(E) - fn_args->e);
        long double y = fn_args->a * std::sqrt(1 - std::pow(fn_args->e, 2)) * std::sin(E);
        
        long double shifted_x = x / COORDINATES_SHIFT;
        long double shifted_y = y / COORDINATES_SHIFT;

        points_buffer[buffer_index] = {shifted_x, shifted_y};

        buffer_index = (buffer_index + 1) % BUFFER_CAPACITY;
        if (point_counter < BUFFER_CAPACITY) point_counter++;

        // rendering system
        render_system(shifted_x, shifted_y);

        std::cout << shifted_x << " " << shifted_y << std::endl;

        fn_args->E = E;
        t += dt;
    }

    glfwTerminate();
}