#include "mainwindow.h"
#include "mythread.h"
#include "serialhandler.h"
#include <iostream>
#include <QApplication>
#include <QSerialPortInfo>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSerialPortInfo info_serial;

    MainWindow w;
    //    MyThread mThread;
    //    mThread.start();
    w.show();
    cout<<"Hellow World"<<endl;

    QList<QSerialPortInfo> serialinfos;
    QList<QSerialPortInfo>::iterator qlist;
    serialinfos = info_serial.availablePorts();

    for(qlist = serialinfos.begin();
            qlist != serialinfos.end();
            qlist++)
    {

     //   ui->Port_combo->setItemText(qlist-serialinfos.begin(),qlist->portName());
        cout<<qlist->portName().toUtf8().constData()<<endl;
    }
    return a.exec();
}
