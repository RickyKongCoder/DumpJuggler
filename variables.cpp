#include "variables.h"
Variables::Variables(TYPES type, void *par){
    this->type = type;
    this->changed = false;
    (type==INT_)?(v.int_v[OLD]=v.int_v[NEW]= *(int*)par):(v.float_v[OLD] = v.float_v[NEW] = *(float*)par);

}
Variables::Variables(QString name,TYPES type, void *par){
    this->type = type;
    this->changed = false;
    this->name = name;
    (type==INT_)?(v.int_v[OLD]=v.int_v[NEW]= *(int*)par):(v.float_v[OLD] = v.float_v[NEW] = *(float*)par);

}


TYPES Variables::get_type() const{

    return type;
};
bool Variables::is_changed() const{
  return v.int_v[NEW] ==v.int_v[OLD];
};
int* Variables::get_int()  {
    return v.int_v;

};
double* Variables::get_float() {
    return v.float_v;
}

QString Variables:: get_name() const{
    return name;
}
