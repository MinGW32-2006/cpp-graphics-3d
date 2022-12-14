#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QColor>
#include <QObject>
#include <QPolygonF>
#include <QVector3D>
#include <QVector>

class UIManager : public QObject {
  Q_OBJECT
public:
  explicit UIManager(QObject *parent = nullptr);

public slots:
  void run();
  void stop();

  void update_alpha(double delta);
  void update_beta(double delta);
  void update_scale(double delta);
  void update_face_alpha(double delta);
  void update_face_beta(double delta);

signals:
  void finished();
  void update_ui(QVector<QPolygonF> polygons, QVector<int> colormap);

private:
  bool is_running;
  double alpha, beta, scale;
  double face_alpha, face_beta;
};

#endif // UIMANAGER_H
