#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "player.h"
#include <QString>
#include <algorithm>
#include <vector>
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
    Game();
    void addPlayer(QString);//+
    void delPlayer(int i);  //+
    int& Bank();
    int  Bank() const; //+
    //void step(int action,int sum, int players_num);
    int win();
    void HighCard();        //0+
    void Pair();            //1+
    void TwoPairs();        //2+
    void ThreeOfKind();     //3+
    void Straight();        //4+
    void Flush();           //5+
    void FullHouse();       //6+
    void FourOfKind();      //7+
    void StraightFlush();   //8+
    void RoyalFlush();      //9
    void Msort(Card arr[],int n);
    void setRandomGame();
    vector<QString> makeInfo();
    // for test
    void setGame(Card * table, Player * players);

};

#endif // GAME_H
