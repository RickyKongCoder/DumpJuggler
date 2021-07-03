#include "mainwindow.h"

#include <QtSerialPort>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "dataplot.h"
#include "datatimer.h"
#include "serialhandler.h"
#include "ui_mainwindow.h"
#include "variables.h"
#include "variables_container.h"


#define QSTR_To_STR(str)    str.toStdString().erase(0,1)

#define DATA_LAYOUT_STRETH 1, 3
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    QStringList list = {"Variable Name","Value","plot"};

    createActions();
    createMenus();
    setWindowTitle(tr("DumpJuggler"));
    dataHorLayout = new QHBoxLayout;
    ui->verticalLayout_2->insertLayout(2, dataHorLayout);
    varTable1 = new VarTable(list);
    vdataplot = new VPlotingPanel;
    vdataplot->setParentWindow(this);
    dataHorLayout->addWidget(varTable1);
    dataHorLayout->addLayout(vdataplot);
    dataHorLayout->setStretch(DATA_LAYOUT_STRETH);
    init_serial_combo();
    //  m_serial->moveToThread(&workerThread);
    // var_container.moveToThread(
    //  &workerThread); //make sure var_container and m_serial are in the same thread hahahahahah

    connect(m_serial, SIGNAL(readyRead()), m_serial, SLOT(receivedFromSerial()));
    connect(&var_container,
            SIGNAL(alter_var_combo(Variables)),
            varTable1,
            SLOT(write_serial_var(Variables)));
    connect(this,
            SIGNAL(expand_plot_axises(Axis, AxisChange)),
            vdataplot,
            SLOT(alter_plot_axises(Axis, AxisChange)));
    connect(&dataTimer, SIGNAL(timeout()), vdataplot, SLOT(realtimeDataSlot()));
    connect(varTable1,
            SIGNAL(singleplotSignal(QString, QColor)),
            vdataplot,
            SLOT(add_plot(QString)));
    connect(varTable1,
            SIGNAL(seperateplotSignal(QList<QString>, QColor)),
            vdataplot,
            SLOT(add_SeperatePlot(QList<QString>)));
    connect(varTable1,
            SIGNAL(mergeplotSignal(QList<QString>, QColor)),
            vdataplot,
            SLOT(add_SeperatePlot(QList<QString>)));
    connect(varTable1, SIGNAL(remove_plot(QString)), vdataplot, SLOT(remove_plot(QString)));
    connect(this, &MainWindow::init_serial_signal, m_serial, &SerialHandler::InitSlot);
    connect(this,
            QOverload<>::of(&MainWindow::alter_plotMode),
            vdataplot,
            &VPlotingPanel::alterPlotMode);
    //workerThread.start();
    // "the new way of doing things" , quote from the Pussy Destroyer
    dataTimer.start();
}

MainWindow::~MainWindow() {
	delete ui;
	m_serial->close();
	workerThread.quit();
	workerThread.wait();
	delete m_serial;
}

void MainWindow::alterShowMode(ZoomMode zm)
{
    (zm == AUTO) ? ui->ZoomModeButton->setText("AUTO") : ui->ZoomModeButton->setText("MANUAL");
}

void MainWindow::init_serial_combo()
{
    QSerialPortInfo info_serial;
    QList<QSerialPortInfo> serialinfos;
    QList<QSerialPortInfo>::iterator qlist;
    serialinfos = info_serial.availablePorts();
    ui->Port_combo->clear();
    for (qlist = serialinfos.begin(); qlist != serialinfos.end(); qlist++) {
        ui->Port_combo->addItem(qlist->portName());
    }
}

bool isNumber(const string &str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}

void MainWindow::init_serial()
{
    // init serial communication
    if (isNumber(ui->baudRate1->text().toUtf8().constData())
        && ui->Port_combo->currentText() != "Serial Port" && ui->Port_combo->currentText() != "") {
        //        m_serial->setPortName(ui->Port_combo->currentText());
        //        m_serial->setBaudRate(ui->baudRate1->text().toInt());
        //		m_serial->open(QIODevice::ReadWrite);
        //		m_serial->write("Init");
        emit init_serial_signal(ui->Port_combo->currentText(),
                                ui->baudRate1->text().toInt(),
                                "Init");
        if (!workerThread.isRunning())
            workerThread.start();
    } else {
        ui->serialWarning->setText("Please enter valid baudrate and select correct Port name");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    init_serial();
}

void MainWindow::customplot_clicked(QString obj_name, QMouseEvent *event)
{
    cout << "clicking" << (obj_name.toStdString()) << endl;
}

void MainWindow::on_pushButton_clicked()
{
    int exception = 0;
    init_serial_combo();
    try {
        m_serial->close();
        throw exception; // Throw an exception when a problem arise
    } catch (int ex) {
    }
}

void MainWindow::on_toolButton_clicked(bool checked)
{
    emit alter_plotMode();
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
}
void MainWindow::newFile()
{

qDebug()<<"newFile clicked"<<endl;
}



void MainWindow::open()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Directory"),
                                                    "./debug",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    qDebug()<<"openFile clicked"<<dir<<endl;
}

void MainWindow::savePlotData(){

for(map<QString,DataPlot*>::const_iterator iter = plots_ptr.begin();iter!=plots_ptr.end();iter++){
DataPlot* plt  = iter->second;
if(plt->isSelected()){
    currentFilename = QFileDialog::getSaveFileName(this,
                                                      tr("Save"), "",
                                                      tr(".csv"));
    if (currentFilename.isEmpty())
          return;
    else {
        qDebug()<<"this is his"<<currentFilename<<endl;
          QFile file(currentFilename);
         if (!file.open(QFile::WriteOnly|QFile::Truncate)) {
              QMessageBox::information(this, tr("Unable to open file"),
                  file.errorString());
              return;
         }
         QTextStream output(&file);
         char* columnNames ;
         if(plt->getMode()==Realtime){
             for(int j =0;j<plt->graphCount();j++){
              for(int i=0;i<plt->graph(j)->dataCount();i++){
//                  QCPGraphDataContainer*contain_ptr = plt->graph(0)->data().get();
//                  QCPGraphDataContainer::const_iterator iterator = contain_ptr->at(contain_ptr->size()-1);
//                  qDebug()<<"Time:"<<iterator->sortKey()<<endl;
                  //later handle this //bitch
              }
             }

         }


    }
}



}

}

void MainWindow::save()
{


    qDebug()<<"save clicked"<<endl;
    currentFilename = "";


}
void MainWindow::saveAll()
{
    qDebug()<<"saveAll clicked"<<endl;
}

void MainWindow::print()
{
    qDebug()<<"print clicked"<<endl;
}

void MainWindow::undo()
{
    qDebug()<<"undo clicked"<<endl;
}

void MainWindow::redo()
{
    qDebug()<<"redo clicked"<<endl;
}

//void MainWindow::cut()
//{
//    qDebug()<<"cut clicked"<<endl;
//}

//void MainWindow::copy()
//{
//    qDebug()<<"copy clicked"<<endl;
//}

//void MainWindow::paste()
//{
//    qDebug()<<"paste clicked"<<endl;
//}

void MainWindow::bold()
{
    qDebug()<<"bold clicked"<<endl;
}

void MainWindow::italic()
{
    qDebug()<<"italic clicked"<<endl;
}

void MainWindow::leftAlign()
{
    qDebug()<<"leftAlign clicked"<<endl;
}

void MainWindow::rightAlign()
{
    qDebug()<<"rightAlign clicked"<<endl;
}

void MainWindow::justify()
{
    qDebug()<<"justify clicked"<<endl;
}

void MainWindow::center()
{
    qDebug()<<"center clicked"<<endl;
}

void MainWindow::setLineSpacing()
{
    qDebug()<<"setLineSpacing clicked"<<endl;
}

void MainWindow::setParagraphSpacing()
{
    qDebug()<<"setParagraphSpacing clicked"<<endl;
}

void MainWindow::about()
{
    qDebug()<<"about clicked"<<endl;

    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}

void MainWindow::aboutQt()
{
    qDebug()<<"aboutQt clicked"<<endl;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
//    fileMenu->addAction(saveAllAct);
    editMenu = menuBar()->addMenu(tr("&Edit"));
    helpMenu = menuBar()->addMenu(tr("&Help"));


    formatMenu = editMenu->addMenu(tr("&Format"));
//    formatMenu->addAction(boldAct);
//    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
//    formatMenu->addAction(leftAlignAct);
//    formatMenu->addAction(rightAlignAct);
//    formatMenu->addAction(justifyAct);
//    formatMenu->addAction(centerAct);
//    formatMenu->addSeparator();
//    formatMenu->addAction(setLineSpacingAct);
//    formatMenu->addAction(setParagraphSpacingAct);

}
void MainWindow::on_horExpand_button_clicked(bool checked)
{
    emit expand_plot_axises(Axis_x, EXPAND);
}

void MainWindow::on_horConstrict_button_clicked(bool checked)
{
    emit expand_plot_axises(Axis_x, CONSTRICT);
}

void MainWindow::on_verExpand_button_clicked(bool checked)
{
    emit expand_plot_axises(Axis_y, EXPAND);
}

void MainWindow::on_vertialConstrict_button_clicked(bool checked)
{
    emit expand_plot_axises(Axis_y, CONSTRICT);
}
