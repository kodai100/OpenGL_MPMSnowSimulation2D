#include "main.h"

using namespace std;

int frame_count = 0;

vector<Shape*> snow_shapes;
PointCloud* snow = NULL;
Grid* grid;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	GLFWwindow* window = initGLFWContext();
	initGLContext();

	initialize_simulation();
	

	while (!glfwWindowShouldClose(window)) {
		cout << "Frame: " << frame_count++ << endl;

		simulate();

		redraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

void createSnowman() {
	Shape* snowball = Shape::generateSnowball(Vector2f(.4, .16), .15, Vector2f(0, 0));
	snow_shapes.push_back(snowball);

	Shape* snowball2 = Shape::generateSnowball(Vector2f(.4, .4), .1, Vector2f(0, 0));
	snow_shapes.push_back(snowball2);

	Shape* snowball3 = Shape::generateSnowball(Vector2f(.4, .57), .07, Vector2f(0, 0));
	snow_shapes.push_back(snowball3);

	Shape* snowball4 = Shape::generateSnowball(Vector2f(.8, .4), .08, Vector2f(-10, 0));
	snow_shapes.push_back(snowball4);
}

void createSnowballSmash() {
	Shape* snowball = Shape::generateSnowball(Vector2f(.8, .6), .15, Vector2f(-10, 0));
	snow_shapes.push_back(snowball);

	Shape* snowball2 = Shape::generateSnowball(Vector2f(.2, .4), .15, Vector2f(10, 0));
	snow_shapes.push_back(snowball2);
}

//Simulation
void initialize_simulation() {
	
	createSnowman();
	//createSnowballSmash();
	

	//Convert drawn shapes to snow particles
	snow = PointCloud::createShape(snow_shapes);

	//If there are no shapes, we can't do a simulation
	if (snow == NULL) return;

	//Computational grid
	grid = new Grid(Vector2f(0), Vector2f(WIN_METERS, WIN_METERS), Vector2f(128), snow);

	//We need to estimate particle volumes before we start
	grid->initializeMass();
	grid->calculateVolumes();

}

void simulate() {
	//Initialize FEM grid
	grid->initializeMass();
	grid->initializeVelocities();

	//Compute grid velocities
	grid->explicitVelocities(GRAVITY);

#if ENABLE_IMPLICIT
	if (IMPLICIT_RATIO > 0)
		grid->implicitVelocities();
#endif

	//Map back to particles
	grid->updateVelocities();

	//Update particle data
	snow->update();
}

void redraw() {

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Grid nodes
	glPointSize(1);
	glColor3f(.2, .2, .2);
	glBegin(GL_POINTS);
	for (int i = 0; i<grid->size[0]; i++) {
		for (int j = 0; j<grid->size[1]; j++)
			glVertex2fv((grid->origin + grid->cellsize*Vector2f(i, j)).data);
	}
	glEnd();

	//Snow particles
	if (SUPPORTS_POINT_SMOOTH)
		glEnable(GL_POINT_SMOOTH);

	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i<snow->size; i++) {
		Particle& p = snow->particles[i];
		// We can use the particle's density to vary color
		float contrast = 0.6;
		float density = p.density / DENSITY * contrast;
		density += 1 - contrast;
		glColor3f(density, density, density);
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