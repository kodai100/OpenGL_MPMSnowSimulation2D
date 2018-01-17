#include "main.h"

using namespace std;

bool dirty_buffer = true;
int frame_count = 0;

int point_size = 2;
Vector2f gravity = Vector2f(0, GRAVITY);

vector<Shape*> snow_shapes;
PointCloud* snow = NULL;
Grid* grid;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	GLFWwindow* window = initGLFWContext();
	initGLContext();

	// -------
	start_simulation();
	
	float cum_sum = 0;
	int iter = 0;
	while (!glfwWindowShouldClose(window)) {
		if (dirty_buffer) {
			redraw();
			dirty_buffer = false;
		}

		cum_sum += TIMESTEP;

		//Initialize FEM grid
		grid->initializeMass();
		grid->initializeVelocities();
		//Compute grid velocities
		grid->explicitVelocities(gravity);
#if ENABLE_IMPLICIT
		if (IMPLICIT_RATIO > 0)
			grid->implicitVelocities();
#endif
		//Map back to particles
		grid->updateVelocities();
		//Update particle data
		snow->update();

		//Redraw snow
		if (!LIMIT_FPS || cum_sum >= FRAMERATE) {
			dirty_buffer = true;
			cum_sum -= FRAMERATE;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}




//Simulation
void start_simulation() {

	Shape* snowball = Shape::generateSnowball(Vector2f(.5, .5), .15);
	snow_shapes.push_back(snowball);

	//Convert drawn shapes to snow particles
	snow = PointCloud::createShape(snow_shapes, Vector2f(0, 0));

	//If there are no shapes, we can't do a simulation
	if (snow == NULL) return;

	//Computational grid
	grid = new Grid(Vector2f(0), Vector2f(WIN_METERS, WIN_METERS), Vector2f(128), snow);

	//We need to estimate particle volumes before we start
	grid->initializeMass();
	grid->calculateVolumes();

	simulate();
}

// Isolate draw process with threading
void simulate() {

	cout << "Starting simulation..." << endl;
	
}

void redraw() {

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Grid nodes
	glPointSize(1);
	glColor3f(0, .2, .5);
	glBegin(GL_POINTS);
	for (int i = 0; i<grid->size[0]; i++) {
		for (int j = 0; j<grid->size[1]; j++)
			glVertex2fv((grid->origin + grid->cellsize*Vector2f(i, j)).data);
	}
	glEnd();

	//Snow particles
	if (SUPPORTS_POINT_SMOOTH)
		glEnable(GL_POINT_SMOOTH);

	glPointSize(point_size);
	glBegin(GL_POINTS);
	for (int i = 0; i<snow->size; i++) {
		Particle& p = snow->particles[i];
		////We can use the particle's density to vary color
		//float contrast = 0.6;
		//float density = p.density / DENSITY * contrast;
		//density += 1 - contrast;
		//glColor3f(density, density, density);
		glColor3f(1, 1, 1);
		glVertex2fv(p.position.data);
	}
	glEnd();

	if (SUPPORTS_POINT_SMOOTH)
		glDisable(GL_POINT_SMOOTH);

}


void initGLContext() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIN_SIZE, WIN_SIZE);
	glOrtho(0, WIN_METERS, 0, WIN_METERS, 0, 1);
}

GLFWwindow* initGLFWContext() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(WIN_SIZE, WIN_SIZE, "Snow Simulator", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	return window;
}

//Print all errors to console
static void error_callback(int error, const char* description) {
	printf("\nError: %s", description);
}

//Key listener
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE)
		return;
	switch (key) {
	case GLFW_KEY_F12:
		break;
	case GLFW_KEY_ESCAPE:
		break;
	case GLFW_KEY_ENTER:
		break;
	}
}

//Mouse listener
void mouse_callback(GLFWwindow* window, int btn, int action, int mods) {
	if (action == GLFW_RELEASE && btn == GLFW_MOUSE_BUTTON_LEFT) {
	}
}