#ifndef FPSWATCHDOG_H
#define FPSWATCHDOG_H

#include <QObject>
#include <QTimer>

class FPSWatchdog : public QObject {
  Q_OBJECT
public:
  explicit FPSWatchdog(QObject *parent = nullptr);
  ~FPSWatchdog();

public slots:
  void start();
  void update();
  void reset();

signals:
  void send(int fps);

private:
  QTimer *timer;
  int record;
};

#endif // FPSWATCHDOG_H
