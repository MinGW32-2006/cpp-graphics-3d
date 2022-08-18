#ifndef WIDGET_H
#define WIDGET_H

#include "global-config.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private:
  QGraphicsScene *scene;
  QGraphicsView *view;
};
#endif // WIDGET_H
