#include "uithread.h"
#include <QtCore>
//QMutex in QtCore
UIThread::UIThread(QObject *parent) : QThread(parent)
{

}

void UIThread::run()
{
    for(int i=0;i<1000;i++){
        QMutex mutex;
        mutex.lock();
        if(this->Stop)
            break;
        mutex.unlock();
        emit NumberChanged(i);
        this->sleep(1);
    }
}
