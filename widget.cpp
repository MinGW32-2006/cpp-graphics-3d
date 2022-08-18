#include "widget.h"
#include "graphics.h"
#include <QIcon>

Widget::Widget(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Stage");
  setWindowIcon(QIcon(":/stage.jpg"));
  setMaximumSize(800, 600);
  setMinimumSize(800, 600);

  scene = new QGraphicsScene;
  scene->setSceneRect(-width() / 2, -height() / 2, width() * 2000,
                      height() * 2000);

  view = new QGraphicsView(scene, this);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setBackgroundBrush(Qt::lightGray);

  fps = new QLabel(this);
  fps->setGeometry(15, 15, 60, 15);
  fps->setFont(QFont(DEFAULT_FONT));
  fps->setText("0 FPS");
}

Widget::~Widget() {
  delete fps;
  delete view;
  delete scene;
}
