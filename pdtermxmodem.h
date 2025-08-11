#ifndef PDTERMXMODEM_H
#define PDTERMXMODEM_H

#include <QObject>

class PdTermXmodem : public QObject
{
    Q_OBJECT
public:
    explicit PdTermXmodem(QObject *parent = nullptr);

signals:
};

#endif // PDTERMXMODEM_H
