#ifndef VARIABLES_H
#define VARIABLES_H
#include <iostream>
#include <QApplication>
using namespace std;
typedef enum{
 INT_,
 FLOAT_,
 NUM_TYPES
}TYPES;
typedef enum{
    OLD,NEW,NUM_OLDNEW
} DATA_STATUS;
class Variables
{
private:
    union {
        double float_v[2];
        int int_v[2];
    }v ;
   QString name;

    bool changed;
    TYPES type;
public:
    Variables() = default;
    Variables(TYPES T,void*par);
    Variables(QString name,TYPES T,void*par);
    TYPES get_type() const;
    bool is_changed() const;
    int* get_int();
    double* get_float();
    QString get_name() const;
    void set_name(QString name){
        this->name = name;
    };
    void set_type(TYPES T){
        type = T;
    }


    void set_new(float i,TYPES T){

       if(T==INT_){
           v.int_v[NEW] = i;

       }
       else
           v.float_v[NEW] = i;
    }
    void set_old(float i,TYPES T){

       if(T==INT_)
           v.int_v[OLD] = i;
       else
           v.float_v[OLD] = i;
    }
    void change_var(float i,TYPES T){
        if(T==INT_){
            v.int_v[OLD] = v.int_v[NEW];
            v.int_v[NEW] = i;
        }
        else
        {
                    v.float_v[OLD] = v.float_v[NEW];
                    v.float_v[NEW] = i;
        }

    }
    float get_val(){
        if(type==INT_){
           return v.int_v[NEW];
        }else
            return v.float_v[NEW];
    }
    float get_old(){
        if(type==INT_){
           return v.int_v[OLD];
        }else
            return v.float_v[OLD];
    }
    void set(float i,TYPES T,DATA_STATUS d){

       if(T==INT_)
           v.int_v[d] = i;
       else
           v.float_v[d] = i;
    }

};

#endif // VARIABLES_H
