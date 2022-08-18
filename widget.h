#ifndef WIDGET_H
#define WIDGET_H

#include "fpswatchdog.h"
#include "uimanager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPolygonF>
#include <QThread>
#include <QWidget>

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

public slots:
  void flash(QVector<QPolygonF> polygons, QVector<int> colormap);
  void fps_update(int fps_number);

signals:
  void fps_add();

private:
  QGraphicsScene *scene;
  QGraphicsView *view;
  QLabel *fps;
  UIManager *manager;
  QThread *thread;
  FPSWatchdog *watchdog;
  QThread *watchthread;
};
#endif // WIDGET_H
