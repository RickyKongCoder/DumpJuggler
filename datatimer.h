#ifndef DATATIMER_H
#define DATATIMER_H
#include <QTimer>


class DataTimer:public QTimer
{
    Q_OBJECT
public:
    DataTimer();

signals:

public slots:
};
extern DataTimer dataTimer;

#endif // DATATIMER_H
