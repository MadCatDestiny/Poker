#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <game.h>
#include <QTimer>
#include <QThread>
#include <thread>
#include <QMessageBox>
//#include <QtConcurrentRun>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTcpSocket* _sock,QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void addPLayer();
    void set_num(int n);
    void set_game(QString mes);
    void set_money(size_t pl_num);
    void start_timer();
    void show_hand();
    void show_rival_hand();
    void show_flop();
    void show_turn();
    void show_river();
    void raise();
    void call();
    void rival_raise(int rate);
    void set_next_step();
    void check();
    void rival_fold();
    void rival_chek();
    void end_game();
    void win(QString res);
signals:
    void all_ready();
    void flop();
    void turn();
    void river();
    void finished(int);
    void show_next();

private slots:
    void on_exit_bt_clicked();
    void reset_timer();
    void start_game();
    void closeEvent(QCloseEvent*);

    void on_fold_bt_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    friend class Helper;
    int num;
    size_t step_num;
    size_t bank;
    QVector <int> money;
    QVector <int> rate;
    Ui::MainWindow *ui;
    Game *game;
    QTcpSocket *sock;
    //graphic items
    QGraphicsScene * scene;
    QGraphicsTextItem * _bank;
    QVector <QGraphicsTextItem*>   _rate;
    QVector <QGraphicsTextItem*>   _players;
    QVector <QGraphicsPixmapItem*> _chips;
    QVector <QGraphicsPixmapItem*> _myCard;
    QVector <QGraphicsPixmapItem*> _onTable;
    QVector <QGraphicsPixmapItem*> _rivalCard;

};
class Helper: public QThread {
public:

    static void msleep(unsigned long ms)
    {
        QThread::msleep(ms);
    }
    void run()
    {
        //start_timer();
    }
};
#endif // MAINWINDOW_H
