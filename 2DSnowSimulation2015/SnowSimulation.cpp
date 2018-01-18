
#include "SnowSimulation.h"

using namespace std;

SnowSimulation::SnowSimulation(Scene* scene) {
	frameCount = 0;
	
	// Convert shapes to snow particles
	snow = PointCloud::createShape(scene->snow_shapes);
	if (snow == NULL) return;

	// Grid Initialization
	grid = new Grid(Vector2f(0), Vector2f(WIN_METERS_X, WIN_METERS_Y), Vector2f(256, 128), snow);
	grid->initializeMass();
	grid->calculateVolumes();
}

// Copy constructor
SnowSimulation::SnowSimulation(const SnowSimulation& orig) {}

SnowSimulation::~SnowSimulation() {}

void SnowSimulation::update() {

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
