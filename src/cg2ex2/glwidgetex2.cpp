#include "cg2/Vector.hpp"

#include "glwidgetex2.h"

#include <GL/glu.h>
#include <qnumeric.h>
#include <QMouseEvent>

#include <iostream>

using namespace cg2;

GLWidgetEx2::GLWidgetEx2(QWidget * parent) :
	QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent) {
}

void GLWidgetEx2::setPointSizeSource(double size) {
	pointSizeSource = size;
	updateGL();
}

void GLWidgetEx2::setPointSizeGrid(double size) {
	pointSizeGrid = size;
	updateGL();
}

void GLWidgetEx2::setGridSize(int size) {
	gridSize = size;
	recalc();
	updateGL();
}

void GLWidgetEx2::setDrawKDTree(int state) {
	pointCloud.drawKDTree(state != Qt::Unchecked);
	updateGL();
}

void GLWidgetEx2::recalc() {
	pointCloud.read("franke4.off");
	//void makeGrid(PointCloud & out, PointCloud const & in, int gridSize);

	pointGrid.width(gridSize);
	pointGrid.height(gridSize);
	pointGrid.generateGrid(pointCloud);

	//makeGrid(pointGrid, pointCloud, gridSize);
}

void GLWidgetEx2::initializeGL() {
	recalc();
	// Set up the rendering context, define display lists etc.:
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	// fix outlines z-fighting withthe quads
	glPolygonOffset(1, 1);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void GLWidgetEx2::resizeGL(int w, int h) {
	// round w and h to multiples of 2
	w = w & ~1;
	h = h & ~1;
	// setup viewport, projection etc.:
	glViewport(0, 0, (GLint)w, (GLint)h);

	// reshaped window aspect ratio
	float aspect = (float) w / (float) h;

	// restore view definition after window reshape
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// perspective projection
	gluPerspective(60.0, aspect, 1.0, 100.0);

	GLdouble centerX= 0;
	GLdouble centerY= 0;
	GLdouble centerZ= 0;
	// set camera parameters
	GLdouble eyeX=0;
	GLdouble eyeY=-1.5*pointCloud.boundingBox().size().length();
	GLdouble eyeZ=0;
	GLdouble upX=0;
	GLdouble upY=0;
	GLdouble upZ=1;

	gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);

	// clear background and depth buffer
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	update();
}

Point3f unProject(QPoint const & pos) {
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble modelView[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	double winX = pos.x();
	double winY = viewport[3] - pos.y();

	GLfloat winZ;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	if (winZ >= 1.0f) {
		return Point3f(qQNaN(), qQNaN(), qQNaN());
	}

	GLdouble x,y,z;
	gluUnProject(winX, winY, winZ, modelView, projection, viewport, &x, &y, &z);
	return Point3f(x,y,z);
}

void GLWidgetEx2::paintGL() {
	cg2::Vec3f center = 0.5*(pointCloud.boundingBox().max.vec3f() + pointCloud.boundingBox().min.vec3f());

	// light and material
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
	GLfloat mat_shininess[] = { 3.0 };
	GLfloat model_ambient[] = { 0.3, 0.3, 0.3 };
	GLfloat light_position[] = { 5.0, 5.0, 5.0, 0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);

	glLoadIdentity();
	glRotatef(pitch, 1, 0, 0);
	glRotatef(yaw, 0, 0, 1);
	glTranslatef(-center.x,-center.y,-center.z);

	pointGrid.drawSurface();
	/*
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	if (pointSizeSource) {
		glPointSize(pointSizeSource);
		pointCloud.draw(cg2::Color(0.8,0.5,0.0));
	}
	if (pointSizeGrid) {
		glPointSize(pointSizeGrid);
		pointGrid.draw(cg2::Color(0.0,0.5,1.0));
	}
	*/
}



// mouse motion
void GLWidgetEx2::mouseMoveEvent(QMouseEvent * event) {
	if (event->buttons() != Qt::NoButton) {
		yaw += event->x() - old_x;
		pitch += event->y() - old_y;
		if (pitch > 90) {
			pitch = 90;
		}
		if (pitch < -90) {
			pitch = -90;
		}
		updateGL();

		old_x = event->x();
		old_y = event->y();
	}
}

// mouse callback
void GLWidgetEx2::mousePressEvent(QMouseEvent * event) {
	if (event->button() != Qt::NoButton) {
		old_x = event->x();
		old_y = event->y();

		updateGL();
	}
}

double wendland(double d, double h) {
	/*x *= h;
	const double Q_PI = 3.14159265358979323846;
	return (1/sqrt(2*Q_PI))*exp(-0.5*d*d); // gauss, not wendland*/
	return std::pow(1-d/h, 4)*(4*d/h+1);
}

void makeGrid(PointCloud & out, PointCloud const & in, int gridSize) {
	BoundingBox box = in.boundingBox();
	int const width = gridSize, height = gridSize;
	float const xdist = box.size().x / width, ydist = box.size().y / height;
	float const xmin = box.min.x, ymin = box.min.y;

	float const radius = sqrt(xdist*xdist + ydist*ydist)*2;
	//float const radius = 2;


	//vector<float> grid;  // Den Typ Array2D gibts nich, analog kann man auch vector< vector<float> > nehmen
	//grid.resize(width*height);

	out.vertices.clear();
	for (int xp = 0; xp < width; xp++) {
		float x = xmin + xp*xdist;
		for (int yp = 0; yp < height; yp++) {
			float y = ymin + yp*ydist;

			Point3f currentXY(x, y, 0);
			int nPoints = 0;
			double sumWeight = 0;
			double currentZ = 0;
			foreach(Vertex const * v, in.collectInRadius(currentXY, radius)) {
				Point3f const & p = v->v;
				Point3f pFlat(p);
				pFlat.z = 0;

				if ((currentXY-p).length() > radius) {
					std::cout << "point exceeds radius: " << (currentXY-p).length() << std::endl;
				}
				double distance = (currentXY-pFlat).length(); // Distanz zwischen P und (x,y,0)
				double weight = wendland(distance, radius);

				sumWeight += weight;
				currentZ += weight * p.z;

				nPoints++;
			}
			if (nPoints && sumWeight) {
				/*std::cout
				        << "nPoints=" << nPoints
				        << " sumWeight=" << sumWeight
				        << " currentZ=" << currentZ;*/

				//current /= nPoints;
				currentZ /= sumWeight;
				/*std::cout
				        << " after=" << currentZ
				        << std::endl;*/
			}

			//grid[y*width+x] = current;

			currentXY.z = currentZ;
			out.vertices.push_back(currentXY);
		}
	}
	out.update();
}
