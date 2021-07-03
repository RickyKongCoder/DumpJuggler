#ifndef VARIABLES_CONTAINER_H
#define VARIABLES_CONTAINER_H
#include <QApplication>
#include "variables.h"
#include <map>
#include <iterator>
#include <iostream>
using namespace std;

class Variables_Container: public QObject
{
Q_OBJECT
    map<QString,Variables> container;

public:
    Variables_Container();
    bool exist(QString varName);
    map<QString,Variables>& content();

signals:
    void alter_var_combo(Variables);
public slots:
    void variable_changed(QString name, void *par, int type);
};
extern Variables_Container var_container;

#endif // VARIABLES_CONTAINER_H
