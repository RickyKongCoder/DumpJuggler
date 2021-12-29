#ifndef VPLOTINGPANEL_H
#define VPLOTINGPANEL_H
typedef enum { Axis_x, Axis_y } Axis;
typedef enum { EXPAND, CONSTRICT } AxisChange;

#include "dataplot.h"
#include "mainwindow.h"
#include "qcustomplot.h"
#include <QApplication>
#include <QVBoxLayout>

class MainWindow;

#define XWIDTH_INCRET 4;
#define YWIDTH_INCRET 4;
#define XWIDTH_INCRET_RATIO 0.07;
#define YWIDTH_INCRET_RATIO 0.07;

#define XAXIS_INDEX 1
#define YAXIS_INDEX 0
class VPlotingPanel : public QVBoxLayout
{
    Q_OBJECT;

private:
    DataPlot *set_dataPlot(DataPlot *newplot,
                           const QCP::Interactions &interactions,
                           PlotMode plt_mode = Realtime);
    void updateDataPlots(DataPlot *dataPlt, QCPRange xRange, QCPRange yRange);
    void alter_plotwidth(DataPlot *, Axis);
    vector<QString> selectedPlots;
    QAction *AutoModeAction;
    QAction *ManualModeAction;
    QAction *RemovePlotAction;
    QAction *RemoveGraphsAction;
    QAction *LegendAction;
    QAction *MergeAction;
    MainWindow *parentMW;
    void createAction();
    void add_dataGraph(DataPlot *, QString, quint32);
    void add_relationGraph(DataPlot *newplot, QList<QString> name_list);
    void realTimeUpdate(DataPlot *curr_plot, double key);
    void relationUpdate(DataPlot *curr_plot, double key);
private slots:
    void AutoModeToggle();
    void ManualModeToggle();
    void RemovePlot();

public:
    map<QString, DataPlot *> plots_ptr;

    VPlotingPanel();

    void setParentWindow(MainWindow *);
signals:
    void alterShowMode(ZoomMode);
    void alterModeToChild(ZoomMode);

public slots:
    void add_plot(QString);
    void add_SeperatePlot(QList<QString>);
    void add_MergePlot(QList<QString>);
    void add_RelationPlot(QList<QString>);
    void remove_plot(QString);
    void alter_plot_axises(Axis ax, AxisChange change);
    void alterPlotMode();
    void realtimeDataSlot();
    void pushSelected(QString);
    void eraseSelected(QString);
    void contextMenuHandle(QContextMenuEvent *);
    void MergePlots();
};

#endif // VPLOTINGPANEL_H
