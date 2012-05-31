#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "cg2/PointCloud.hpp"
#include "cg2/PointCloud2D.hpp"

class GLWidgetEx2 : public QGLWidget {
		Q_OBJECT
	public:
		explicit GLWidgetEx2(QWidget * parent = 0);

		cg2::PointCloud2D pointCloud;
		cg2::PointCloud2D pointGrid;
		cg2::PointCloud2D pointGridCasteljau;

		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int w, int h);
		virtual void paintGL();

	private:
		void recalc();

		float pointSizeSource;

		float pointSizeGrid;
		int gridSize;

		float pointSizeGridCasteljau;
		int gridSizeCasteljau;

		int renderMode;

		// some stateholders for mouse motion
		// last mouse position in window
		int old_x, old_y;
		float yaw, pitch;

	signals:

	public slots:
		void setPointSizeSource(double size);

		void setPointSizeGrid(double size);
		void setGridSize(int size);

		void setPointSizeGridCasteljau(double size);
		void setGridSizeCasteljau(int size);

		void setDrawKDTree(int state);
		void setRenderMode(int state);
};

#endif // GLWIDGET_H
