#include "vartable.h"
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
VarTable::VarTable(const QStringList & horzHeaders)
{
 this->setColumnCount(horzHeaders.size());
 this->setHorizontalHeaderLabels(horzHeaders);
 connect(this,QOverload<QTableWidgetItem *>::of(&QTableWidget::itemChanged),this,&VarTable::on_itemChanged);
 createAction();
}
void VarTable::createAction()
{
    Plot = new QAction(tr("Plot"), this);
    Plot->setStatusTip(tr("Plot"));
    connect(Plot, SIGNAL(triggered()), this, SLOT(Single_Plot()));

    Plot_Seperate = new QAction(tr("Plot Seperately"), this);
    Plot_Seperate->setStatusTip(tr("Plot Seperately"));
    connect(Plot_Seperate, SIGNAL(triggered()), this, SLOT(Seperate_Plot()));

    Plot_Together = new QAction(tr("Plot Together"), this);
    Plot_Together->setStatusTip(tr("Plot Together"));
    connect(Plot_Together, SIGNAL(triggered()), this, SLOT(Merge_Plot()));

    DirectRelational = new QAction("", this);
    connect(DirectRelational, SIGNAL(triggered()), this, SLOT(DirectRelationalPlot()));
    SwitchedRelational = new QAction(".", this);
    connect(SwitchedRelational, SIGNAL(triggered()), this, SLOT(SwitchedRelationalPlot()));
}
void VarTable::on_itemChanged(QTableWidgetItem *item) {

    vector<QTableWidgetItem *>::const_iterator iter = std::find(checkboxInTable.begin(), checkboxInTable.end(), item);
    if (iter != checkboxInTable.end()) {
        int row_num = (*iter)->row();
        QString var_name = this->item(row_num, 0)->text();
        if ((*iter)->checkState() == Qt::Checked) {
          emit  add_plot(var_name,PLOT_COLOR);
        } else {
          emit  remove_plot(var_name);
        }
    }

}
void VarTable::write_serial_var(Variables v) {
    QString v_name =v.get_name();
    QString v_value = (v.get_type() == INT_) ? QString::number(v.get_int()[NEW]) : QString::number(v.get_float()[NEW]);
    QList<QTableWidgetItem *> item_list = this->findItems(v_name, Qt::MatchExactly);
    if (item_list.empty()) {
        this->insertRow(this->rowCount());
        QTableWidgetItem *checkbox_item = new QTableWidgetItem();
        checkbox_item->setCheckState(Qt::Unchecked);
        checkboxInTable.push_back(checkbox_item);
        QTableWidgetItem *name = new QTableWidgetItem(v_name);

        QTableWidgetItem *value = new QTableWidgetItem(v_value);
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);
        value->setFlags(value->flags() & ~Qt::ItemIsEditable);
        this->setItem(this->rowCount() - 1, 0,
                               name);  // don't worry about memory leaks, they will take care of it.... :D
        this->setItem(this->rowCount() - 1, 1, value);
        this->setItem(this->rowCount() - 1, 2, checkbox_item);
    } else {
        QList<QTableWidgetItem *> list =
            this->findItems(v_name, Qt::MatchExactly);
        if (v.get_val() != v.get_old()) {
            this->setItem(list[0]->row(), 1, new QTableWidgetItem(v_value));
        }
    }
}
QList<QTableWidgetItem *> VarTable::getColumnSelectedItem(int col)
{
    QList<QTableWidgetItem *> selected_items = selectedItems();
    for (int i = 0; i < selected_items.size(); i++) {
        if (selected_items[i]->column() != 0) { //very ugly optimise later
            selected_items.removeAt(i);
        }
    }

    return selected_items;
}
QList<QString> VarTable::getColumnSelectedText(int col)
{
    QList<QTableWidgetItem *> selected_items = getColumnSelectedItem(col);
    QList<QString> select_varName = {};
    for (int i = 0; i < selected_items.size(); i++) {
        select_varName.push_back(selected_items[i]->text());
    }
    return select_varName;
}

void VarTable::contextMenuEvent(QContextMenuEvent *event)
{ // varTable1->setContextMenuPolicy(Qt::CustomContextMenu);

    qDebug() << "contextMenu of table widget" << endl;

    QList<QString> select_varName = getColumnSelectedText(0);
    QMenu menu;
    QMenu relation{"Relation Plot"};

    if (select_varName.size() > 1) {
        QString directAction_ = select_varName[0] + " over " + select_varName[1];
        QString switchedAction_ = select_varName[1] + " over " + select_varName[0];
        menu.addAction(Plot_Seperate);
        menu.addAction(Plot_Together);
        if (select_varName.size() == 2) {
            menu.addMenu(&relation);
            DirectRelational->setText(directAction_);
            SwitchedRelational->setText(switchedAction_);
            relation.addAction(DirectRelational);
            relation.addAction(SwitchedRelational);
        }

    } else if (select_varName.size() == 1) {
        menu.addAction(Plot);
    } else {
    }
    menu.exec(event->globalPos());
}
void VarTable::Single_Plot()
{
    QList<QTableWidgetItem *> selected_items = getColumnSelectedItem(0);

    emit singleplotSignal(selected_items[0]->text(), QColor(255, 0, 0, 255));
}
void VarTable::Merge_Plot()
{
    QList<QString> select_varName = getColumnSelectedText(0);
    emit mergeplotSignal(select_varName, QColor(255, 0, 0, 255));
}
void VarTable::Relational_Plot()
{
    QList<QString> select_varName = getColumnSelectedText(0);

    emit relationalplotSignal(select_varName);
}
void VarTable::Seperate_Plot()
{
    QList<QString> select_varName = getColumnSelectedText(0);

    emit seperateplotSignal(select_varName, QColor(255, 0, 0, 255));
}
void VarTable::DirectRelationalPlot()
{
    QList<QString> select_varName = getColumnSelectedText(0);
    qDebug() << select_varName[0] << "|" << select_varName[1] << endl;
    emit relationalplotSignal(select_varName);
}
void VarTable::SwitchedRelationalPlot()
{
    QList<QString> select_varName = getColumnSelectedText(0);
    qDebug() << select_varName[0] << '|' << select_varName[1] << endl;
    QString name2 = select_varName[1];
    select_varName[1] = select_varName[0];
    select_varName[0] = name2;
    qDebug() << select_varName[0] << '|' << select_varName[1] << endl;

    emit relationalplotSignal(select_varName);
}
