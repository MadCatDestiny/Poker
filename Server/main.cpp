#include <QApplication>
#include <mainwindow.h>
#include <game.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Game * g = new Game();
    Card * table = new Card [5];
    table[0] = Card(1,8);
    table[1] = Card(0,9);
    table[2] = Card(2,10);
    table[3] = Card(0,1);
    table[4] = Card(0,2);
    Player * p = new Player[2];
    p[0].hand(0) = Card(2,12);
    p[0].hand(1) = Card(1,11);
    p[1].hand(0) = Card(2,4);
    p[1].hand(1) = Card(2,7);
    g->setGame(table,p);
    g->win();

   // Game * game = new Game();
   // game->setRandomGame();
   // game->win();
    return a.exec();
}
