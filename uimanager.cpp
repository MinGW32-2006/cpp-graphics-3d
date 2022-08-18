#include "uimanager.h"
#include "graphics.h"
#include <QDebug>
#include <QPolygonF>
#include <algorithm>
#include <cmath>

using std::sort;

UIManager::UIManager(QObject *parent)
    : QObject(parent), is_running(true), alpha(0), beta(0), scale(10),
      face_alpha(0), face_beta(M_PI) {}

void UIManager::run() {
  const QVector<QVector3D> points = {{1, 1, 1},   {1, 1, -1},  {1, -1, 1},
                                     {1, -1, -1}, {-1, 1, 1},  {-1, 1, -1},
                                     {-1, -1, 1}, {-1, -1, -1}};
  const QVector<QVector<int>> surfaces = {{0, 1, 3, 2, 0}, {0, 1, 5, 4, 1},
                                          {0, 2, 6, 4, 2}, {7, 6, 4, 5, 3},
                                          {7, 6, 2, 3, 4}, {7, 5, 1, 3, 5}};

  while (is_running) {
    QVector3D pos = scale * Graphics::GenerateUnitVector(alpha, beta);
    QVector3D n = Graphics::GenerateUnitVector(face_alpha, face_beta);

    QVector<QVector<int>> s = surfaces;
    sort(s.begin(), s.end(),
         [points, pos](const QVector<int> &lhs,
                       const QVector<int> &rhs) -> bool {
           double left = ((points[lhs[0]] - pos) + (points[lhs[1]] - pos) +
                          (points[lhs[2]] - pos) + (points[lhs[3]] - pos))
                             .length();
           double right = ((points[rhs[0]] - pos) + (points[rhs[1]] - pos) +
                           (points[rhs[2]] - pos) + (points[rhs[3]] - pos))
                              .length();
           return left > right;
         });

    QVector<QVector<QVector3D>> converted;

    for (auto i : s) {
      QVector<QVector3D> cut;
      for (int k = 0; k < 4; ++k) {
        QVector3D vec = points[i[k]] - pos, prev, next;
        if (k == 0) {
          prev = points[i[3]] - pos;
        } else {
          prev = points[i[k - 1]] - pos;
        }
        if (k == 3) {
          next = points[i[0]] - pos;
        } else {
          next = points[i[k + 1]] - pos;
        }
        if (QVector3D::dotProduct(vec, n) - 1 < 0 &&
            QVector3D::dotProduct(prev, n) - 1 > 0) {
          double s =
              (1 - QVector3D::dotProduct(vec, n)) /
              (QVector3D::dotProduct(prev, n) - QVector3D::dotProduct(vec, n));
          cut.push_back((1 - s) * vec + s * prev);
        } else if (QVector3D::dotProduct(vec, n) - 1 < 0 &&
                   QVector3D::dotProduct(next, n) - 1 > 0) {
          double s =
              (1 - QVector3D::dotProduct(vec, n)) /
              (QVector3D::dotProduct(next, n) - QVector3D::dotProduct(vec, n));
          cut.push_back((1 - s) * vec + s * next);
        } else {
          cut.push_back(vec);
        }
      }
      converted.push_back(cut);
    }

    QVector<QPolygonF> polygons;

    for (auto i : converted) {
      QPolygonF poly;
      bool all_negative = true;
      for (QVector3D j : i) {
        QVector3D r = j;
        if (QVector3D::dotProduct(n, r) >= 1)
          all_negative = false;
        QVector3D t = 400 / QVector3D::dotProduct(n, r) *
                      QVector3D::crossProduct(QVector3D::crossProduct(n, r), n);
        t = Graphics::RotateVector(t, alpha, beta);
        poly.push_back({t.z(), t.y()});
      }
      if (all_negative)
        continue;
      polygons.push_back(poly);
    }

    QVector<int> colormap;
    for (auto i : s) {
      colormap.push_back(i[4]);
    }

    emit update_ui(polygons, colormap);
  }
  emit finished();
}

void UIManager::stop() { is_running = false; }

void UIManager::update_alpha(double delta) { alpha += delta; }

void UIManager::update_beta(double delta) { beta += delta; }

void UIManager::update_scale(double delta) { scale += delta; }

void UIManager::update_face_alpha(double delta) { face_alpha += delta; }

void UIManager::update_face_beta(double delta) { face_beta += delta; }
