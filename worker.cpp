#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Worker::updateStatus);
    m_timer->start(500);
}

void Worker::updateStatus() {
    static int count = 0;
    emit statusUpdated(QString("Atualizado %1 vezes").arg(++count));  // Emissão correta
}
