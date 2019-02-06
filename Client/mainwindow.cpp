#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "myclient.h"
MainWindow::MainWindow(QTcpSocket* _sock, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), sock(_sock)
{
    // set background
    this->ui->setupUi(this);
    this->game = new Game();
    this->scene = new QGraphicsScene();
    this->ui->graphicView->setScene(scene);
    this->scene->setSceneRect(0,0,1030,540);
    this->scene->setBackgroundBrush(QBrush(QImage(":/img/back.jpg")));
    QGraphicsPixmapItem* pPixmapItem =scene->addPixmap(QPixmap::fromImage(QImage(":/img/table_top.png")));
    pPixmapItem->setPos(0,0);
    QGraphicsPixmapItem* cards =scene->addPixmap(QPixmap::fromImage(QImage(":/img/rub.jpg")));
    cards->setPos(120,160);
    QGraphicsPixmapItem* placement =scene->addPixmap(QPixmap::fromImage(QImage(":/img/placemaent.png")));
    placement->setPos(350,200);

    //set items
    this->ui->graphicView = new QGraphicsView(this->scene);
    this->ui->progressBar->setValue(0);
    this->ui->spinBox->setMaximum(1000);
    this->ui->spinBox->setMinimum(0);
    this->_onTable.resize(5);
    this->_myCard.resize(2);
    this->_rivalCard.resize(2);
    this->_players.resize(2);
    this->_chips.resize(2);
    this->_rate.resize(2);
    this->rate.resize(2);
    this->money.resize(2);
    this->_rivalCard.resize(2);
    this->money[0] = 1000;
    this->money[1] = 1000;
    this->bank = 0;
    this->step_num = 0;
    this->_players[0] = new QGraphicsTextItem("First - 1000");
    this->scene->addItem(this->_players[0]);
    this->_players[0]->setPos(450,40);
    this->_players[0]->setDefaultTextColor(Qt::red);
    this->_players[0]->setTextWidth(QFont::Bold);
    this->_bank = new QGraphicsTextItem("0AAA0");
    this->scene->addItem(this->_bank);
    this->_bank->setPos(165,240);
    this->ui->call_bt->setEnabled(false);
    this->ui->raise_bt->setEnabled(false);
    //set signal & slot
    //connect(ui->call_bt,SIGNAL(clicked()),SLOT(reset_timer()));
    //connect(ui->check_bt,SIGNAL(clicked()),SLOT(reset_timer()));
    //connect(ui->raise_bt,SIGNAL(clicked()),this,SLOT(reset_timer()));
    connect(ui->call_bt,SIGNAL(clicked()),this,SLOT(call()));
    connect(ui->raise_bt,SIGNAL(clicked()),this,SLOT(raise()));
    connect(ui->check_bt,SIGNAL(clicked()),this,SLOT(check()));
    //connect(this,SIGNAL(next_step()),SLOT(set_next_step()));
    connect(this,SIGNAL(all_ready()),SLOT(show_hand()));
    connect(this,SIGNAL(all_ready()),SLOT(show_rival_hand()));
    connect(this,SIGNAL(show_next()),SLOT(set_next_step()));
    //connect(ui->fold_bt,SIGNAL(clicked()),SLOT(reset_timer()));

    //for test
    //connect(ui->fold_bt,SIGNAL(clicked()),this,SLOT(set_game()));
    //connect(this,SIGNAL(all_ready()),SLOT(start_game()));

    //connect(this,SIGNAL(all_ready()),SLOT(show_flop()));
   // connect(this,SIGNAL(all_ready()),SLOT(show_turn()));
    //connect(this,SIGNAL(all_ready()),SLOT(show_river()));
   // connect(this,SIGNAL(all_ready()),SLOT(rival_raise(100)));
    //

}
void MainWindow::closeEvent(QCloseEvent* event)
{
    this->on_exit_bt_clicked();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset_timer()
{
    this->ui->progressBar->setValue(100);
}

void MainWindow::start_game()
{
   // std::thread*t = new std::thread(&MainWindow::start_timer,this);
    //t->detach();
   /* QThread t;
    connect(&t,&QThread::started,this,&MainWindow::start_timer);
    connect(this,&MainWindow::finished,&t,&QThread::terminate);
    this->ui->progressBar->moveToThread(&t);
    t.start();*/
}

void MainWindow::start_timer()
{
    for (int i =0; i <= 10; i++)
    {
        Helper::msleep(1000);
        this->ui->progressBar->setValue(i*10);
    }
    emit finished(this->ui->progressBar->value());
}

void MainWindow::show_hand()
{
    this->ui->graphicView->setScene(scene);
    this->_myCard[0] = scene->addPixmap(QPixmap::fromImage(QImage(game->_players[0].hand(0).picture())));
    this->_myCard[1] = scene->addPixmap(QPixmap::fromImage(QImage(game->_players[0].hand(1).picture())));
    this->_myCard[0]->setPos(400,70);
    this->_myCard[1]->setPos(500,70);
}

void MainWindow::show_rival_hand()
{
    this->ui->graphicView->setScene(scene);
    this->_rivalCard[0] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/b_card.png")));
    this->_rivalCard[1] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/b_card.png")));
    this->_rivalCard[0]->setPos(395,340);
    this->_rivalCard[1]->setPos(495,340);
}

void MainWindow::show_flop()
{
    this->ui->graphicView->setScene(scene);
    this->_onTable[0] = scene->addPixmap(QPixmap::fromImage(QImage(game->_onTable[0].picture())));
    this->_onTable[1] = scene->addPixmap(QPixmap::fromImage(QImage(game->_onTable[1].picture())));
    this->_onTable[2] = scene->addPixmap(QPixmap::fromImage(QImage(game->_onTable[2].picture())));
    this->_onTable[0]->setPos(358,208);
    this->_onTable[1]->setPos(436,208);
    this->_onTable[2]->setPos(514,208);
}

void MainWindow::show_turn()
{
    this->ui->graphicView->setScene(scene);
    this->_onTable[3] = scene->addPixmap(QPixmap::fromImage(QImage(game->_onTable[3].picture())));
    this->_onTable[3]->setPos(592,208);
}

void MainWindow::show_river()
{
    this->ui->graphicView->setScene(scene);
    this->_onTable[4] = scene->addPixmap(QPixmap::fromImage(QImage(game->_onTable[4].picture())));
    this->_onTable[4]->setPos(667,208);
}

void MainWindow::raise()
{
    QString num = QString::number(this->ui->spinBox->value());
    QString mes = QString("RAISE " + num);
    this->sock->write(mes.toStdString().c_str());
    this->ui->graphicView->setScene(scene);
    this->_chips[0] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/chip.png")));
    this->_chips[0]->setPos(580,80);
    this->_rate[0] =  new QGraphicsTextItem(QString::number(rate[0] + this->ui->spinBox->value()));
    this->_rate[0]->setPos(590,95);
    this->_rate[0]->setDefaultTextColor(Qt::white);
    this->_rate[0]->setTextWidth(QFont::Bold);
    this->scene->addItem(_chips[0]);
    this->scene->addItem(_rate[0]);
    this->_players[0]->setPlainText("First - " + QString::number(money[0] - ui->spinBox->value()));
    this->money[0] -= ui->spinBox->value();
    this->ui->spinBox->setMaximum(money[0]);
    rate[0] += this->ui->spinBox->value();
}

void MainWindow::call()
{
    cout << endl;
    this->ui->spinBox->setValue(rate[1] - rate[0]);
    this->raise();
    this->ui->spinBox->setValue(0);
    this->ui->call_bt->setEnabled(false);
    step_num++;
    emit show_next();
}

void MainWindow::rival_raise(int rate)
{
    this->ui->call_bt->setEnabled(true);
    this->rate[1] += rate;
    this->ui->graphicView->setScene(scene);
    this->_chips[1] = new QGraphicsPixmapItem();
    this->_chips[1] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/chip.png")));
    this->_chips[1]->setPos(580,400);
    this->_rate[1] = new QGraphicsTextItem(QString::number(this->rate[1]));
    this->_rate[1]->setPos(590,415);
    this->_rate[1]->setDefaultTextColor(Qt::white);
    this->_rate[1]->setTextWidth(QFont::Bold);
    this->scene->addItem(_chips[1]);
    this->scene->addItem(_rate[1]);
    this->_players[1]->setPlainText("Second - " + QString::number(money[0] - rate));
    this->money[1] -= rate;
    if (this->rate[0] == this->rate[1])
    {
        step_num++;
        this->ui->call_bt->setEnabled(false);
        emit show_next();
    }
}
void MainWindow::addPLayer()
{
    this->ui->progressBar->setValue(50);
    this->ui->graphicView->setScene(scene);
    this->_players[1] = new QGraphicsTextItem("Second - 1000");
    this->scene->addItem(this->_players[1]);
    this->_players[1]->setPos(450,450);
    this->_players[1]->setDefaultTextColor(Qt::red);
    this->_players[1]->setTextWidth(QFont::Bold);
}

void MainWindow::set_num(int n)
{
    this->num = n;
}

void MainWindow::set_game(QString mes)
{
    // mes = "START1,1 2,2 3,3 0,4 0,5 2,11 2,12;";
    //STEP     /*ONTABLE*/   /*PLAYER1*/
    Card * table =   new Card [5];
    Player * pl = new Player();
    mes.remove(0,5);
    QStringList list = mes.split(" ");
    //
    vector<QStringList> cards;
    cards.resize(7);
    for(int i =0; i < 7; i++)
        cards[i] = list[i].split(",");
    for(int i = 0; i < 5; i++)
        table[i] = Card(cards[i].first().toInt(),cards[i].back().toInt());
    for (int i =0; i < 2; i++)
        pl->hand(i) =  table[5+i] = Card(cards[5+i].first().toInt(),cards[5+i].back().toInt());
    game->setGame(table,pl);
    emit all_ready();
}

void MainWindow::set_money(size_t pl_num)
{
    if (pl_num == 0)
        _players[pl_num]->setPlainText("First - " + QString::number(money[pl_num]));
    else
        _players[pl_num]->setPlainText("Second - " + QString::number(money[pl_num]));
}
void MainWindow::on_exit_bt_clicked()
{
    sock->write("DISCONECT",10);
    QThread::msleep(100);
    sock->disconnectFromHost();

    /*if (sock->isOpen())
        sock->close();
    if(sock1->isOpen())
        sock1->close();*/
    close();
}

void MainWindow::set_next_step()
{
    switch (step_num) {
    case 1: show_flop();break;
    case 2: show_turn();break;
    case 3: show_river();break;
    case 4: end_game();break;
    default: return;
    }
    this->ui->graphicView->setScene(scene);
    _rate[0]->setPlainText(" ");
    _rate[1]->setPlainText(" ");
    bank += rate[0];
    bank += rate[1];
    _bank->setPlainText(QString::number(bank));
    _bank->setDefaultTextColor(Qt::red);
    this->scene->addItem(_rate[0]);
    this->scene->addItem(_rate[1]);
    this->scene->addItem(_bank);

}

void MainWindow::check()
{
    sock->write("CHECK");
    this->ui->graphicView->setScene(scene);
    this->_chips[0] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/chip.png")));
    this->_chips[0]->setPos(580,80);
    this->_rate[0] =  new QGraphicsTextItem("CHECK");
    this->_rate[0]->setPos(590,95);
    this->_rate[0]->setDefaultTextColor(Qt::red);
    this->_rate[0]->setTextWidth(QFont::Bold);
    this->scene->addItem(_chips[0]);
    this->scene->addItem(_rate[0]);
    if (_rate[1])
    {
        if(_rate[1]->toPlainText() == "CHECK")
        {
            step_num++;
            emit show_next();
        }
    }
}



void MainWindow::rival_fold()
{
    on_exit_bt_clicked();
}

void MainWindow::rival_chek()
{
    this->ui->graphicView->setScene(scene);
    this->_chips[1] = new QGraphicsPixmapItem();
    this->_chips[1] = scene->addPixmap(QPixmap::fromImage(QImage(":/img/chip.png")));
    this->_chips[1]->setPos(580,400);
    this->_rate[1] =  new QGraphicsTextItem("CHECK");
    this->_rate[1]->setPos(590,415);
    this->_rate[1]->setDefaultTextColor(Qt::red);
    this->_rate[1]->setTextWidth(QFont::Bold);
    this->scene->addItem(_chips[1]);
    this->scene->addItem(_rate[1]);
    if (_rate[0])
    {
        if(_rate[0]->toPlainText() == "CHECK")
        {
            step_num++;
            emit show_next();
        }
    }
}

void MainWindow::end_game()
{
    sock->write("END");

}

void MainWindow::win(QString res)
{

    QMessageBox * msg = new QMessageBox(QMessageBox::Information,"Result", res,QMessageBox::Ok);
    msg->setModal(true);
    msg->show();
    if(msg->exec())
        on_exit_bt_clicked();
}

void MainWindow::on_fold_bt_clicked()
{
    sock->write("FOLD");
    on_exit_bt_clicked();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (arg1 > 0 )
        ui->raise_bt->setEnabled(true);
    else
        ui->raise_bt->setEnabled(false);
}
