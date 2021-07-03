#ifndef DATAPLOT_H
#define DATAPLOT_H
//#include "plotingpanel.h"
#include <iostream>
//This class is abandoned
#include "qcustomplot.h"
using namespace std;

#define DEFAULT_XWIDTH 40

typedef enum{
    Relation,
    Realtime
}PlotMode;
typedef enum {

    MANUAL,
    AUTO
} ZoomMode;

#define DEFAULT_PLOT Realtime
class DataPlot : public QCustomPlot
{

Q_OBJECT
private:
//   QCustomPlot* plot;
//VPlotingPanel *panel;
QString name;
bool auto_scale;
bool selected = false;
PlotMode plotMode = DEFAULT_PLOT;
ZoomMode mode = AUTO;
QTime startTime;
QAction *AutoModeAction;
QAction *ManualModeAction;
QList<QString> varNames;
void createAction();
float lastPointKey;
signals:
void alterZoomModeShown(ZoomMode);
void pushSelected(QString);
void eraseSelected(QString);
void contextMenuTrigger(QContextMenuEvent *event);

public slots:
void selectedEvent(QString name, QMouseEvent *);
void alterZoomMode(ZoomMode);
void AutoModeToggle();
void ManualModeToggle();
void LegendModeToggle();

protected:
void contextMenuEvent(QContextMenuEvent *event);

public:
    DataPlot();
    void setname(QString name,bool auto_scale=true){
       this->name = name;
       this->auto_scale = auto_scale;

    }

    QCPRange rangeX;
    QCPRange rangeY;
    float x_width=DEFAULT_XWIDTH;
    float y_width=DEFAULT_XWIDTH;
    float getLastKey();
    void setLastKey(float);
    // void setPanel(VPlotingPanel *);
    QCPRange get_rangeX() const;
    void update_widthX();
    void update_widthY();
    PlotMode getMode() const;
    void setMode(PlotMode);
    bool isSelected();
    void set_rangeX();
    void select();
    void deselect();
    void setStartTime(QTime time);
    float getYRangeMax();
    float getYRangeMin();
    void addVar(QString);
    void removeVar(QString);
    QList<QString> getVarNames();
    ZoomMode getZoomMode();
    QTime& getStartTime();
    void merge(DataPlot*plt);
    void merge(DataPlot *plt1, DataPlot *plt2);
};
//extern vector<DataPlot> dataPlot;
#endif // DATAPLOT_H
