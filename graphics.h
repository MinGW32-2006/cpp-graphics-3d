#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QVector3D>

namespace Graphics {
QVector3D RotateVector(QVector3D v, double A, double B);
QVector3D GenerateUnitVector(double A, double B);
} // namespace Graphics

#endif // GRAPHICS_H
