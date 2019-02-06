#include <game.h>

Game::Game()
{
    unsigned short suit = 0;
    for (int i = 0; i < 52; i++)
    {
        if (i%13 == 0 && i != 0)
            suit++;
        this->_deck[i] = Card(suit,i%13);
        //this->_deck[i].show();
    }
    this->_playersCount = 0;
    this->_bank = 0;
}

void Game::addPlayer(QString name)
{
    _players[_playersCount].name() = name;
    _players[_playersCount].Money() = 1000;
}

void Game::delPlayer(int i)
{
    _players[i].name() = "";
    _players[i].Money() = 0;
}

int& Game::Bank()
{
    return this->_bank;
}
int Game::Bank() const
{
    return this->_bank;
}

void Game::setGame(Card * table, Player * players)
{
    for (int i =0; i < 5; i++)
    {
       if(i < 2)
            this->_players[0].hand(i) = players->hand(i);
       this->_onTable[i] = table[i];
    }

}
