#include "main.h"

using namespace std;

int frame_count = 0;

vector<Shape*> snow_shapes;
SnowSimulation* snow_simulator;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	GLFWwindow* window = initGLFWContext();
	initGLContext();

	initialize_simulation();
	

	while (!glfwWindowShouldClose(window)) {
		cout << "Frame: " << frame_count++ << endl;

		snow_simulator->update();

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
	Shape* snowball = Shape::generateSnowball(Vector2f(1, .16), .15, Vector2f(0, 0));
	snow_shapes.push_back(snowball);

	Shape* snowball2 = Shape::generateSnowball(Vector2f(1, .4), .1, Vector2f(0, 0));
	snow_shapes.push_back(snowball2);

	Shape* snowball3 = Shape::generateSnowball(Vector2f(1, .57), .07, Vector2f(0, 0));
	snow_shapes.push_back(snowball3);

	Shape* snowball4 = Shape::generateSnowball(Vector2f(1.8, .4), .08, Vector2f(-20, -5));
	snow_shapes.push_back(snowball4);
}

void createSnowballSmash() {
	Shape* snowball = Shape::generateSnowball(Vector2f(1.6, .65), .15, Vector2f(-10, 0));
	snow_shapes.push_back(snowball);

	Shape* snowball2 = Shape::generateSnowball(Vector2f(.3, .4), .15, Vector2f(10, 0));
	snow_shapes.push_back(snowball2);
}

void createHighspeedSnowballSmash() {
	Shape* snowball = Shape::generateSnowball(Vector2f(1.6, .6), .15, Vector2f(-20, 0));
	snow_shapes.push_back(snowball);

	Shape* snowball2 = Shape::generateSnowball(Vector2f(.3, .4), .15, Vector2f(20, 0));
	snow_shapes.push_back(snowball2);
}

//Simulation
void initialize_simulation() {
	
	//createSnowman();
	createSnowballSmash();
	//createHighspeedSnowballSmash();

	//Convert drawn shapes to snow particles
	PointCloud* snow = PointCloud::createShape(snow_shapes);
	//If there are no shapes, we can't do a simulation
	if (snow == NULL) return;

	snow_simulator = new SnowSimulation(snow);
}

void redraw() {

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Grid nodes
	glPointSize(1);
	glColor3f(.2, .2, .2);
	glBegin(GL_POINTS);
	for (int i = 0; i<snow_simulator->grid->size[0]; i++) {
		for (int j = 0; j<snow_simulator->grid->size[1]; j++)
			glVertex2fv((snow_simulator->grid->origin + snow_simulator->grid->cellsize*Vector2f(i, j)).data);
	}
	glEnd();

	//Snow particles
	if (SUPPORTS_POINT_SMOOTH)
		glEnable(GL_POINT_SMOOTH);

	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i<snow_simulator->snow->size; i++) {
		Particle& p = snow_simulator->snow->particles[i];
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
	glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);
	glOrtho(0, WIN_METERS_X, 0, WIN_METERS_Y, 0, 1);
}

GLFWwindow* initGLFWContext() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "Snow Simulator", NULL, NULL);

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