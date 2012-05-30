#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "cg2/PointCloud.hpp"

class GLWidgetEx2 : public QGLWidget {
		Q_OBJECT
	public:
		explicit GLWidgetEx2(QWidget * parent = 0);

		cg2::PointCloud pointCloud;
		cg2::PointCloud pointGrid;

		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int w, int h);
		virtual void paintGL();

	private:
		float pointSize;
		// some stateholders for mouse motion
		// last mouse position in window
		int old_x, old_y;
		float angle;

	signals:

	public slots:
		void setPointSize(double size);
		void setDrawKDTree(int state);
};

#endif // GLWIDGET_H
