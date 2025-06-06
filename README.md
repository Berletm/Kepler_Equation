# Numerical Modeling of Kepler's Equation

This project implements numerical solutions to Kepler's equation for simulating orbital motion of celestial bodies with OpenGL visualization.

## Description

The project features:
- Two numerical methods for solving Kepler's equation:
  - Newton's method
  - Bisection method
- Orbital motion visualization using OpenGL/GLFW
- Support for different system configurations (Earth-Sun, comet, binary star system)

## Requirements

- C++ compiler with C++11 support
- CMake (version 3.20 or higher)
- Libraries:
  - GLFW3
  - OpenGL
  - GLM
  - GLU

## Building and Running

1. Clone the repository:
   ```bash
   git clone https://github.com/Berletm/Kepler_Equation.git
   cd Kepler_Equation
2. Build the project:
    ```bash
    mkdir build && cd build
    cmake ..
    make
3. Run the program:
    ```bash
    ./kepler

## Visualization 

### Helium comet  
![Helium comet](https://github.com/user-attachments/assets/8be30ae3-bf3c-4afa-afa5-2eccd581946b)

### Earth-Sun system  
![Earth-Sun system](https://github.com/user-attachments/assets/682df1e3-f4e6-4fc2-8c66-e58f992ea8bf)

### Binary star system  
![Binary star system](https://github.com/user-attachments/assets/bb46c031-4a15-4baf-90b9-098665c678cc)


