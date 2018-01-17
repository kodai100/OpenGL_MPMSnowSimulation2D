#include "Shape.h"

Shape::Shape(Vector2f vel):vel(vel){}

// Copy constructor
Shape::Shape(const Shape& orig){}

Shape::~Shape(){}

void Shape::addPoint(float x, float y){
	vertices.push_back(Vector2f(x, y));
}


bool Shape::contains(float x, float y){
	//Source: http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html	
	bool result = false;
	int len = vertices.size();
	for (int i=0, j=len-1; i<len; j=i++){
		Vector2f &vi = vertices[i], &vj = vertices[j];
		if ((vi[1] > y) != (vj[1] > y) && (x < (vj[0] - vi[0]) * (y - vi[1]) / (vj[1]-vi[1]) + vi[0])) {
			result = !result;
		}
	}
	return result;
}


float Shape::area(){
	//Source: http://www.mathopenref.com/coordpolygonarea2.html
	float area = 0,
		len = vertices.size(),
		j = len-1;
	for (int i=0; i<len; j = i++){
		Vector2f &vi = vertices[i], &vj = vertices[j];
		area += (vj[0]+vi[0]) * (vj[1]-vi[1]);
    }
	return fabs(area/2);
}


float Shape::volume(){
	float len = sqrt(area());
	return len*len*len;
}


void Shape::bounds(float bounds[4]){
	bounds[0] = vertices[0][0]; bounds[1] = bounds[0];
	bounds[2] = vertices[0][1]; bounds[3] = bounds[2];
	for (int i=0, len=vertices.size(); i<len; i++){
		Vector2f &p = vertices[i];
		//X-bounds
		if (p[0] < bounds[0])
			bounds[0] = p[0];
		else if (p[0] > bounds[1])
			bounds[1] = p[0];
		//Y-bounds
		if (p[1] < bounds[2])
			bounds[2] = p[1];
		else if (p[1] > bounds[3])
			bounds[3] = p[1];
	}
}


void Shape::draw(){
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int i=0, l=vertices.size(); i<l; i++)
		glVertex2fv(vertices[i].data);
	glEnd();
	glColor3f(0, .3, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i=0, l=vertices.size(); i<l; i++)
		glVertex2fv(vertices[i].data);
	glEnd();
}


// ŽÀ‘•‚Ì•û‚Éstatic‚Í•s—v
Shape* Shape::generateSnowball(Vector2f origin, float radius, Vector2f vel) {
	Shape* snowball = new Shape(vel);
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
