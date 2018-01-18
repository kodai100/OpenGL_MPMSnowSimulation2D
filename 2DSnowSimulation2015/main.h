#ifndef MAIN_H
#define	MAIN_H

// -- Note
// This code is based on Azmisov's 2d MPM implementation : https://github.com/Azmisov/snow

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Particle.h"
#include "PointCloud.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"
#include "SnowSimulation.h"
#include "Scene.h"

using namespace std;

int frame_count;
vector<Shape*> snow_shapes;
SnowSimulation* snow_simulator;

void start();
void update();
void render();

// GLFW
GLFWwindow* initGLFWContext();
static void error_callback(int, const char*);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, int, int, int);

// GL
void initGLContext();

#endif

