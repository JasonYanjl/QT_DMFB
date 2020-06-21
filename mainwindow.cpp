#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=0;i<=200;i++)
        Colors.push_back(QColor(rand() % 256,rand() % 256,rand() % 256));

    connect(ui->pushButtonNext,SIGNAL(clicked()),this,SLOT(on_actionNext_triggered()));
    connect(ui->pushButtonPre,SIGNAL(clicked()),this,SLOT(on_actionPrevious_triggered()));
    connect(ui->pushButtonPlay,SIGNAL(clicked()),this,SLOT(on_actionPlay_all_triggered()));
    connect(ui->pushButtonReset,SIGNAL(clicked()),this,SLOT(on_actionReset_triggered()));

    soundMove=new QSound(":/new/bgm/bgm/jiu.wav");
    soundSplit1=new QSound(":/new/bgm/bgm/tan.wav");
    soundSplit2=new QSound(":/new/bgm/bgm/beng.wav");
    soundMerge=new QSound(":/new/bgm/bgm/bling.wav");

    ui->lcdNumber->display(0);

    Washxy.clear();

    ALLCLEAR();
    Save[0]=Record();

    QFont font("Microsoft YaHei");
    ui->menuBar->setFont(font);
    ui->menuLet_s_start->setFont(font);
    ui->menuopation->setFont(font);
    ui->mainToolBar->setFont(font);
    ui->pushButtonPre->setFont(font);
    ui->pushButtonNext->setFont(font);
    ui->pushButtonPlay->setFont(font);
    ui->pushButtonReset->setFont(font);
    ui->pushButtonSetClean->setFont(font);
    ui->pushButtonStartClean->setFont(font);
    ui->radioButtonCleaning->setFont(font);
    ui->lcdNumber->setFont(font);


    ui->pushButtonNext->setIcon(QIcon(QPixmap(":/new/photo/photo/Next.png")));
    ui->pushButtonPre->setIcon(QIcon(QPixmap(":/new/photo/photo/Previous.png")));
    ui->pushButtonPlay->setIcon(QIcon(QPixmap(":/new/photo/photo/Playall.png")));
    ui->pushButtonReset->setIcon(QIcon(QPixmap(":/new/photo/photo/Reset.png")));

    this->setWindowTitle("DMFB");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetNextRecord(Record &tmprecord, int tmptime)
{
    tmprecord=Save[tmptime];
    for(int i=0;i<opt.size();i++) {
        if (opt[i].type==1) {
            if (opt[i].time!=tmptime) continue;
            if (!Check_Input(opt[i].v[0],opt[i].v[1])) {
                QMessageBox::critical(NULL, "Warning", tr("time %1 input illegal").arg(QString::number(tmptime)),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
            else {
                tmprecord.totbubble++;
                tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]=tmprecord.totbubble;
                tmprecord.dirty[opt[i].v[0]][opt[i].v[1]].push_back(tmprecord.totbubble);
            }
        }
        else if (opt[i].type==2) {
            if (opt[i].time!=tmptime) continue;
            if (tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]==0) {
                QMessageBox::critical(NULL, "Warning", tr("time %1 cannot move").arg(QString::number(tmptime)),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
            else {
                int x1=opt[i].v[0];
                int y1=opt[i].v[1];
                int x2=opt[i].v[2];
                int y2=opt[i].v[3];
                tmprecord.bubble[opt[i].v[2]][opt[i].v[3]]=tmprecord.bubble[opt[i].v[0]][opt[i].v[1]];
                tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]=0;
                tmprecord.dirty[opt[i].v[2]][opt[i].v[3]].push_back
                        (tmprecord.bubble[opt[i].v[2]][opt[i].v[3]]);

                if (PlaySoundOrNot!=NOTPLAYSOUND) soundMove->play();
            }
        }
        else if (opt[i].type==3) {
            if (opt[i].time==tmptime) {
                if (tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]==0) {
                    QMessageBox::critical(NULL, "Warning", tr("time %1 cannot split").arg(QString::number(tmptime)),
                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                }
                else {
                    tmprecord.totbubble++;
                    tmprecord.bubble[opt[i].v[2]][opt[i].v[3]]=tmprecord.totbubble;
                    tmprecord.dirty[opt[i].v[2]][opt[i].v[3]].push_back
                            (tmprecord.bubble[opt[i].v[2]][opt[i].v[3]]);
                    tmprecord.totbubble++;
                    tmprecord.bubble[opt[i].v[4]][opt[i].v[5]]=tmprecord.totbubble;
                    tmprecord.dirty[opt[i].v[4]][opt[i].v[5]].push_back
                            (tmprecord.bubble[opt[i].v[4]][opt[i].v[5]]);
                    if (PlaySoundOrNot!=NOTPLAYSOUND) soundSplit1->play();
                }
            }
            else if (opt[i].time+1==tmptime) {
                    tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]=0;
                    if (PlaySoundOrNot!=NOTPLAYSOUND) soundSplit2->play();
            }
            else {
                continue;
            }
        }
        else if (opt[i].type==4) {
            int nown=opt[i].v.size() / 2;
            if (tmptime>=opt[i].time && tmptime<=opt[i].time+(nown-2)) {
                int x1=opt[i].v[2*(tmptime-opt[i].time)];
                int y1=opt[i].v[2*(tmptime-opt[i].time)+1];
                int x2=opt[i].v[2*(tmptime-opt[i].time)+2];
                int y2=opt[i].v[2*(tmptime-opt[i].time)+3];
                /*if (tmptime==opt[i].time) {
                    tmprecord.totbubble++;
                    tmprecord.bubble[x2][y2]=tmprecord.totbubble;
                    tmprecord.dirty[x2][y2].push_back(tmprecord.totbubble);
                    tmprecord.bubble[x1][y1]=0;
                }
                else if (tmptime==opt[i].time+(nown-2)){
                    tmprecord.totbubble++;
                    tmprecord.bubble[x2][y2]=tmprecord.totbubble;
                    tmprecord.dirty[x2][y2].push_back(tmprecord.totbubble);
                    tmprecord.bubble[x1][y1]=0;
                }*/

                tmprecord.bubble[x2][y2]=tmprecord.bubble[x1][y1];
                tmprecord.dirty[x2][y2].push_back(tmprecord.bubble[x2][y2]);
                tmprecord.bubble[x1][y1]=0;

                if (PlaySoundOrNot!=NOTPLAYSOUND) soundMove->play();
            }
            else continue;
        }
        else if (opt[i].type==5) {
            if (opt[i].time==tmptime) {
                tmprecord.totbubble++;
                tmprecord.bubble[(opt[i].v[0]+opt[i].v[2])/2][(opt[i].v[1]+opt[i].v[3])/2]=tmprecord.totbubble;
                tmprecord.dirty[(opt[i].v[0]+opt[i].v[2])/2][(opt[i].v[1]+opt[i].v[3])/2].
                        push_back(tmprecord.totbubble);
            }
            else if (opt[i].time+1==tmptime) {
                tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]=0;
                tmprecord.bubble[opt[i].v[2]][opt[i].v[3]]=0;

                if (PlaySoundOrNot!=NOTPLAYSOUND) soundMerge->play();
            }
            else {
                continue;
            }
        }
        else if (opt[i].type==6) {
            if (opt[i].time!=tmptime) continue;
            if (!Check_Output(opt[i].v[0],opt[i].v[1])) {
                QMessageBox::critical(NULL, "Warning", tr("time %1 output illegal").arg(QString::number(tmptime)),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
            else {
                tmprecord.bubble[opt[i].v[0]][opt[i].v[1]]=0;
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //lcdnumber
    ui->lcdNumber->display(Nowtime);

    //draw matrix
    painter.setBrush(QColor(0,0,0,0));
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++)
                painter.drawRect(Place[i][j].first,Place[i][j].second,MATRIX_WH,MATRIX_WH);

    //draw input
    for(int i=0;i<InLocation.size();i++) {
        if (InLocation[i]==std::make_pair(0,0)) continue;
        int x=InLocation[i].first;
        int y=InLocation[i].second;
        QBrush brush(QColor(255,192,203));
        painter.setBrush(brush);
        if (x==1) {
            QRectF rect(Place[x][y].first-MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "In");
        }
        else if (y==1) {
            QRectF rect(Place[x][y].first,Place[x][y].second-MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "In");
        }
        else if (x==Row) {
            QRectF rect(Place[x][y].first+MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "In");
        }
        else if (y==Column){
            QRectF rect(Place[x][y].first,Place[x][y].second+MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "In");
        }
    }

    //draw output
    if (Row!=0 && Column!=0){
        QBrush brush(QColor(0,0,205));
        painter.setBrush(brush);
        int x=Outx;
        int y=Outy;
        if (x==1) {
            QRectF rect(Place[x][y].first-MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Out");
        }
        else if (y==1) {
            QRectF rect(Place[x][y].first,Place[x][y].second-MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Out");
        }
        else if (x==Row) {
            QRectF rect(Place[x][y].first+MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Out");
        }
        else if (y==Column){
            QRectF rect(Place[x][y].first,Place[x][y].second+MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Out");
        }
    }

    memset(nowpainted,0,sizeof(nowpainted));

    //draw oval
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (nowpainted[i][j]) continue;
            if (i+2<=Row && Save[Nowtime].bubble[i][j] && Save[Nowtime].bubble[i+1][j]
                    && Save[Nowtime].bubble[i+2][j])
            {
                nowpainted[i][j]=nowpainted[i+1][j]=nowpainted[i+2][j]=1;
                QBrush brush(Colors[Save[Nowtime].bubble[i+1][j]]);
                painter.setBrush(brush);
                painter.drawEllipse(Place[i][j].first+5,Place[i][j].second+5,80,20);
            }
            else if (j+2<=Column && Save[Nowtime].bubble[i][j] && Save[Nowtime].bubble[i][j+1]
                     && Save[Nowtime].bubble[i][j+2])
            {
                nowpainted[i][j]=nowpainted[i][j+1]=nowpainted[i][j+2]=1;
                QBrush brush(Colors[Save[Nowtime].bubble[i][j+1]]);
                painter.setBrush(brush);
                painter.drawEllipse(Place[i][j].first+5,Place[i][j].second+5,20,80);
            }
        }

    //draw circle
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (Save[Nowtime].bubble[i][j]==0) continue;
            if (nowpainted[i][j]) continue;
            nowpainted[i][j]=1;
            QBrush brush(Colors[Save[Nowtime].bubble[i][j]]);
            painter.setBrush(brush);
            painter.drawEllipse(Place[i][j].first+5,Place[i][j].second+5,20,20);
        }

    //draw small circle
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (nowpainted[i][j]) continue;
            for(int k=0;k<Save[Nowtime].dirty[i][j].size();k++) {
                if (k>=16) continue;
                //QPen pen(Colors[Save[Nowtime].dirty[i][j][k]]);
                //painter.setPen(pen);
                QBrush brush(Colors[Save[Nowtime].dirty[i][j][k]]);
                painter.setBrush(brush);
                painter.drawEllipse(Place[i][j].first+5+(k / 4)*5,Place[i][j].second+5+(k % 4)*5,5,5);
                //QPen pen1(Qt::black);
                //painter.setPen(pen1);
            }
        }

    //draw visit Text
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            painter.drawText(Place[i][j].first+25,Place[i][j].second+25,
                             QString::number(Save[Nowtime].dirty[i][j].size()));
        }

    //draw nowclick
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (Save[Nowtime].nowclick[i][j]) {
                QPen pen(Qt::yellow);
                painter.setPen(pen);
                painter.drawLine(Place[i][j].first+1,Place[i][j].second+1,
                                 Place[i][j].first+10,Place[i][j].second+1);
                painter.drawLine(Place[i][j].first+1,Place[i][j].second+1,
                                 Place[i][j].first+1,Place[i][j].second+10);
                //
                painter.drawLine(Place[i][j].first+2,Place[i][j].second+2,
                                 Place[i][j].first+10,Place[i][j].second+2);
                painter.drawLine(Place[i][j].first+2,Place[i][j].second+2,
                                 Place[i][j].first+2,Place[i][j].second+10);
                //
                painter.drawLine(Place[i][j].first+19,Place[i][j].second+29,
                                 Place[i][j].first+29,Place[i][j].second+29);
                painter.drawLine(Place[i][j].first+29,Place[i][j].second+19,
                                 Place[i][j].first+29,Place[i][j].second+29);
                //
                painter.drawLine(Place[i][j].first+19,Place[i][j].second+28,
                                 Place[i][j].first+28,Place[i][j].second+28);
                painter.drawLine(Place[i][j].first+28,Place[i][j].second+19,
                                 Place[i][j].first+28,Place[i][j].second+28);
                //
                QPen pen1(Qt::black);
                painter.setPen(pen1);
            }
        }

    //draw Wash
    if (isClean) {
        QBrush brush(QColor(87,250,255));
        painter.setBrush(brush);
        int x=CleanInx;
        int y=CleanIny;
        if (x==1) {
            QRectF rect(Place[x][y].first-MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "WIn");
        }
        else if (y==1) {
            QRectF rect(Place[x][y].first,Place[x][y].second-MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "WIn");
        }
        else if (x==Row) {
            QRectF rect(Place[x][y].first+MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "WIn");
        }
        else if (y==Column){
            QRectF rect(Place[x][y].first,Place[x][y].second+MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "WIn");
        }
        //OutWash
        QBrush brush1(QColor(169,169,169));
        painter.setBrush(brush1);
        x=CleanOutx;
        y=CleanOuty;
        if (x==1) {
            QRectF rect(Place[x][y].first-MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Wst");
        }
        else if (y==1) {
            QRectF rect(Place[x][y].first,Place[x][y].second-MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Wst");
        }
        else if (x==Row) {
            QRectF rect(Place[x][y].first+MATRIX_WH,Place[x][y].second,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Wst");
        }
        else if (y==Column){
            QRectF rect(Place[x][y].first,Place[x][y].second+MATRIX_WH,MATRIX_WH,MATRIX_WH);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, "Wst");
        }

        // draw Wash bubble
        for(int i=0;i<Washxy.size();i++) {
            int x=Washxy[i].first;
            int y=Washxy[i].second;
            QBrush brush(QColor(127,255,212));
            painter.setBrush(brush);
            painter.drawEllipse(Place[x][y].first+5,Place[x][y].second+5,20,20);
        }
    }

}

void MainWindow::on_actionStart_triggered()
{
    startdialog = new StartDialog(this);
    //startdialog->setFixedHeight(100);
    //startdialog->setFixedWidth(100);
    startdialog->show();
    connect(startdialog,SIGNAL(buildDMFB(int,int)),
           this,SLOT(buildDMFB(int,int)));
    connect(startdialog,SIGNAL(InLocationSet(int,int)),this,SLOT(InLocationSet(int,int)));
    connect(startdialog,SIGNAL(OutLocationSet(int,int)),this,SLOT(OutLocationSet(int,int)));
    connect(startdialog,SIGNAL(AutoCleanSet(bool)),this,SLOT(on_radioButtonCleaning_clicked(bool)));
    connect(startdialog,SIGNAL(AutoCleanSet(bool)),this,SLOT(SetAutoClean(bool)));
}

void MainWindow::buildDMFB(int x, int y)
{
    qDebug()<<"MainWindow::buildDMFB";
    qDebug()<<x<<y;
    ALLCLEAR();
    Row=x;
    Column=y;
    for(int i=1;i<=x;i++){
        for(int j=1;j<=y;j++) {
            Place[i][j]=std::make_pair(270-MATRIX_WH*((x+1) /2 -i),270-MATRIX_WH*((y+1) / 2-j));
        }
    }
    update();
}

void MainWindow::InLocationSet(int x, int y)
{
    qDebug()<<"InLocation";
    qDebug()<<x<<y;
    bool bz=1;
    for(int i=0;i<InLocation.size();i++)
        if (x==InLocation[i].first && y==InLocation[i].second) {
            bz=0;
            break;
        }
    if (bz) {
        InLocation.push_back(std::make_pair(x,y));
    }
    update();
}

void MainWindow::OutLocationSet(int x, int y)
{
    qDebug()<<"OutLocation";
    qDebug()<<x<<y;
    Outx=x;
    Outy=y;
    update();
}

void MainWindow::buildCleaning(int x1, int y1, int x2, int y2, bool tmpLimit)
{
    qDebug()<<"buildCleaning"<<x1<<y1<<x2<<y2<<tmpLimit;
    if (x1==0 || y1==0 || x2==0 || y2==0) {
        QMessageBox::critical(NULL, "Warning", "Input is illegal",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if (x1<=0 || x1>Row || y1<=0 || y1>Column ||x2<=0 || x2>Row || y2<=0 || y2>Column){
        QMessageBox::critical(NULL, "Warning", "Input is illegal",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if ((x1!=1 && x1!=Row && y1!=1 && y1!=Column) || (x2!=1 && x2!=Row && y2!=1 && y2!=Column)) {
        QMessageBox::critical(NULL, "Warning", "Input is illegal",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if (Check_Input(x1,y1) || Check_Input(x2,y2) || Check_Output(x1,y1) || Check_Output(x2,y2)) {
        QMessageBox::critical(NULL, "Warning", "Input is illegal",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    CleanInx=x1;
    CleanIny=y1;
    CleanOutx=x2;
    CleanOuty=y2;
    isClean=1;
    CleanLimit=tmpLimit;
    ui->radioButtonCleaning->setChecked(true);
    qDebug()<<"buildOK"<<x1<<y1<<x2<<y2<<tmpLimit;
}

void MainWindow::on_actionLoad_File_triggered()
{
    qDebug()<<"Load File";
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Input File"),"",tr("*.txt"));
    qDebug()<<fileName;

    if (!fileName.isNull()){
        QFile file(fileName);
        if(!file.open((QFile::ReadOnly|QFile::Text))){
            QMessageBox::warning(this,tr("Error"),tr("Cannot Read File:&1").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QStringList strlist;
        QVector<QString> tmp;
        tmp.clear();
        opt.clear();
        int cntline= 0;
        while(!in.atEnd())
        {
            tmp.push_back(in.readLine());
            qDebug() << tmp[cntline];
            cntline++;
        }
        for(int i=0;i<cntline;i++) {
            std::string nows = tmp[i].toStdString();
            Option tmpoption;
            tmpoption.type=0;
            tmpoption.time=-1;
            tmpoption.v.clear();
            int nowlen=nows.length();
            std::string tmps1="";
            int fir=0;
            for(int j=0;j<nowlen;j++) {
                if (nows[j]==' ') {
                    fir=j+1;
                    break;
                }
                else {
                    tmps1=tmps1+nows[j];
                }
            }
            if (tmps1=="Input") {
                tmpoption.type=1;
            }
            else if (tmps1=="Move") {
                tmpoption.type=2;
            }
            else if (tmps1=="Split") {
                tmpoption.type=3;
            }
            else if (tmps1=="Mix") {
                tmpoption.type=4;
            }
            else if (tmps1=="Merge") {
                tmpoption.type=5;
            }
            else if (tmps1=="Output") {
                tmpoption.type=6;
            }
            else continue;
            int tmpx=0;
            for(int j=fir;j<nowlen;j++) {
                if (nows[j]>='0' && nows[j]<='9') {
                    tmpx=tmpx * 10+(nows[j]-'0');
                }
                else {
                    if (tmpoption.time==-1) tmpoption.time=tmpx;
                    else tmpoption.v.push_back(tmpx);
                    //std::cout<<tmpx<<std::endl;
                    tmpx=0;
                }
            }
            opt.push_back(tmpoption);
            if (tmpoption.type==1) Endtime=std::max(Endtime,tmpoption.time+1);
            else if (tmpoption.type==2) Endtime=std::max(Endtime,tmpoption.time+1);
            else if (tmpoption.type==3) Endtime=std::max(Endtime,tmpoption.time+2);
            else if (tmpoption.type==4) Endtime=std::max(Endtime,tmpoption.time+(int)tmpoption.v.size() / 2);
            else if (tmpoption.type==5) Endtime=std::max(Endtime,tmpoption.time+2);
            else if (tmpoption.type==6) Endtime=std::max(Endtime,tmpoption.time+1);
        }
        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::on_actionReset_triggered()
{
    Nowtime=0;
    update();
}

void MainWindow::on_actionPrevious_triggered()
{
    if (Nowtime>0) {
        Nowtime--;
        update();
    }
    else {
        QMessageBox::critical(NULL, "Warning", "Now at the time 0",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void MainWindow::CheckPollution()
{
    bool nopollution=1;
    for(int i=1;i<=Row;i++) {
        if (nopollution==0) break;
        for(int j=1;j<=Column;j++) {
            if (Save[Nowtime].dirty[i][j].size()>=2 && Save[Nowtime].bubble[i][j]>0) {
                nopollution=0;
                break;
            }
        }
    }
    if (nopollution==0) {
        QMessageBox::critical(NULL, "Warning", "Pollution cannot be cleaned",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void MainWindow::on_actionNext_triggered()
{
    qDebug()<<"Time"<<Nowtime;
    if (Nowtime>=Endtime) {
        QMessageBox::critical(NULL, "Warning", "Now at the end time",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else {
        Record tmprecord,tmprecord1;
        GetNextRecord(tmprecord,Nowtime);
        //GetNextRecord(tmprecord1,Nowtime,0);
        if (!Check_Move(tmprecord,Nowtime)) {
            QMessageBox::critical(NULL, "Warning", tr("time %1 cannot move").arg(QString::number(Nowtime)),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else {
            Nowtime++;
            Unique_Record(tmprecord);
            Save[Nowtime]=tmprecord;
            if (AutoClean && isClean) CheckPollution();
            repaint();
            if (AutoClean && isClean) AddClear();
        }
    }
}

bool MainWindow::actionNext()
{
    qDebug()<<"Time"<<Nowtime;
    if (Nowtime>=Endtime) {
        QMessageBox::critical(NULL, "Warning", "Now at the end time",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return 0;
    }
    else {
        Record tmprecord,tmprecord1;
        GetNextRecord(tmprecord,Nowtime);
        //GetNextRecord(tmprecord1,Nowtime,0);
        if (!Check_Move(tmprecord,Nowtime)) {
            QMessageBox::critical(NULL, "Warning", tr("time %1 cannot move").arg(QString::number(Nowtime)),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return 0;
        }
        else {
            Nowtime++;
            Unique_Record(tmprecord);
            Save[Nowtime]=tmprecord;
            if (AutoClean && isClean) CheckPollution();
            return 1;
        }
    }
}

void MainWindow::on_actionPlay_all_triggered()
{
    nowPlaying=1;
    PlaySoundOrNot=0;
    while (1) {
        //for(int i=1;i<=3e7;i++);
        QTime qtime;
        qtime.start();
        while(qtime.elapsed()<300)
            QCoreApplication::processEvents();
        //Sleep(100);
        if (actionNext()==0) break;
        repaint();
        if (AutoClean && isClean) AddClear();
    }
    PlaySoundOrNot=0;
    nowPlaying=0;
}

void MainWindow::mousePressEvent(QMouseEvent *nowevent)
{
    if (nowPlaying) {
        nowevent->ignore();
        return;
    }
    if (nowevent->button()==Qt::LeftButton){
        int x=nowevent->x();
        int y=nowevent->y();
        bool finded=0;
        for(int i=1;i<=Row;i++) {
            if (finded) break;
            for(int j=1;j<=Column;j++) {
                if (Place[i][j].first<=x && x<=Place[i][j].first+30 &&
                        Place[i][j].second<=y && y<=Place[i][j].second+30) {
                    Save[Nowtime].nowclick[i][j]^=1;
                    finded=1;
                    break;
                }
            }
        }
        repaint();
    }
}

bool MainWindow::Check_Input(int x, int y)
{
    for(int i=0;i<InLocation.size();i++)
        if (x==InLocation[i].first && y==InLocation[i].second) return 1;
    for(int i=0;i<InLocation.size();i++)
        qDebug()<<InLocation[i].first<<InLocation[i].second;
    qDebug()<<x<<y;
    qDebug()<<opt.size();
    return 0;
}

bool MainWindow::Check_Output(int x, int y)
{
    return (x==Outx && y==Outy);
}

bool MainWindow::Check_Move(const Record &tmprecord, int tmptime)
{
    for(int i=0;i<opt.size();i++) {
        if (opt[i].type==1) {
            if (opt[i].time!=tmptime) continue;
            int x1=opt[i].v[0];
            int y1=opt[i].v[1];
            for(int k1=-1;k1<=1;k1++)
                for(int k2=-1;k2<=1;k2++) {
                    int x2=x1+k1;
                    int y2=y1+k2;
                    if (x2<1 || y2<1 || x2>Row || y2>Column) continue;
                    if (tmprecord.bubble[x2][y2]!=0 && tmprecord.bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                        return 0;
                    if (Save[tmptime].bubble[x2][y2]!=0 && Save[tmptime].bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                        return 0;
                }
        }
        else if (opt[i].type==2) {
            if (opt[i].time!=tmptime) continue;
            int x1=opt[i].v[2];
            int y1=opt[i].v[3];
            for(int k1=-1;k1<=1;k1++)
                for(int k2=-1;k2<=1;k2++) {
                    int x2=x1+k1;
                    int y2=y1+k2;
                    if (x2<1 || y2<1 || x2>Row || y2>Column) continue;
                    if (tmprecord.bubble[x2][y2]!=0 && tmprecord.bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                        return 0;
                    if (Save[tmptime].bubble[x2][y2]!=0 && Save[tmptime].bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                        return 0;
                }
        }
        else if (opt[i].type==4) {
            int nown=opt[i].v.size() / 2;
            if (tmptime>=opt[i].time && tmptime<=opt[i].time+(nown-2)) {
                int x1=opt[i].v[2*(tmptime-opt[i].time)+2];
                int y1=opt[i].v[2*(tmptime-opt[i].time)+3];
                for(int k1=-1;k1<=1;k1++)
                    for(int k2=-1;k2<=1;k2++) {
                        int x2=x1+k1;
                        int y2=y1+k2;
                        if (x2<1 || y2<1 || x2>Row || y2>Column) continue;
                        if (tmprecord.bubble[x2][y2]!=0 && tmprecord.bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                            return 0;
                        if (Save[tmptime].bubble[x2][y2]!=0 && Save[tmptime].bubble[x2][y2]!=tmprecord.bubble[x1][y1])
                            return 0;
                    }
            }
            else continue;
        }
    }
    return 1;
}

void MainWindow::ALLCLEAR()
{
    InLocation.clear();
    CleanInx=CleanIny=CleanOutx=CleanOuty=isClean=CleanLimit=0;
    AutoClean=0;
    ui->radioButtonCleaning->setChecked(false);
    Row=Column=0;
    PlaySoundOrNot=0;
    nowPlaying=0;
    Outx=Outy=0;
    Washxy.clear();
    Nowtime=Endtime=0;
    Save[0]=Record();
    memset(Place,0,sizeof(Place));
    opt.clear();
}

void MainWindow::Unique_Record(Record &tmprecord)
{
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            sort(tmprecord.dirty[i][j].begin(),tmprecord.dirty[i][j].begin());
            tmprecord.dirty[i][j].erase(unique(tmprecord.dirty[i][j].begin(),
                                               tmprecord.dirty[i][j].end()),
                                        tmprecord.dirty[i][j].end());
        }
}

QVector<std::pair<std::pair<int,int>,std::pair<int,int> > > WashRoute;

int nowstatus[MAXWH][MAXWH];

int dist[MAXWH][MAXWH][MAXWH][MAXWH];

int dirtydist[MAXWH][MAXWH];

const int fx[4][4]={{1,0},{-1,0},{0,1},{0,-1}};

std::pair<int,int> ThisRoute[MAXWH * MAXWH * 2];

std::pair<int,int> Pre[MAXWH][MAXWH];

bool BzTime[MAXWH * MAXWH];

bool NowWalked[MAXWH][MAXWH];

bool dijbz[MAXWH][MAXWH];

void MainWindow::ExtendClean(){
    bool hasextend=1;
    while (hasextend) {
        hasextend=0;
        for(int i=0;i<MAXWH * MAXWH;i++) {
            if (BzTime[i]==0) continue;
            if (ThisRoute[i]==std::make_pair(CleanOutx,CleanOuty)) break;
            if (hasextend) break;
            int nexttime=0;
            for(int j=i+1;j<MAXWH * MAXWH;j++) {
                if (BzTime[j]) {
                    nexttime=j;
                    break;
                }
            }
            if (nexttime==0 || nexttime==i+1) continue;
            for(int x1=1;x1<=Row;x1++) {
                if (hasextend) break;
                for(int x2=1;x2<=Column;x2++) {
                    if (nowstatus[x1][x2]==2 && nexttime-i==
                            dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]+
                            dist[x1][x2][ThisRoute[nexttime].first][ThisRoute[nexttime].second]) {
                        nowstatus[x1][x2]=0;
                        BzTime[i+dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]]=1;
                        ThisRoute[i+dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]]=
                                std::make_pair(x1,x2);
                        hasextend=1;
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::ExtendRoute(int mxtime){
    for(int i=0;i<MAXWH * MAXWH;i++) {
        //if (ThisRoute[i]==std::make_pair(CleanOutx,CleanOuty)) break;
        if (i==mxtime) break;
        int nexttime=0;
        for(int j=i+1;j<MAXWH * MAXWH;j++) {
            if (BzTime[j]) {
                nexttime=j;
                break;
            }
        }
        if (nexttime==0 || nexttime==i+1) continue;
        bool finded=0;
        for(int k=0;k<=3;k++) {
            if (finded) break;
            int x1=ThisRoute[i].first+fx[k][0];
            int x2=ThisRoute[i].second+fx[k][1];
            if (x1<1 || x1>Row || x2<1 || x2>Column) continue;
            if (nowstatus[x1][x2]==1) continue;
            if (ThisRoute[i]==std::make_pair(x1,x2)) continue;
            if (nexttime-i==dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]+
                    dist[x1][x2][ThisRoute[nexttime].first][ThisRoute[nexttime].second]) {
                nowstatus[x1][x2]=0;
                BzTime[i+dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]]=1;
                ThisRoute[i+dist[ThisRoute[i].first][ThisRoute[i].second][x1][x2]]=
                        std::make_pair(x1,x2);
                finded=1;
                break;
            }
        }
    }
}

int abs(int x) {
    return x>0 ? x : -x;
}

int MainWindow::GetNeedAddTime(int LastTime)
{
    for(int ret=1;;ret++) {
        bool checkok=1;
        for(int i=0;i<=LastTime;i++) {
            if (!checkok) break;
            for(int j=0;j<WashRoute.size();j++) {
                if (abs(i+ret-WashRoute[j].first.first)<=1 &&
                        abs(ThisRoute[i].first-WashRoute[j].second.first)<=1 &&
                        abs(ThisRoute[i].second-WashRoute[j].second.second)<=1) {
                    checkok=0;
                    break;
                }
            }
        }
        if (checkok) return ret;
    }
}

void MainWindow::Cleandfs(int sx, int sy)
{
    memset(dijbz,0,sizeof(dijbz));
    while (1) {
        int mnx=0,mny=0,mnt=1e9;
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) {
                if (nowstatus[i][j]==1) continue;
                if (dijbz[i][j]==1) continue;
                if (dirtydist[i][j]<mnt) {
                    mnt=dirtydist[i][j];
                    mnx=i;
                    mny=j;
                }
            }
        //
        if (mnt==1e9) break;
        dijbz[mnx][mny]=1;
        for(int k=0;k<=3;k++) {
            int xx=mnx+fx[k][0],yy=mny+fx[k][1];
            if (xx<1 || xx>Row || yy<1 || yy>Column || nowstatus[xx][yy]==1) continue;
            if (NowWalked[xx][yy] || Save[Nowtime].dirty[xx][yy].size()==0) {
                if (dirtydist[mnx][mny]<dirtydist[xx][yy]) {
                    Pre[xx][yy]=std::make_pair(mnx,mny);
                    dirtydist[xx][yy]=dirtydist[mnx][mny];
                }
            }
            else {
                if (dirtydist[mnx][mny]+1<dirtydist[xx][yy]) {
                    Pre[xx][yy]=std::make_pair(mnx,mny);
                    dirtydist[xx][yy]=dirtydist[mnx][mny]+1;
                }
            }
        }
    }
}

int MainWindow::GetRouteBfs(int wzx, int wzy)
{
    int DistTime=0;
    int nx=wzx,ny=wzy;
    while (nx!=CleanInx || ny!=CleanIny) {
        DistTime++;
        int tmpx=Pre[nx][ny].first;
        int tmpy=Pre[nx][ny].second;
        nx=tmpx;
        ny=tmpy;
    }
    //
    int tmptime=DistTime;
    nx=wzx;
    ny=wzy;
    while (nx!=CleanInx || ny!=CleanIny) {
        ThisRoute[DistTime]=std::make_pair(nx,ny);
        int tmpx=Pre[nx][ny].first;
        int tmpy=Pre[nx][ny].second;
        nx=tmpx;
        ny=tmpy;
        DistTime--;
    }
    ThisRoute[0]=std::make_pair(CleanInx,CleanIny);
    //
    for(int i=tmptime+1;i<=tmptime+dist[wzx][wzy][CleanOutx][CleanOuty];i++) {
        int lastx=ThisRoute[i-1].first;
        int lasty=ThisRoute[i-1].second;
        for(int k=0;k<=3;k++) {
            int xx=lastx+fx[k][0],yy=lasty+fx[k][1];
            if (xx<1 || xx>Row || yy<1 || yy>Column || nowstatus[xx][yy]==1) continue;
            if (dist[lastx][lasty][CleanOutx][CleanOuty]==
                    dist[lastx][lasty][xx][yy]+dist[xx][yy][CleanOutx][CleanOuty]){
                ThisRoute[i]=std::make_pair(xx,yy);
                break;
            }
        }
    }
    return tmptime+dist[wzx][wzy][CleanOutx][CleanOuty];
}

void MainWindow::AddClear()
{
    memset(nowstatus,0,sizeof(nowstatus));
    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (Save[Nowtime].dirty[i][j].size()>0) {
                nowstatus[i][j]=2;
            }
        }
    StartClear();
}

void MainWindow::AddCleanPlace(int x, int y)
{
    qDebug()<<"AddCleanPlace"<<x<<y;
    if (x<1 || x>Row || y<1 || y>Column) {
        QMessageBox::critical(NULL, "Warning", tr("Illegal input"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    nowstatus[x][y]=2;
}

void MainWindow::SetAutoClean(bool checked)
{
    qDebug()<<"SetAutoClean"<<checked;
    AutoClean=checked;
}

void MainWindow::Calc(){
    bool hasclean=0;
    for(int i=1;i<=Row;i++) {
        if (hasclean) break;
        for(int j=1;j<=Column;j++) {
            if (nowstatus[i][j]==2) {
                hasclean=1;
                break;
            }
        }
    }
    if (hasclean==0) {
        if (AutoClean==0)
            QMessageBox::critical(NULL, "Warning", tr("No location to clean"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    //
    if (CleanLimit==0) {
        memset(dist,63,sizeof(dist));
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) dist[i][j][i][j]=0;
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) {
                if (nowstatus[i][j]==1) continue;
                for(int k=0;k<=3;k++) {
                    int xx=i+fx[k][0],yy=j+fx[k][1];
                    if (xx<1 || xx>Row || yy<1 || yy>Column || nowstatus[xx][yy]==1) continue;
                    dist[i][j][xx][yy]=1;
                }
            }

        //floyd
        for(int kx=1;kx<=Row;kx++)
            for(int ky=1;ky<=Column;ky++)
                for(int ix=1;ix<=Row;ix++)
                    for(int iy=1;iy<=Column;iy++)
                        for(int jx=1;jx<=Row;jx++)
                            for(int jy=1;jy<=Column;jy++)
                                dist[ix][iy][jx][jy]=std::min(dist[ix][iy][kx][ky]+dist[kx][ky][jx][jy],
                                                              dist[ix][iy][jx][jy]);
        //
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++)
                if (nowstatus[i][j]==2 && dist[CleanInx][CleanIny][i][j]+dist[i][j][CleanOutx][CleanOuty]>1e6)
                    nowstatus[i][j]=0;
        hasclean=0;
        for(int i=1;i<=Row;i++) {
            if (hasclean) break;
            for(int j=1;j<=Column;j++) {
                if (nowstatus[i][j]==2) {
                    hasclean=1;
                    break;
                }
            }
        }
        if (hasclean==0) {
            if (AutoClean==0)
                QMessageBox::critical(NULL, "Warning", tr("No location to clean"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        //
        while (hasclean) {
            int wzx=0,wzy=0,maxroute=0;
            //
            for(int i=1;i<=Row;i++)
                for(int j=1;j<=Column;j++) {
                    if (nowstatus[i][j]==2) {
                        if (dist[CleanInx][CleanIny][i][j]+dist[i][j][CleanOutx][CleanOuty]>maxroute){
                            maxroute=dist[CleanInx][CleanIny][i][j]+dist[i][j][CleanOutx][CleanOuty];
                            wzx=i;
                            wzy=j;
                        }
                    }
                }
            //
            memset(BzTime,0,sizeof(BzTime));
            memset(ThisRoute,0,sizeof(ThisRoute));
            nowstatus[wzx][wzy]=0;
            BzTime[0]=1;
            ThisRoute[0]=std::make_pair(CleanInx,CleanIny);
            BzTime[dist[CleanInx][CleanIny][wzx][wzy]]=1;
            ThisRoute[dist[CleanInx][CleanIny][wzx][wzy]]=std::make_pair(wzx,wzy);
            BzTime[dist[CleanInx][CleanIny][wzx][wzy]+dist[wzx][wzy][CleanOutx][CleanOuty]]=1;
            ThisRoute[dist[CleanInx][CleanIny][wzx][wzy]+dist[wzx][wzy][CleanOutx][CleanOuty]]=std::make_pair(CleanOutx,CleanOuty);
            ExtendClean();
            ExtendRoute(dist[CleanInx][CleanIny][wzx][wzy]+dist[wzx][wzy][CleanOutx][CleanOuty]);
            //
            int NeedAddTime=GetNeedAddTime(dist[CleanInx][CleanIny][wzx][wzy]+dist[wzx][wzy][CleanOutx][CleanOuty]);
            for(int i=0;i<=dist[CleanInx][CleanIny][wzx][wzy]+dist[wzx][wzy][CleanOutx][CleanOuty];i++) {
                WashRoute.push_back(std::make_pair(std::make_pair(i+NeedAddTime,1),ThisRoute[i]));
            }
            //
            hasclean=0;
            for(int i=1;i<=Row;i++) {
                if (hasclean) break;
                for(int j=1;j<=Column;j++) {
                    if (nowstatus[i][j]==2) {
                        hasclean=1;
                        break;
                    }
                }
            }
        }
    }
    else {
        memset(NowWalked,0,sizeof(NowWalked));
        memset(dist,63,sizeof(dist));
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) dist[i][j][i][j]=0;
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) {
                if (nowstatus[i][j]==1) continue;
                for(int k=0;k<=3;k++) {
                    int xx=i+fx[k][0],yy=j+fx[k][1];
                    if (xx<1 || xx>Row || yy<1 || yy>Column || nowstatus[xx][yy]==1) continue;
                    dist[i][j][xx][yy]=1;
                }
            }

        //floyd
        for(int kx=1;kx<=Row;kx++)
            for(int ky=1;ky<=Column;ky++)
                for(int ix=1;ix<=Row;ix++)
                    for(int iy=1;iy<=Column;iy++)
                        for(int jx=1;jx<=Row;jx++)
                            for(int jy=1;jy<=Column;jy++)
                                dist[ix][iy][jx][jy]=std::min(dist[ix][iy][kx][ky]+dist[kx][ky][jx][jy],
                                                              dist[ix][iy][jx][jy]);
        //
        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++)
                if (nowstatus[i][j]==2 && dist[CleanInx][CleanIny][i][j]+dist[i][j][CleanOutx][CleanOuty]>1e6)
                    nowstatus[i][j]=0;
        hasclean=0;
        for(int i=1;i<=Row;i++) {
            if (hasclean) break;
            for(int j=1;j<=Column;j++) {
                if (nowstatus[i][j]==2) {
                    hasclean=1;
                    break;
                }
            }
        }
        while (hasclean) {
            memset(dirtydist,63,sizeof(dirtydist));
            memset(ThisRoute,0,sizeof(ThisRoute));
            if (NowWalked[CleanInx][CleanIny] ||
                    Save[Nowtime].dirty[CleanInx][CleanIny].size()==0) dirtydist[CleanInx][CleanIny]=0;
            else dirtydist[CleanInx][CleanIny]=1;
            Cleandfs(CleanInx,CleanIny);
            //
            int wzx=0,wzy=0;
            for(int i=1;i<=Row;i++) {
                if (wzx!=0) break;
                for(int j=1;j<=Column;j++) {
                    if (nowstatus[i][j]==2 && dirtydist[i][j]<=3) {
                        wzx=i;
                        wzy=j;
                        break;
                    }
                }
            }
            if (wzx==0 && wzy==0) break;
            //
            int nowlasttime=GetRouteBfs(wzx,wzy);
            int NeedAddTime=GetNeedAddTime(nowlasttime);
            int nowcnt=0;
            for(int i=0;i<=nowlasttime;i++) {
                int xx=ThisRoute[i].first;
                int yy=ThisRoute[i].second;
                if (NowWalked[xx][yy]==0 && (Save[Nowtime].dirty[xx][yy].size()>0 || nowstatus[xx][yy]==2)) {
                    if (Save[Nowtime].dirty[xx][yy].size()>0) nowcnt++;
                    if (nowcnt<=3) {
                        WashRoute.push_back(std::make_pair(std::make_pair(i+NeedAddTime,1),ThisRoute[i]));
                        NowWalked[xx][yy]=1;
                        if (nowstatus[xx][yy]==2) nowstatus[xx][yy]=0;
                    } else {
                        WashRoute.push_back(std::make_pair(std::make_pair(i+NeedAddTime,0),ThisRoute[i]));
                    }
                }
                else {
                    WashRoute.push_back(std::make_pair(std::make_pair(i+NeedAddTime,0),ThisRoute[i]));
                }
            }
            //
            hasclean=0;
            for(int i=1;i<=Row;i++) {
                if (hasclean) break;
                for(int j=1;j<=Column;j++) {
                    if (nowstatus[i][j]==2) {
                        hasclean=1;
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::on_pushButtonStartClean_clicked()
{
    if (isClean==0) {
        QMessageBox::critical(NULL, "Warning", tr("You should click 'Cleaning' first"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else {
        nowPlaying=1;
        WashRoute.clear();

        for(int i=1;i<=Row;i++)
            for(int j=1;j<=Column;j++) {
                if (Save[Nowtime].nowclick[i][j]) {
                    nowstatus[i][j]=1;
                }
                else {
                    for(int k1=-1;k1<=1;k1++) {
                        if (nowstatus[i][j]==1) break;
                        for(int k2=-1;k2<=1;k2++) {
                            int xx=i+k1,yy=j+k2;
                            if (1<=xx && xx<=Row && 1<=yy && yy<=Column && Save[Nowtime].bubble[xx][yy]>0) {
                                nowstatus[i][j]=1;
                                break;
                            }
                        }
                    }
                }
            }

        Calc();

        if (WashRoute.size()==0) {
            QMessageBox::critical(NULL, "Warning", tr("No cleaning path"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else {
            //paint wash play
            sort(WashRoute.begin(),WashRoute.end());
            int nowlasttime=WashRoute[WashRoute.size()-1].first.first;
            int wz=0;
            for(int i=1;i<=nowlasttime;i++) {
                qDebug()<<"Time"<<i;
                //
                //for(int j=1;j<=3e7;j++);
                QTime qtime;
                qtime.start();
                while(qtime.elapsed()<300)
                    QCoreApplication::processEvents();
                //Sleep(100);
                //
                Washxy.clear();
                while (wz<WashRoute.size() && WashRoute[wz].first.first==i) {
                    Washxy.push_back(WashRoute[wz].second);
                    if (WashRoute[wz].first.second)
                        Save[Nowtime].dirty[WashRoute[wz].second.first][WashRoute[wz].second.second].clear();
                    wz++;
                }
                repaint();
            }
            //paint the last
            //for(int j=1;j<=3e7;j++);
            QTime qtime;
            qtime.start();
            while(qtime.elapsed()<300)
                QCoreApplication::processEvents();
            //Sleep(100);
            Washxy.clear();
            repaint();
            //
            QMessageBox::information(NULL, "Information", "Clean up",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }

        nowPlaying=0;
    }
    ui->radioButtonCleaning->setChecked(false);
    isClean=0;
    CleanInx=CleanIny=CleanOutx=CleanOuty=CleanLimit=0;
    Washxy.clear();
}

void MainWindow::StartClear()
{
    nowPlaying=1;
    WashRoute.clear();

    for(int i=1;i<=Row;i++)
        for(int j=1;j<=Column;j++) {
            if (Save[Nowtime].nowclick[i][j]) {
                nowstatus[i][j]=1;
            }
            else {
                for(int k1=-1;k1<=1;k1++) {
                    if (nowstatus[i][j]==1) break;
                    for(int k2=-1;k2<=1;k2++) {
                        int xx=i+k1,yy=j+k2;
                        if (1<=xx && xx<=Row && 1<=yy && yy<=Column && Save[Nowtime].bubble[xx][yy]>0) {
                            nowstatus[i][j]=1;
                            break;
                        }
                    }
                }
            }
        }
    Calc();

    if (WashRoute.size()>0) {
        //paint wash play
        sort(WashRoute.begin(),WashRoute.end());
        int nowlasttime=WashRoute[WashRoute.size()-1].first.first;
        int wz=0;
        for(int i=1;i<=nowlasttime;i++) {
            qDebug()<<"Wash Time"<<i;
            //
            //for(int j=1;j<=3e7;j++);
            QTime qtime;
            qtime.start();
            while(qtime.elapsed()<300)
                QCoreApplication::processEvents();
            //Sleep(100);
            //
            Washxy.clear();
            while (wz<WashRoute.size() && WashRoute[wz].first.first==i) {
                Washxy.push_back(WashRoute[wz].second);
                if (WashRoute[wz].first.second)
                    Save[Nowtime].dirty[WashRoute[wz].second.first][WashRoute[wz].second.second].clear();
                wz++;
            }
            repaint();
        }
        //paint the last
        //for(int j=1;j<=3e7;j++);
        QTime qtime;
        qtime.start();
        while(qtime.elapsed()<300)
            QCoreApplication::processEvents();
        //Sleep(100);
        Washxy.clear();
        repaint();
        //
    }

    nowPlaying=0;

    Washxy.clear();
}

void MainWindow::on_radioButtonCleaning_clicked(bool checked)
{
    qDebug()<<"Cleaning_clicked"<<checked;
    memset(nowstatus,0,sizeof(nowstatus));
    if (checked==0) {
        CleanInx=CleanIny=CleanOutx=CleanOuty=isClean=CleanLimit=0;
        ui->radioButtonCleaning->setChecked(false);
    }
    else {
        ui->radioButtonCleaning->setChecked(false);
        cleaningdialog = new CleaningDialog(this);
        cleaningdialog->show();
        connect(cleaningdialog,SIGNAL(buildCleaning(int,int,int,int,bool)),this,SLOT(buildCleaning(int,int,int,int,bool)));
    }
}

void MainWindow::on_pushButtonSetClean_clicked()
{
    setcleandialog=new setcleanDialog();
    setcleandialog->show();
    connect(setcleandialog,SIGNAL(AddCleanPlace(int,int)),this,SLOT(AddCleanPlace(int,int)));
}
