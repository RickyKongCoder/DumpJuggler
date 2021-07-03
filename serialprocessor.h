#ifndef SERIALPROCESSOR_H
#define SERIALPROCESSOR_H
#include <QApplication>
#include "variables_container.h"
class SerialProcessor
{
QString * buffer_ref;
public:
    SerialProcessor(QString*buffer_ref);

};

#endif // SERIALPROCESSOR_H
