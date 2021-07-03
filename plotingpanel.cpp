#include "plotingpanel.h"
#include "PenColor.h"
#include "mainwindow.h"
#include "variables_container.h"
#include <QMenu>
#include <QRandomGenerator>
#include <QString>
VPlotingPanel::VPlotingPanel()
{
    createAction();
}

void VPlotingPanel::add_plot(QString graph_name)
{
    DataPlot *newplot = new DataPlot;
    set_dataPlot(newplot,
                 QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectLegend | QCP::iSelectOther);
    add_dataGraph(newplot, graph_name);
}
void VPlotingPanel::add_SeperatePlot(QList<QString> plotname_list)
{
    for (int i = 0; i < plotname_list.size(); i++) {
        add_plot(plotname_list[i]);
    }
}
void VPlotingPanel::add_MergePlot(QList<QString> merge_list)
{
    DataPlot *newplot = new DataPlot;
    set_dataPlot(newplot,
                 QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectLegend | QCP::iSelectOther);

    while (!merge_list.isEmpty()) {
        add_dataGraph(newplot, merge_list.back());
        merge_list.pop_back();
    }
}
void VPlotingPanel::remove_plot(QString name) {
    this->removeWidget(plots_ptr[name]);

    delete plots_ptr[name];
    plots_ptr.erase(name);
    selectedPlots.erase(std::remove(selectedPlots.begin(), selectedPlots.end(), name),
                        selectedPlots.end());
}
void VPlotingPanel::setParentWindow(MainWindow *mw)
{
    parentMW = mw;
}

DataPlot *VPlotingPanel::set_dataPlot(DataPlot *newplot, const QCP::Interactions &interactions)
{
    QCPTextElement *title = new QCPTextElement(newplot);
    newplot->setStartTime(QTime::currentTime());
    newplot->getStartTime().start();

    this->insertWidget(0, newplot);
    QString plot_index = QString::number(plots_ptr.size() + 1);
    QString title_name = QString("Plot") + plot_index;
    newplot->setObjectName(title_name);
    newplot->setname(title_name); //at first the graph name is same as var name
    title->setText(title_name);
    title->setFont(QFont("sans", 12, QFont::Bold));
    plots_ptr.insert(std::pair<QString, DataPlot *>(title_name, newplot));
    qDebug() << "set DataPLot  title_name" << title_name << endl;
    newplot->plotLayout()->insertRow(0);  // insert an empty row above the axis rect
    newplot->plotLayout()->addElement(0, 0, title);
    newplot->setInteractions(interactions);
    connect(this, &VPlotingPanel::alterModeToChild, newplot, &DataPlot::alterZoomMode);
    connect(newplot, &DataPlot::pushSelected, this, &VPlotingPanel::pushSelected);
    connect(newplot, &DataPlot::eraseSelected, this, &VPlotingPanel::eraseSelected);
    connect(newplot, &DataPlot::contextMenuTrigger, this, &VPlotingPanel::contextMenuHandle);
    connect(AutoModeAction, SIGNAL(triggered()), newplot, SLOT(AutoModeToggle()));
    connect(ManualModeAction, SIGNAL(triggered()), newplot, SLOT(ManualModeToggle()));
    connect(MergeAction, SIGNAL(triggered()), newplot, SLOT(MergePlots()));
    connect(LegendAction, SIGNAL(triggered()), newplot, SLOT(LegendModeToggle()));
    connect(RemovePlotAction, SIGNAL(triggered()), this, SLOT(RemovePlot()));
}
void VPlotingPanel::add_dataGraph(DataPlot *newplot, QString graph_name)
{
    newplot->addGraph(); // blue line
    int new_graphIndex = newplot->graphCount() - 1;
    newplot->graph(new_graphIndex)->setPen(QPen(randomBrightColor(255)));
    newplot->graph(new_graphIndex)->setName(graph_name);
}
void VPlotingPanel::alterPlotMode()
{
    //pretty much useless
}
void VPlotingPanel::realtimeDataSlot()
{
    // qDebug() << "RealPLoting" << endl;

    for (map<QString, DataPlot *>::const_iterator i = plots_ptr.begin(); i != plots_ptr.end(); i++) {
        DataPlot *curr_plot = i->second;
        // qDebug() << "Value added |" << var_container.content()[i->first].get_val() << endl;
        double key = curr_plot->getStartTime().elapsed()
                     / 1000.0;                     // time elapsed since start of demo, in seconds
        if (key - curr_plot->getLastKey() > 0.002) // at most add point every 2 ms
        {
            for (int index = 0; index < curr_plot->graphCount(); index++) {
                curr_plot->graph(index)
                    ->addData(key,
                              var_container.content()[curr_plot->graph(index)->name()].get_val());
            }
            curr_plot->setLastKey(key);
        }

        bool foundRange;
        if (curr_plot->getZoomMode() == AUTO) {
            curr_plot->xAxis->setRangeUpper(key);
            curr_plot->xAxis->setRangeLower(key - curr_plot->x_width);
            curr_plot->rangeY.lower = -abs(curr_plot->graph(0)
                                               ->getValueRange(foundRange,
                                                               QCP::sdBoth,
                                                               curr_plot->rangeX)
                                               .lower)
                                      - curr_plot->y_width * 0.07;
            curr_plot->rangeY.upper = abs(curr_plot->graph(0)
                                              ->getValueRange(foundRange,
                                                              QCP::sdBoth,
                                                              curr_plot->rangeX)
                                              .upper)
                                      + curr_plot->y_width * 0.07;
            curr_plot->yAxis->setRange(curr_plot->rangeY.lower, curr_plot->rangeY.upper);
        }
        updateDataPlots(curr_plot, curr_plot->xAxis->range(), curr_plot->yAxis->range());
        curr_plot->replot();
    }
}
void VPlotingPanel::updateDataPlots(DataPlot *dataPlt, QCPRange xRange, QCPRange yRange)
{
    dataPlt->rangeX = xRange;
    dataPlt->rangeY = yRange;
    //    qDebug()<<"XAixs"<<dataPlt->rangeX<<endl;
    //    qDebug()<<"YAixs"<<dataPlt->rangeY<<endl;
    dataPlt->update_widthX();
    dataPlt->update_widthY();
}
void VPlotingPanel::alter_plotwidth(DataPlot *dataplot, Axis ax)
{
    if (ax == Axis_y) {
        float upper = (dataplot->rangeY.upper + dataplot->rangeY.lower) / 2 + dataplot->y_width / 2;
        float lower = (dataplot->rangeY.upper + dataplot->rangeY.lower) / 2 - dataplot->y_width / 2;
        dataplot->yAxis->setRangeUpper(upper);
        dataplot->yAxis->setRangeLower(lower);
    } else {
        float upper = (dataplot->rangeX.upper + dataplot->rangeX.lower) / 2 + dataplot->x_width / 2;
        float lower = (dataplot->rangeX.upper + dataplot->rangeX.lower) / 2 - dataplot->x_width / 2;
        dataplot->xAxis->setRangeUpper(upper);
        dataplot->xAxis->setRangeLower(lower);
    }
}
void VPlotingPanel::alter_plot_axises(Axis ax, AxisChange change)
{
    for (map<QString, DataPlot *>::const_iterator iter = plots_ptr.begin(); iter != plots_ptr.end();
         iter++) {
        DataPlot *plt = iter->second;
        if (plt->isSelected()) {
            bool foundRange;
            if (ax == Axis_x && change == EXPAND) {
                qDebug() << plt->x_width << "|" << plt->rangeX.upper;

                plt->x_width += plt->x_width * XWIDTH_INCRET_RATIO;
                alter_plotwidth(plt, Axis_x);
            } else if (ax == Axis_x && change == CONSTRICT) {
                plt->x_width -= plt->x_width * XWIDTH_INCRET_RATIO;
                alter_plotwidth(plt, Axis_x);
            } else if (ax == Axis_y && change == EXPAND) {
                plt->y_width -= plt->y_width * YWIDTH_INCRET_RATIO;
                alter_plotwidth(plt, Axis_y);
            } else if (ax == Axis_y && change == CONSTRICT) {
                plt->y_width += plt->y_width * YWIDTH_INCRET_RATIO;
                alter_plotwidth(plt, Axis_y);
            }
        }
    }
}
void VPlotingPanel::MergePlots()
{
    //bra...
}
void VPlotingPanel::createAction()
{
    AutoModeAction = new QAction(tr("AUTO ZOOM"), this);
    AutoModeAction->setStatusTip(tr("AUTO ZOOM"));
    AutoModeAction->setCheckable(true);

    ManualModeAction = new QAction(tr("MANUAL ZOOM"), this);
    ManualModeAction->setStatusTip(tr("MANUAL ZOOM"));
    ManualModeAction->setCheckable(true);
    LegendAction = new QAction(tr("Show Variable Name"), this);
    LegendAction->setStatusTip(tr("Show Variable Name"));
    LegendAction->setCheckable(true);
    RemovePlotAction = new QAction(tr("Remove "), this);
    RemovePlotAction->setStatusTip(tr("Remove "));
    MergeAction = new QAction(tr("Merge Action"), this);
    MergeAction->setStatusTip(tr("Merge Action"));

    // MergeAction->setCheckable(true);
}

void VPlotingPanel::AutoModeToggle()
{ //useless for now
}
void VPlotingPanel::ManualModeToggle()
{ //useless for now
}

void VPlotingPanel::pushSelected(QString str)
{
    bool found = std::find(selectedPlots.begin(), selectedPlots.end(), str) != selectedPlots.end();
    if (!found) //put this to play safe
        selectedPlots.push_back(str);
}
void VPlotingPanel::eraseSelected(QString str)
{
    selectedPlots.erase(std::remove(selectedPlots.begin(), selectedPlots.end(), str),
                        selectedPlots.end());
}
void VPlotingPanel::RemovePlot()
{
    qDebug() << "Removing Plots" << endl;
    while (!selectedPlots.empty()) {
        remove_plot(selectedPlots.back());
    }
}

void VPlotingPanel::contextMenuHandle(QContextMenuEvent *event)
{
    QMenu menu;
    if (selectedPlots.size() > 1) {
        AutoModeAction->setChecked(false);
        ManualModeAction->setChecked(false);
        LegendAction->setChecked(false);
        menu.addAction(AutoModeAction);
        menu.addAction(ManualModeAction);
        menu.addAction(RemovePlotAction);
        menu.addAction(LegendAction);
        menu.addAction(MergeAction);

    } else if (selectedPlots.size() == 1) {
        QString name = selectedPlots.at(0);
        DataPlot *curr_plot = plots_ptr[name];
        AutoModeAction->setChecked(curr_plot->getZoomMode() == AUTO);
        ManualModeAction->setChecked(curr_plot->getZoomMode() == MANUAL);
        LegendAction->setChecked(curr_plot->legend->visible());
        menu.addAction(AutoModeAction);
        menu.addAction(ManualModeAction);
        menu.addAction(RemovePlotAction);
        menu.addAction(LegendAction);
    }
    menu.exec(event->globalPos());

    qDebug() << "selected size" << selectedPlots.size() << endl;
}
