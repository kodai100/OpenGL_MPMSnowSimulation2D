#ifndef SNOWSIM_H
#define	SNOWSIM_H

#include "PointCloud.h"
#include "Grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "Particle.h"
#include "PointCloud.h"
#include "Grid.h"
#include "SimConstants.h"
#include "Shape.h"

class SnowSimulation {
public:
	int frameCount;
	Grid* grid;
	PointCloud* snow;	// null check

	SnowSimulation(PointCloud* snow);
	SnowSimulation(const SnowSimulation& orig);
	virtual ~SnowSimulation();

	void update();
};


#endif