#ifndef MAIN_H
#define	MAIN_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "Particle.h"
#include "PointCloud.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"
#include "SnowSimulation.h"

void redraw();
void initialize_simulation();

GLFWwindow* initGLFWContext();
static void error_callback(int, const char*);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, int, int, int);

void initGLContext();

#endif

