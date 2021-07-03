#ifndef UITHREAD_H
#define UITHREAD_H

#include <QThread>
#include <QObject>

class UIThread : public QThread
{
    Q_OBJECT
public:
    explicit UIThread(QObject *parent = nullptr);
    void run();
    bool Stop;


signals:
    void NumberChanged(int);
public slots:

};

#endif // UITHREAD_H
