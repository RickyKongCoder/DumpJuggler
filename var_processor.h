#ifndef VAR_PROCESSOR_H
#define VAR_PROCESSOR_H

#include <list>
#include <iterator>
#include <QApplication>
#include <string>
#include "variables.h"
using namespace std;

typedef enum {
    RD_NULL,
    RD_START,
  RD_VARNAME,
  RD_VARVAL,
   RD_NEXTLINE,
  RD_END,
NUM_STATE
} ProcessState;

class Var_Processor : public QObject
{
    Q_OBJECT
private:
ProcessState state = RD_NULL;

list<char>  start_stack;
list<char>  varName_stack;
list<char>  varValue_stack;
list<char>  end_stack;
public:
    Var_Processor();

    void process(char str[7]);
signals:
    void alter_var(QString name,void*par,int type);
};

#endif // VAR_PROCESSOR_H
