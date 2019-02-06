#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    server_status=0;
}

void MainWindow::on_starting_clicked()
{
    this->tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));

    if (!tcpServer->listen(QHostAddress::Any, 1998) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        this->ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        this->ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void MainWindow::newuser()
{
          if (!game_started){
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        if (this->SClients.size() > 2)
        {
            QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
            clientSocket->write("Server is full");
            clientSocket->close();
            return;
        }
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();

        QString msg = "COUNT ";

        msg += QString::number(SClients.size()) + '\n';
        clientSocket->write(msg.toStdString().c_str()); // сообщение о текущем состоянии сервера и количестве игрков а так же информацию о игроках

        //
        int idusersocs=clientSocket->socketDescriptor();
        descs[this->SClients.size()] = idusersocs;
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(disconnected()),this,SLOT(dellUser()));
        connect(SClients[idusersocs], SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        connect(this,SIGNAL(all_ready()),SLOT(send_info()));
        this->tcpServer->listen();
        server_status=1;
        if (SClients.size() == 2 )
        {
            SClients[descs[0]]->write(msg.toStdString().c_str());
            //QThread::msleep(100);
            emit all_ready();
            game_started = true;
        }
        }
    }
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = reinterpret_cast<QTcpSocket*>(sender());
    int desc = clientSocket->socketDescriptor();
    QString desc_str = QString::number(desc);
    QString msg = clientSocket->readLine(40);// поменять
    ui->textinfo->append("ReadClient-"+desc_str+": "+msg+"\n\r");
    parse_msg(msg,desc);
}

void MainWindow::parse(Game *g,QString mes)
{
    //"END!;1,1 2,2 3,3 4,4 0,5;1,1 2,2;1,1 2,2;"
    //STEP     /*ONTABLE*/  /*PLAYER1*//*PLAYER2*/
    Card * table =   new Card [5];
    Player * pl = new Player[2];
    table[0] = Card(mes[5].digitValue(),mes[7].digitValue());
    table[1] = Card(mes[9].digitValue(),mes[11].digitValue());
    table[2] = Card(mes[13].digitValue(),mes[15].digitValue());
    table[3] = Card(mes[17].digitValue(),mes[19].digitValue());
    table[4] = Card(mes[21].digitValue(),mes[23].digitValue());
    pl[0].hand(0) = Card(mes[25].digitValue(),mes[27].digitValue());
    pl[0].hand(1) = Card(mes[29].digitValue(),mes[31].digitValue());
    pl[1].hand(0) = Card(mes[33].digitValue(),mes[35].digitValue());
    pl[1].hand(1) = Card(mes[37].digitValue(),mes[39].digitValue());
    g->setGame(table,pl);
}

void MainWindow::dellUser(int desc)
{
    SClients[desc]->close();
    SClients.remove(desc);
    ui->textinfo->append("User " + QString::number(desc) + " disconected");
    game_started = false;
}

void MainWindow::send_info()
{
    if (!game_started)
    {
        game = new Game();
        game->setRandomGame();
        vector<QString> vec = game->makeInfo();
        SClients[descs[0]]->write(vec[0].toStdString().c_str());
        SClients[descs[1]]->write(vec[1].toStdString().c_str());
        game_started = true;
    }
}

void MainWindow::parse_msg(QString msg,int desc)
{
    if (msg.contains("DISCONECT"))
    {
        dellUser(desc);
        return;
    }
    if (!msg.contains("END") && this->SClients.size() == 2)
    {
        int next_desc;
        QMap<int,QTcpSocket*>::iterator it = this->SClients.find(desc);
        if (it.key() == descs[0])
            next_desc = descs[1];
        else
            next_desc = descs[0];
        SClients[next_desc]->write(msg.toStdString().c_str());
    }
    else if (this->SClients.size() == 2)
    {
        /*int next_desc;
        QMap<int,QTcpSocket*>::iterator it = this->SClients.find(desc);
        if (it.key() == descs[0])
            next_desc = descs[1];
        else
            next_desc = descs[0];
        QString sec_mes = SClients[next_desc]->readAll();

        for (int i =25; i < sec_mes.size();i++)
            msg.push_back(sec_mes[i]);
        Game * game = new Game();
        parse(game,msg);*/
        int num = game->win();
        switch (num) {
        case 0 : {
            SClients[descs[1]]->write("LOSE",5);
            SClients[descs[0]]->write("WIN",4);
        }break;
        case 1 : {
             SClients[descs[1]]->write("WIN",4);
             SClients[descs[0]]->write("LOSE",5);
        }break;
        case 2 : {
            SClients[descs[1]]->write("DRAW",5);
            SClients[descs[0]]->write("DRAW",5);
        }break;
        }
    }
}
