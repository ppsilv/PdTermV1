#ifndef __WORKER__
#define __WORKER__


#include <QObject>
#include <QTimer>

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

signals:
    void statusUpdated(const QString &message);  // Signal corrigido

private slots:
    void updateStatus();

private:
    QTimer *m_timer;
};
#endif
