#ifndef VARTABLE_H
#define VARTABLE_H

#include <QTableWidget>
#include <vector>
#include "variables.h"
#include <algorithm>
using namespace std;
#include <QColor>
#define PLOT_COLOR QColor(200, 0, 0, 255)

class VarTable : public QTableWidget
{
    Q_OBJECT
private:
    QAction *Plot;
    QAction *Plot_Seperate;
    QAction *Plot_Together;
    //  QAction *Relation_Plot;
    QAction *DirectRelational;
    QAction *SwitchedRelational;

    void createAction();
    QList<QTableWidgetItem *> getColumnSelectedItem(int col);
    QList<QString> getColumnSelectedText(int col);

public:
    VarTable() = default;
    VarTable(const QStringList & horzHeaders);
    vector<QTableWidgetItem *> checkboxInTable;
    vector<qint32> selected_varRow;

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void remove_plot(QString);
    void add_plot(QString,QColor);
    void singleplotSignal(QString, QColor);
    void relationalplotSignal(QList<QString>);
    void mergeplotSignal(QList<QString>, QColor);
    void seperateplotSignal(QList<QString>, QColor);
public slots:
    void on_itemChanged(QTableWidgetItem *);
    void write_serial_var(Variables v);
    void Relational_Plot();
    void Merge_Plot();
    void Seperate_Plot();
    void Single_Plot();
    void DirectRelationalPlot();
    void SwitchedRelationalPlot();
};

#endif // VARTABLE_H
