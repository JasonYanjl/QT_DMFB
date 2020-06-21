#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startdialog.h"
#include "cleaningdialog.h"
#include "setcleandialog.h"
#include <windows.h>
#include <QVector>
#include<vector>
#include<QtDebug>
#include<QFileDialog>
#include<QMessageBox>
#include<iostream>
#include<QPainter>
#include<QColor>
#include<algorithm>
#include<QMouseEvent>
#include<QSound>
#include<QTime>

#define MATRIX_WH 30
#define MAXTIME 120
#define MAXWH 15
#define NOTPLAYSOUND 1e9

namespace Ui {
class MainWindow;
}

struct Option{
    int type,time;
    std::vector<int> v;
};

struct Record{
    int bubble[MAXWH][MAXWH];
    int totbubble;
    bool nowclick[MAXWH][MAXWH];
    std::vector<int> dirty[MAXWH][MAXWH];
    Record(){
        memset(bubble,0,sizeof(bubble));
        totbubble=0;
        memset(nowclick,0,sizeof(nowclick));
        for(int i=0;i<MAXWH;i++)
            for(int j=0;j<MAXWH;j++) dirty[i][j].clear();
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<Option> opt;
    std::pair<int,int> Place[MAXWH][MAXWH];
    bool nowpainted[MAXWH][MAXWH];
    QVector<QColor> Colors;
    Record Save[MAXTIME];
    QSound *soundMove,*soundSplit1,*soundSplit2,*soundMerge;

    void GetNextRecord(Record &tmprecord,int tmptime);
    void DrawDMFB();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_actionStart_triggered();
    void buildDMFB(int,int);
    void InLocationSet(int,int);
    void OutLocationSet(int,int);
    void buildCleaning(int,int,int,int,bool);
    void AddCleanPlace(int,int);
    void SetAutoClean(bool);

    void on_actionLoad_File_triggered();

    void on_actionReset_triggered();

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

    void on_actionPlay_all_triggered();

    void mousePressEvent(QMouseEvent *);

    void on_radioButtonCleaning_clicked(bool checked);

    void on_pushButtonStartClean_clicked();

    void on_pushButtonSetClean_clicked();

private:
    Ui::MainWindow *ui;
    StartDialog *startdialog;
    CleaningDialog *cleaningdialog;
    setcleanDialog *setcleandialog;
    QVector<std::pair<int,int> > InLocation;
    int Outx,Outy;
    int Row,Column;
    int Nowtime=0,Endtime=0;
    int CleanInx,CleanIny,CleanOutx,CleanOuty;
    bool isClean,CleanLimit;
    bool AutoClean;
    bool nowPlaying;
    int PlaySoundOrNot=0;
    QVector<std::pair<int,int> > Washxy;

    void CheckPollution();
    bool Check_Input(int x,int y);
    bool Check_Output(int x,int y);
    bool Check_Move(const Record &tmprecord,int tmptime);
    void ALLCLEAR();
    bool actionNext();
    void Calc();
    void ExtendClean();
    void ExtendRoute(int mxtime);
    int GetNeedAddTime(int LastTime);
    void Cleandfs(int sx,int sy);
    int GetRouteBfs(int wzx,int wzy);
    void AddClear();
    void StartClear();
    void Unique_Record(Record &tmprecord);
};

#endif // MAINWINDOW_H
