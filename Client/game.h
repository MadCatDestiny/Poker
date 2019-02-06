#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "player.h"
#include <QString>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
class Game
{
private:
    Card _deck[51];
    size_t _playersCount;
    int _bank;
    Player _players[2];
    Card _onTable[5];
public:
    friend class MainWindow;
    Game();
    void addPlayer(QString);//+
    void delPlayer(int i);  //+
    int& Bank();
    int  Bank() const; //+
    void setGame(Card * table, Player * players);
    //void step(int action,int sum, int players_num);


};

#endif // GAME_H
