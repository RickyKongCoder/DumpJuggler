#include "variables_container.h"
#include <QDebug>
Variables_Container::Variables_Container() {}

void Variables_Container::variable_changed(QString name, void *par, int type)
{
    //cout<<"accept var change"<<endl;
    //cout<<"name:"<<name<<endl;
    //cout<<(name).erase(0,1)<<endl;//ugly fix
    if (exist(name)) {
        (type == INT_) ? this->container[name].change_var(*(int *) (par), (TYPES) type)
                       : this->container[name].change_var(*(float *) (par), (TYPES) type);

        emit alter_var_combo(this->container[name]);
        /*  qDebug() << "Myname using get_name" << this->container[name].get_name() << " "
                 << this->container[name].get_int()[NEW] << "  |  "
                 << this->container[name].get_float()[NEW] << endl;*/
    } else {
        Variables var;
        var.set_name(name);
        var.set_type((TYPES) type);
        (type == INT_) ? var.change_var(*(int *) (par), (TYPES) type)
                       : var.change_var(*(float *) (par), (TYPES) type);
        this->container.insert(this->container.begin(), pair<QString, Variables>(name, var));
        //  cout<<"Myname using get_name"<<this->container[name].get_name()<<" "<<this->container[name].get_int()[NEW]<<"  |  "<<this->container[name].get_float()[NEW]<<endl;
        emit alter_var_combo(this->container[name]);
    }
}

map<QString, Variables> &Variables_Container::content()
{
    return container;
}

bool Variables_Container::exist(QString varName)
{
    return !(this->container.find(varName) == this->container.end());
}

Variables_Container  var_container;
