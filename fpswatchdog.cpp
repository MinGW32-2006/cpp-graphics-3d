#include "fpswatchdog.h"

FPSWatchdog::FPSWatchdog(QObject *parent) : QObject(parent), record(0) {}

FPSWatchdog::~FPSWatchdog() { delete timer; }

void FPSWatchdog::start() {
  timer = new QTimer;
  timer->setInterval(250);
  connect(timer, SIGNAL(timeout()), this, SLOT(reset()));
  timer->start();
}

void FPSWatchdog::update() { record++; }

void FPSWatchdog::reset() {
  emit send(record * 4);
  record = 0;
}
