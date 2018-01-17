#include "main.h"

using namespace std;

//Old and new time values for each timestep
bool dirty_buffer = true;
int frame_count = 0,
bsize = 3 * WIN_SIZE*WIN_SIZE;
unsigned char* img_buffer;

//Simulation data
bool simulating = false;
vector<Shape*> snow_shapes;
int point_size;
PointCloud* snow = NULL;
Grid* grid;

void initGLContext() {
	//Setup OpenGL context
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIN_SIZE, WIN_SIZE);
	glOrtho(0, WIN_METERS, 0, WIN_METERS, 0, 1);
}

int main(int argc, char** argv) {
	srand(time(NULL));

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

	initGLContext();

	// -------
	start_simulation();

	Vector2f gravity = Vector2f(0, GRAVITY);
	float cum_sum = 0;
	int iter = 0;
	while (!glfwWindowShouldClose(window)) {
		if (dirty_buffer) {
			redraw();
			dirty_buffer = false;
		}



		TIMESTEP = 0.0002;// adaptive_timestep();

		cout << TIMESTEP << endl;

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


//Simulation
void start_simulation() {

	Shape* snowball = generateSnowball(Vector2f(.5, .5), .15);
	snow_shapes.push_back(snowball);

	//Convert drawn shapes to snow particles
	snow = PointCloud::createShape(snow_shapes, Vector2f(0, 0));

	//If there are no shapes, we can't do a simulation
	if (snow == NULL) return;
	point_size = 4;

	//Computational grid
	grid = new Grid(Vector2f(0), Vector2f(WIN_METERS, WIN_METERS), Vector2f(128), snow);

	//We need to estimate particle volumes before we start
	grid->initializeMass();
	grid->calculateVolumes();

	simulate();
	//thread simThread(simulate);
	//pthread_t sim_thread;
	//pthread_create(&sim_thread, NULL, simulate, NULL);
}

// Isolate draw process with threading
void simulate() {

	simulating = true;
	struct timespec delay;
	delay.tv_sec = 0;
	cout << "Starting simulation..." << endl;
	
}

float adaptive_timestep() {
	float max_vel = snow->max_velocity, f;
	if (max_vel > 1e-8) {
		//We should really take the min(cellsize) I think, if the grid is not square
		float dt = CFL * grid->cellsize[0] / sqrt(max_vel);
		f = dt > FRAMERATE ? FRAMERATE : dt;
	}
	else f = FRAMERATE;
	return f > MAX_TIMESTEP ? MAX_TIMESTEP : f;
}

void redraw() {

	if (!simulating) return;

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
		//We can use the particle's density to vary color
		float contrast = 0.6;
		float density = p.density / DENSITY*contrast;
		density += 1 - contrast;
		glColor3f(density, density, density);
		glVertex2fv(p.position.data);
	}
	glEnd();

	if (SUPPORTS_POINT_SMOOTH)
		glDisable(GL_POINT_SMOOTH);

}

Shape* generateSnowball(Vector2f origin, float radius) {
	Shape* snowball = new Shape();
	const int segments = 18;
	//Cool circle algorithm: http://slabode.exofire.net/circle_draw.shtml
	float theta = 6.283185307 / (float)segments,
		tan_fac = tan(theta),
		cos_fac = cos(theta),
		x = radius,
		y = 0;
	for (int i = 0; i<segments; i++) {
		snowball->addPoint(x + origin[0], y + origin[1]);
		float flip_x = -y, flip_y = x;
		x += flip_x*tan_fac;
		y += flip_y*tan_fac;
		x *= cos_fac;
		y *= cos_fac;
	}
	return snowball;
}