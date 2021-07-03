#include "dataplot.h"

DataPlot::DataPlot()
{  qDebug()<<"connected signal"<<endl;
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->legend->setFont(legendFont);
    this->legend->setSelectedFont(legendFont);
    this->legend->setSelectableParts(
        QCPLegend::spItems); // legend box shall not be selectable, only legend items
    this->legend->setVisible(true);

    connect(this,
            QOverload<QString, QMouseEvent *>::of(&DataPlot::mouseDoubleClick),
            this,
            &DataPlot::selectedEvent);
}

void DataPlot::select(){
    if (!selected) {
        selected = true;
        this->setBackground(QColor(240, 240, 240));
        qDebug() << "i pushed name" << name << endl;
        emit pushSelected(name);
    }
}
void DataPlot::deselect()
{
    if (selected) {
        selected = false;
        this->setBackground(Qt::white);
        emit eraseSelected(name);
    }
}
bool DataPlot::isSelected(){
return selected;
}
void DataPlot::update_widthX(){
   x_width =  this->rangeX.upper - this->rangeX.lower;
}

void DataPlot::update_widthY(){
    y_width = this->rangeY.upper- this->rangeY.lower;
}
// /vector<DataPlot> dataPlot;
PlotMode DataPlot::getMode() const{
    return plotMode;
}
void DataPlot::setMode(PlotMode pltMode){
     plotMode = pltMode;

}
QTime& DataPlot::getStartTime(){
    return startTime;
}
void DataPlot::setStartTime(QTime time){
    startTime = time;
}
float DataPlot::getLastKey(){
    return lastPointKey;
}
void DataPlot::setLastKey(float f){
   lastPointKey =f;


}
void DataPlot::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "contextMenu of DataPlot trigger " << endl;
    QCPGraphDataContainer *contain_ptr = this->graph(0)->data().get();
    QCPGraphDataContainer::const_iterator iterator = contain_ptr->at(contain_ptr->size() - 1);
    this->select();
    qDebug() << "Time:" << iterator->sortKey() << endl;
    emit contextMenuTrigger(event);
}

void DataPlot::merge(DataPlot *plt)
{
    //merge this and plt
    //assume have n graphs,
    QMenu menu(this);
}

void DataPlot::merge(DataPlot *plt1, DataPlot *plt2)
{
    ///todays progress
}
void DataPlot::selectedEvent(QString name, QMouseEvent *event)
{
    cout << "selectEvent Triggered" << endl;
    if (selected) // exist
    {
        this->deselect();

    } else {
        this->select();
    }
}
void DataPlot::alterZoomMode(ZoomMode mode)
{
    //    if (this->selected) {
    //        mode = (mode == AUTO) ? MANUAL : AUTO;
    //        emit alterZoomModeShown(mode);
    //    }
    this->mode = mode;
}
ZoomMode DataPlot::getZoomMode()
{
    return mode;
}
void DataPlot::AutoModeToggle()
{
    if (selected)
        mode = AUTO;
}

void DataPlot::ManualModeToggle()
{
    if (selected)

        mode = MANUAL;
}
void DataPlot::LegendModeToggle()
{
    if (selected) {
        this->legend->setVisible(!this->legend->visible());
    }
}
float DataPlot::getYRangeMax()
{
    float maxY = 0;
    for (int i = 0; i < this->graphCount(); i++) {
        bool foundRange = true;
        float upper = this->graph(i)->getValueRange(foundRange, QCP::sdBoth, this->rangeX).upper;
        if (upper > maxY) {
            maxY = upper;
        };
    }
    return abs(maxY) + this->y_width * 0.07;
}
float DataPlot::getYRangeMin()
{
    float minY = 0;
    for (int i = 0; i < this->graphCount(); i++) {
        bool foundRange = true;
        float lower = this->graph(i)->getValueRange(foundRange, QCP::sdBoth, this->rangeX).lower;
        if (lower < minY) {
            minY = lower;
        };
    }
    return -abs(minY) - this->y_width * 0.07;
}
void DataPlot::addVar(QString name)
{
    varNames.push_back(name);
};
void DataPlot::removeVar(QString name)
{
    varNames.removeAll(name);
}

QList<QString> DataPlot::getVarNames()
{
    return varNames;
}
//void DataPlot::setPanel(VPlotingPanel *vpp)
//{
//    panel = vpp;
//}
