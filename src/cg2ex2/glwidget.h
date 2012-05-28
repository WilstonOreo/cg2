#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

#include "cg2/PointCloud.hpp"

class GLWidget : public QGLWidget {
		Q_OBJECT
	public:
		explicit GLWidget(QWidget * parent = 0);

		cg2::PointCloud pointCloud;

		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);

		typedef enum { SELECT_KNEAREST, SELECT_RADIUS } SelectionMode;

		float pointSize;
		float radius;
		int kNearest;
		SelectionMode selectionMode;

		cg2::Point3f selection;

		void update();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int w, int h);
		virtual void paintGL();

	private:
		// some stateholders for mouse motion
		// last mouse position in window
		int old_x, old_y;
		// is left mouse button pressed
		bool lbutton;
		float angle;


	signals:

	public slots:

};

#endif // GLWIDGET_H
