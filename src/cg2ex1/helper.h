#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QtOpenGL/QtOpenGL>

static inline void glhApplyBrush(QBrush const & brush)
{
    QColor color = brush.color();
    glColor4d(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

static inline void glhApplyPen(QPen const & pen)
{
    glhApplyBrush(pen.brush());
    glLineWidth(pen.widthF());
}

#define DECOMPOSEV3(v) (v).x, (v).y, (v).z

#endif // HELPER_H
