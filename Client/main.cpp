#include "mainwindow.h"
#include "game.h"
#include <QApplication>
#include "myclient.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyClient     client("127.0.0.1", 1998,0);
    client.show();

    return a.exec();
}
