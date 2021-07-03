#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QtSerialPort>
#include "var_processor.h"

#define DEFAULT_PORTNAME "com7"
#define DEFAULT_BAUDRATE Baud115200
#define DEFAULT_DATABITS Data8
#define DEFAULT_PARITY NoParity
#define DEFAULT_STOPBITS OneStop
#define DEFAULT_FLOWCONTROL NoFlowControl
class SerialHandler : public QSerialPort
{Q_OBJECT//when add this "Q_OBJECT" to class run qmake again
private:
    QString serial_str="";
public:
   SerialHandler(QObject *parent);
    SerialHandler(QString name=DEFAULT_PORTNAME,BaudRate br=DEFAULT_BAUDRATE,DataBits db=DEFAULT_DATABITS,Parity p=DEFAULT_PARITY,StopBits stp=DEFAULT_STOPBITS,FlowControl flow = DEFAULT_FLOWCONTROL);
    void update_portName();
    QString get_serial_str();
     Var_Processor var_process;
signals:
    void write_serial_str(QString str);
public slots:
    void receivedFromSerial();
    //    void setPortNameSlot(QString);
    //    void setBaudRateSlot(int);
    //    void openSlot(OpenMode);
    //    void writeSlot(QString);
    void InitSlot(QString, int, QString);
};
extern SerialHandler *m_serial;

#endif // SERIALHANDLER_H
