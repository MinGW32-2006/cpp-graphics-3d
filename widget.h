#ifndef WIDGET_H
#define WIDGET_H

#include "fpswatchdog.h"
#include "uimanager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QMap>
#include <QPolygonF>
#include <QThread>
#include <QTimer>
#include <QWidget>

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

public slots:
  void flash(QVector<QPolygonF> polygons, QVector<int> colormap);
  void fps_update(int fps_number);
  void keyFetch();

signals:
  void fps_add();
  void delta_alpha(double delta);
  void delta_beta(double delta);
  void delta_scale(double delta);
  void delta_face_alpha(double delta);
  void delta_face_beta(double delta);

private:
  QGraphicsScene *scene;
  QGraphicsView *view;
  QLabel *fps;
  UIManager *manager;
  QThread *thread;
  FPSWatchdog *watchdog;
  QThread *watchthread;
  QMap<int, bool> keymap;
  QTimer *timer;
};
#endif // WIDGET_H
