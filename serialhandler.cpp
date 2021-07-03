#include "serialhandler.h"
#include <QtSerialPort>
#include <iostream>
#include <stack>
#include <iterator>
#include <cstring>
using namespace std;
SerialHandler *m_serial = new SerialHandler;

SerialHandler:: SerialHandler(QString name,BaudRate br,DataBits db,Parity p,StopBits stp,FlowControl flow)
{
    this->setPortName(name);
    this->setBaudRate(br);
    this->setDataBits(db);
    this->setParity(p);
    this->setStopBits(stp);
    this->setFlowControl(flow);
}

SerialHandler:: SerialHandler(QObject*parent):QSerialPort(parent){
    this->setPortName(DEFAULT_PORTNAME);
    this->setBaudRate(DEFAULT_BAUDRATE);
    this->setDataBits(DEFAULT_DATABITS);
    this->setParity(DEFAULT_PARITY);
    this->setStopBits(DEFAULT_STOPBITS);
    this->setFlowControl(DEFAULT_FLOWCONTROL);
}

int debug = 0;

void SerialHandler::receivedFromSerial(){
    //this->readLine(linedata,);
    // qDebug() << "receiving shits******************888" << endl;
    QString str = "";
    int size = 7;
    char str_[size];
    while (this->canReadLine()) {
        this->readLine(str_, size); //use readline if use canReadLine
        var_process.process(str_);  //checked
        str += str_;
    }
    //  qDebug() << str << endl;

    emit write_serial_str(str);
}
QString SerialHandler::get_serial_str(){
    
    
}

void SerialHandler::InitSlot(QString portname, int BaudRate, QString init_msg)
{
    this->setPortName(portname);
    this->setBaudRate(BaudRate);
    this->open(QIODevice::ReadWrite);
    //  this->write(init_msg); "leave it for now, it is not useful" , the Pussy Destroyer
}

//void setBaudRateSlot(int);
//void openSlot(OpenMode);
//void writeSlot(QString);
