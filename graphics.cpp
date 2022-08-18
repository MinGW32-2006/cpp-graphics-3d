#include "graphics.h"
#include <cmath>

QVector3D Graphics::RotateVector(QVector3D v, double A, double B) {
  return {cosf(A) * (cosf(B) * v.x() + sinf(B) * v.z()) + sinf(A) * v.y(),
          -sinf(A) * (cosf(B) * v.x() + sinf(B) * v.z()) + cosf(A) * v.y(),
          cosf(B) * v.z() - sinf(B) * v.x()};
}

QVector3D Graphics::GenerateUnitVector(double A, double B) {
  return {cosf(A) * cosf(B), sinf(A), cosf(A) * sinf(B)};
}
