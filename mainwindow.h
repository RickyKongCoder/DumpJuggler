#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dataplot.h"
#include "plotingpanel.h"
#include "qiodevice.h"
#include "serialhandler.h"
#include "ui_mainwindow.h"
#include "uithread.h"
#include "vartable.h"
#include <map>
#include <vector>
#include <QMainWindow>
#include <QSerialPort>
#include <QTreeWidgetItem>
#include <qcustomplot.h>
class QMenu;

class QAction;
class QActionGroup;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define Default_Plot_Color QColor(144, 0, 0, 0)
class VPlotingPanel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
   QThread workerThread;

    void init_serial_combo();
 //   void add_plot(QString title_name,QVBoxLayout* layout,QColor pencolor);
   // DataPlot * set_dataPlot(DataPlot *newplot,QVBoxLayout*layout, QString title_name, const QCP::Interactions &interactions,QColor pencolor) ;
//    void remove_plot(QString);
    void init_serial();
    ZoomMode mode = AUTO;
    void alter_plotwidth(DataPlot*dataplot,Axis ax);
    void createMenus();
    VarTable*varTable1;
    QHBoxLayout * dataHorLayout;
    VPlotingPanel*vdataplot;

    QString currentFilename;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
        QAction *newAct;
        QAction *openAct;
        QAction *saveAct;
        QAction *saveAllAct;//

        QAction* plotMode;
        QAction *printAct;

        QAction *exitAct;
        QAction *undoAct;
        QAction *redoAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;
        QAction *boldAct;
        QAction *italicAct;
        QAction *leftAlignAct;
        QAction *rightAlignAct;
        QAction *justifyAct;
        QAction *centerAct;
        QAction *setLineSpacingAct;
        QAction *setParagraphSpacingAct;
        QAction *aboutAct;
        QAction *aboutQtAct;
        QLabel *infoLabel;
    void createActions();
    void savePlotData();


public:
    MainWindow(QWidget *parent = nullptr);
    UIThread *uithread1;
    ~MainWindow();
  map<QString,DataPlot*> plots_ptr;
  vector<QTableWidgetItem*> checkboxInTable;
  vector<QString> selected_plot_nm;
  float x_width = 40;
  void add_plot();
  vector<DataPlot*> get_plots();
  void updateDataPlots(DataPlot*,QCPRange,QCPRange);


signals:
  void expand_plot_axises(Axis ,AxisChange );
  void init_serial_signal(QString, int, QString);
  void alter_plotMode();
  void alter_plotMode(ZoomMode);

  //  void addPlot(QString title_name, QVBoxLayout *layout, QColor pencolor);
  //  void removePlot(QString name);

  private slots:
  void newFile();
      void open();
      void save();
      void saveAll();
      void print();
      void undo();
      void redo();
      void bold();
      void italic();
      void leftAlign();
      void rightAlign();
      void justify();
      void center();
      void setLineSpacing();
      void setParagraphSpacing();
      void about();
      void aboutQt();
void on_pushButton_2_clicked();
void customplot_clicked(QString,QMouseEvent*);
void on_pushButton_clicked();
void on_toolButton_clicked(bool checked);
//void alter_plot_axises(Axis,AxisChange);

void on_horExpand_button_clicked(bool checked);
void on_horConstrict_button_clicked(bool checked);
void on_verExpand_button_clicked(bool checked);
void on_vertialConstrict_button_clicked(bool checked);
//temporary comment out, use later void varWidgetContextMenu(QMouseEvent*event);
public slots:
void alterShowMode(ZoomMode);
};
#endif // MAINWINDOW_H
