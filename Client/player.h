#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "card.h"
#include <iostream>
#include <cstdlib>
class Player
{
private:
    QString         _name;
    size_t          _money;
    Card            _hand[2];
    size_t          _combination[2];
public:
    Player();               //+
    QString make_info() const; //+
    QString& name ();       //+
    size_t & Money();       //+
    size_t Money () const;  //+
    Card hand(int i) const; //+
    Card & hand(int i);     //+
    void combination(size_t  num_of_comb,size_t val_of_hight_card); //+
    Player & operator = (const Player&); //+

};

#endif // PLAYER_H
