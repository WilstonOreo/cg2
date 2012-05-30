#include "cg2/Vector.hpp"

#include "glwidgetex2.h"

#include <GL/glu.h>
#include <qnumeric.h>
#include <QMouseEvent>

using namespace cg2;

GLWidgetEx2::GLWidgetEx2(QWidget * parent) :
	QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::DirectRendering), parent) {
}

void GLWidgetEx2::setPointSize(double size) {
	pointSize = size;
	updateGL();
}

void GLWidgetEx2::setDrawKDTree(int state) {
	pointCloud.drawKDTree(state != Qt::Unchecked);
	updateGL();
}

void GLWidgetEx2::initializeGL() {
	pointCloud.read("franke4.off");

	// Set up the rendering context, define display lists etc.:
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_CULL_FACE);

	// fix outlines z-fighting withthe quads
	glPolygonOffset(1, 1);
	glEnable(GL_POLYGON_OFFSET_FILL);

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
	GLdouble eyeY=pointCloud.boundingBox().size().y*1.5;
	GLdouble eyeZ=-pointCloud.boundingBox().size().length();
	GLdouble upX=0;
	GLdouble upY=1;
	GLdouble upZ=0;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(pointSize);
	glLoadIdentity();
	glRotatef(270, 1, 0, 0);
	glRotatef(angle, 0, 0, 1);

	cg2::Vec3f c = 0.5*(pointCloud.boundingBox().max.vec3f() + pointCloud.boundingBox().min.vec3f());
	glTranslatef(-c.x,-c.y,-c.z);
	pointCloud.draw(cg2::Color(0.8,0.5,0.0));
}



// mouse motion
void GLWidgetEx2::mouseMoveEvent(QMouseEvent * event) {
	if (event->buttons() != Qt::NoButton) {
		angle += event->x() - old_x;
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
