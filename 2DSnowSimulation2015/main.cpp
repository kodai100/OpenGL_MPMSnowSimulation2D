#include "main.h"

// Unity format

void start() {
	Scene* scene = Scene::snowman();
	//Scene* scene = Scene::snowballSmash();
	//Scene* scene = Scene::highspeedSnowballSmash();

	snow_simulator = new SnowSimulation(scene);
}

void update() {
	cout << "Frame: " << frame_count << endl;
	snow_simulator->update();
}

void render() {
	// Draw grid
	snow_simulator->grid->draw();

	// Draw snow
	snow_simulator->snow->draw();
}






// -------------- GLFW Routine ------------------------

int main(int argc, char** argv) {

	srand(time(NULL));
	
	GLFWwindow* window = initGLFWContext();
	initGLContext();

	start();

	frame_count = 0;
	while (!glfwWindowShouldClose(window)) {

		update();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		frame_count++;
	}

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
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
	/*if (action != GLFW_RELEASE)
		return;
	switch (key) {
	case GLFW_KEY_ENTER:
		break;
	}*/
}

//Mouse listener
void mouse_callback(GLFWwindow* window, int btn, int action, int mods) {
	/*if (action == GLFW_RELEASE && btn == GLFW_MOUSE_BUTTON_LEFT) {
	}*/
}