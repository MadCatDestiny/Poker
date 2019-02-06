
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <cstdlib>
#include <QMap>
#include <game.h>
#include <player.h>
#include <cstdlib>
#include <QPair>
namespace Ui {
    class MainWindow;
}

class QTcpServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void slotReadClient();
    void parse(Game * g,QString mes);
    void dellUser(int desc);
    void send_info();
    void parse_msg(QString msg,int desc);
signals:
    void all_ready();
private:
    Game * game;
    Player players[2];
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    int server_status;
    int descs [2];
    bool game_started;
    QMap<int,QTcpSocket *> SClients;

};

#endif // MAINWINDOW_H
