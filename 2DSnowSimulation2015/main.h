#ifndef MAIN_H
#define	MAIN_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<thread>
#include <time.h>
#include <sys/stat.h>
#include <math.h>
#include "Particle.h"
#include "PointCloud.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"

float TIMESTEP;

static void error_callback(int, const char*);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, int, int, int);
void redraw();
void start_simulation();
void simulate();
float adaptive_timestep();

GLFWwindow* initGLFWContext();
void initGLContext();

#endif

