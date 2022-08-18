#include "widget.h"
#include <QIcon>

Widget::Widget(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Stage");
  setWindowIcon(QIcon(":/stage.jpg"));
  setMaximumSize(800, 600);
  setMinimumSize(800, 600);

  scene = new QGraphicsScene;
  view = new QGraphicsView(scene, this);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setBackgroundBrush(Qt::lightGray);
  scene->setSceneRect(-width() / 2, -height() / 2, width() * 2000,
                      height() * 2000);

  fps = new QLabel(this);
  fps->setGeometry(15, 15, 60, 15);
  fps->setFont(QFont("Noto Sans Mono CJK SC"));
  fps->setText("0 FPS");

  manager = new UIManager;
  thread = new QThread;

  connect(manager, SIGNAL(update_ui(QVector<QPolygonF>, QVector<int>)), this,
          SLOT(flash(QVector<QPolygonF>, QVector<int>)),
          Qt::BlockingQueuedConnection);

  connect(this, SIGNAL(delta_alpha(double)), manager,
          SLOT(update_alpha(double)), Qt::DirectConnection);
  connect(this, SIGNAL(delta_beta(double)), manager, SLOT(update_beta(double)),
          Qt::DirectConnection);
  connect(this, SIGNAL(delta_scale(double)), manager,
          SLOT(update_scale(double)), Qt::DirectConnection);
  connect(this, SIGNAL(delta_face_alpha(double)), manager,
          SLOT(update_face_alpha(double)), Qt::DirectConnection);
  connect(this, SIGNAL(delta_face_beta(double)), manager,
          SLOT(update_face_beta(double)), Qt::DirectConnection);

  connect(thread, &QThread::started, manager, &UIManager::run,
          Qt::DirectConnection);
  connect(manager, &UIManager::finished, thread, &QThread::quit,
          Qt::DirectConnection);
  connect(thread, &QThread::finished, manager, &UIManager::deleteLater,
          Qt::DirectConnection);
  connect(thread, &QThread::finished, thread, &UIManager::deleteLater,
          Qt::DirectConnection);

  manager->moveToThread(thread);
  thread->start();

  watchdog = new FPSWatchdog;
  watchthread = new QThread;

  connect(this, SIGNAL(fps_add()), watchdog, SLOT(update()));
  connect(watchdog, SIGNAL(send(int)), this, SLOT(fps_update(int)));

  connect(watchthread, &QThread::started, watchdog, &FPSWatchdog::start,
          Qt::DirectConnection);
  connect(watchthread, &QThread::finished, watchdog, &FPSWatchdog::deleteLater,
          Qt::DirectConnection);
  connect(watchthread, &QThread::finished, watchthread, &QThread::deleteLater,
          Qt::DirectConnection);

  watchdog->moveToThread(watchthread);
  watchthread->start();

  timer = new QTimer;
  timer->setInterval(20);
  connect(timer, &QTimer::timeout, this, &Widget::keyFetch,
          Qt::DirectConnection);
  timer->start();
}

Widget::~Widget() {
  manager->stop();
  watchthread->quit();
  delete fps;
  delete view;
  delete scene;
  delete timer;
}

void Widget::keyPressEvent(QKeyEvent *event) {
  if (!event->isAutoRepeat())
    keymap[event->key()] = true;
}

void Widget::keyReleaseEvent(QKeyEvent *event) {
  if (!event->isAutoRepeat())
    keymap[event->key()] = false;
}

void Widget::flash(QVector<QPolygonF> polygons, QVector<int> colormap) {
  const QVector<QColor> colors = {Qt::red,    Qt::green,    Qt::blue,
                                  Qt::yellow, Qt::darkCyan, Qt::darkMagenta};
  scene->clear();
  for (int i = 0; i < polygons.size(); ++i) {
    scene->addPolygon(polygons[i], QPen(Qt::white, 1),
                      QBrush(colors[colormap[i]]));
  }
  emit fps_add();
}

void Widget::fps_update(int fps_number) {
  fps->setText(QString::number(fps_number) + " FPS");
}

void Widget::keyFetch() {
  QList<int> keys = keymap.keys();
  for (int i = 0; i < keys.size(); ++i) {
    if (keys[i] == Qt::Key_W && keymap[keys[i]])
      emit delta_alpha(0.01);
    if (keys[i] == Qt::Key_S && keymap[keys[i]])
      emit delta_alpha(-0.01);
    if (keys[i] == Qt::Key_A && keymap[keys[i]])
      emit delta_beta(-0.01);
    if (keys[i] == Qt::Key_D && keymap[keys[i]])
      emit delta_beta(0.01);
    if (keys[i] == Qt::Key_Space && keymap[keys[i]])
      emit delta_scale(-0.1);
    if (keys[i] == Qt::Key_Z && keymap[keys[i]])
      emit delta_scale(0.1);
  }
}
