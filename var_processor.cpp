#include "var_processor.h"
#include "algorithm"
#include "variables_container.h"
#include <cstring>
#include <math.h>
#include <QDebug>
Var_Processor::Var_Processor()
{
    connect(this,SIGNAL(alter_var(QString,void*,int)),&var_container,SLOT(variable_changed(QString,void*,int)));

}


QString list_to_cstring(list<char> clist) {//checked in compiler
    QString string1 ={};
    int index=0;
    for(list<char>::const_iterator i = clist.begin();i!=clist.end();i++,index++){
    string1.push_back(*i);
    }
    return string1;

}
float list_to_float(list<char> clist){//checked in compiler
    float fvalue=0;
    int index=1;
    bool dot_exist = false;
    for(list<char>::const_iterator i = clist.begin();i!=clist.end();i++,index++){
        if(*i!='.'){

        fvalue+=(*i-'0')*pow(0.1,index);
        }
        else{
        fvalue *=pow(10,index-1);
        index = 0;
        }


    }
    return fvalue;
}
int list_to_int(list<char> clist){//checked

    int Ivalue=0;
    int index=0;
    int size = clist.size();
    bool dot_exist = false;

    for(;index<size;index++){
    Ivalue += (clist.back()-'0')*pow(10,index);
    clist.pop_back();
    }

    return Ivalue;
 //   return fvalue;
}

void Var_Processor::process(char str[7]){
  // char str_debug[7]={};
//cout<<str<<"size "<<strlen(str)<<endl;
//cout<<"__"<<endl;
for(uint i=0;i<strlen(str);i++){
switch (str[i]) {
        case '/':
        state = RD_VARNAME;
        break;
        case ':':
        if(state == RD_VARVAL){//checked
          //  cout<<"I see : and i am processing it"<<endl;
            //that means last variable is packed finished
            bool is_float = (std::find(varValue_stack.begin(), varValue_stack.end(), '.') != varValue_stack.end());
            if(is_float){
               float value = list_to_float(varValue_stack);
               // qDebug() << "Name" << QString(list_to_cstring(varName_stack))
               //        << " varValue_stack_content" << value << (int) FLOAT_ << endl;

               emit alter_var(list_to_cstring(varName_stack), &value, (int) FLOAT_);
               //  qDebug()<<"list_to_cstring"<<list_to_cstring(varName_stack)<<endl;
               varName_stack.clear();
               varValue_stack.clear();

            }
            else{//checked
                int value = list_to_int(varValue_stack);
                //                qDebug() << "Name" << QString(list_to_cstring(varName_stack))
                //                         << " varValue_stack_content" << value << endl;
                emit alter_var(list_to_cstring(varName_stack), &value, (int) INT_);
                //   qDebug()<<"list_to_cstring"<<list_to_cstring(varName_stack)<<endl;
                varValue_stack.clear();
                varName_stack.clear();

            }
            state = RD_VARNAME;
        }
        else if(RD_VARNAME){
            state = RD_VARVAL;
        }
        break;
        case '\n':
        state = RD_NEXTLINE;
        break;
        default:
        if(state == RD_VARNAME){
            varName_stack.push_back(str[i]);
        }
        else if(state == RD_VARVAL){
          //  cout<<"I am pushing the value"<<str[i]<<endl;
            varValue_stack.push_back(str[i]);
        }
        break;
    }

}

//cout<<str_debug<<endl;


}
