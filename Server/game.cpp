#include "game.h"
#include <ctime>
#include <cstdlib>
using namespace std;
bool cmp_val(const Card & c1, const Card & c2 )
{return (c1.value() > c2.value()) ? true : false;}

Game::Game()
{
    unsigned short suit = 0;
    for (int i = 0; i < 52; i++)
    {
        if (i%13 == 0 && i != 0)
            suit++;
        this->_deck[i] = Card(suit,i%13);
        this->_deck[i].show();
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
/*void Game::step(int action, int sum, int players_num)
{
     Actions:
        1)call/raise (1,+|- sum, 0|1)
        2)check (2,0,0|1)
        3)

    switch(action)
    {
        case 1 :
        {
            Bank() += sum;
            _players[players_num].Money() -=  sum;
            break;
        }
    }
}*/

int Game::win()
{

    Msort(this->_onTable,5);
    HighCard();
    Pair();
    TwoPairs();
    ThreeOfKind();
    Straight();
    Flush();
    FullHouse();
    FourOfKind();
    StraightFlush();
    RoyalFlush();

    if (_players[0].combination()[0] > _players[1].combination()[0])
        return 0;
    else if (_players[0].combination()[0] < _players[1].combination()[0])
        return 1;
    else if (_players[0].combination()[0] == _players[1].combination()[0])
    {
        if (_players[0].combination()[1] > _players[1].combination()[1])
            return 0;
        else if (_players[0].combination()[1] < _players[1].combination()[1])
            return 1;
        else if(_players[0].combination()[1] == _players[1].combination()[1])
            return 2;
    }
}

void Game::HighCard()
{
    for (int i =0; i < 2; i++)
    {
        if(_players[i].hand(0).value() >= _players[i].hand(1).value())
            _players[i].combination(0,_players[i].hand(0).value());
        else
            _players[i].combination(0,_players[i].hand(1).value());
    }
}

void Game::Pair()
{
    for (int i =0; i < 2; i++)
    {
        if (_players[i].hand(0).value()==_players[i].hand(1).value())
            _players[i].combination(1,_players[i].hand(0).value());
        for(int j =0; j < 2; j++)
        {
            for (int k =0; k < 5; k++)
            {
                if (_players[i].hand(j).value() == this->_onTable[k].value())
                    _players[i].combination(1,_players[i].hand(j).value());
            }
        }
    }
}

void Game::TwoPairs()
{
    for (int i =0; i < 2; i++)
    {
        if (_players[i].hand(0).value()==_players[i].hand(1).value())
        {
            for(int j = 0; j < 5; j++)
            {
                for(int n = j; n < 5; n++)
                {
                    if (this->_onTable[j].value() == this->_onTable[n].value())
                    {
                        if (_players[i].hand(0).value() > this->_onTable[j].value())
                            _players[i].combination(2,_players[i].hand(0).value());
                        else
                            _players[i].combination(2,this->_onTable[j].value());
                        return;
                    }

                }
            }
            return;
        }
        int n = 0;
        for(int j =0; j < 2; j++)
        {
            for (int k = n; k < 5; k++)
            {
                if (_players[i].hand(j).value() == this->_onTable[k].value())
                {
                    if (k >= n )
                        swap(this->_onTable[k],this->_onTable[n]);
                    n++;
                    break;
                }
            }
        }
        if (_players[i].hand(0).value() == this->_onTable[0].value()
                && _players[i].hand(1).value() == this->_onTable[1].value())
        {
            if(this->_onTable[0].value() > this->_onTable[1].value())
                _players[i].combination(2,this->_onTable[0].value());
            else
                _players[i].combination(2,this->_onTable[1].value());
        }
    }
}

void Game::ThreeOfKind()
{
    for (int i =0; i < 2; i++)
    {
        if (_players[i].hand(0).value() == _players[i].hand(1).value())
        {
            for (int j =0; j < 5; j++)
            {
                if (this->_onTable[j].value() == _players[i].hand(0).value())
                {
                    _players[i].combination(3,_players[i].hand(0).value());
                    break;
                }
            }
        }
        else
        {
            for(int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    if(_players[i].hand(j).value() == _onTable[k].value() &&
                            _players[i].hand(j).value() == _onTable[k+1].value())
                    {
                        _players[i].combination(3,_players[i].hand(0).value());
                        break;
                    }
                }
            }
        }
    }
}

void Game::Straight()
{
    Card table_p1[7];
    Card table_p2[7];

    table_p1[0] = _players[0].hand(0);
    table_p1[1] = _players[0].hand(1);
    table_p2[0] = _players[1].hand(0);
    table_p2[1] = _players[1].hand(1);
    for (int i = 2; i < 7; i++)
    {
        table_p1[i] = _onTable[i-2];
        table_p2[i] = _onTable[i-2];
    }
    Msort(table_p1,7);
    Msort(table_p2,7);
    Msort(_onTable,5);
    int count =0;
    bool straight_onTable = false;
    for (int i =1; i < 5; i++)
    {
        if(_onTable[i].value() - _onTable[i-1].value() == 1)
            count ++;
    }
    if (_onTable[4].value() - _onTable[0].value() == 12)
        count++;
    if (count >= 4)
        straight_onTable = true;
    count =0;
    if (!straight_onTable)
    {
        size_t max_val = 0;
        for (int i =1; i <7; i++)
        {
            if (table_p1[i].value() - table_p1[i-1].value() == 1)
            {
                count++;
                max_val = table_p1[i].value();
                if (count >= 4)
                    break;
            }
            else
            {
                if (max_val == 3 && count == 3 && table_p1[6].value() - table_p1[0].value() == 12)
                {
                    count++;
                    break;
                }
                if (i > 2 && count < 4)
                    break;
            }
        }

        if (count >= 4 )
        {
            _players[0].combination(4,max_val);
        }
        //
        count = 0;
        max_val =0;
        //
        for (int i =1; i <7; i++)
        {
            if (table_p2[i].value() - table_p2[i-1].value() == 1)
            {
                count++;
                max_val = table_p2[i].value();
                if (count >= 4)
                    break;
            }
            else
            {
                if (max_val == 3 && count == 3 && table_p2[6].value() - table_p2[0].value() == 12)
                {
                    count++;
                    break;
                }
                if (i > 2 && count < 4)
                    return;
            }
        }

        if (count >= 4)
        {
            _players[1].combination(4,max_val);
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if(_players[i].hand(j).value() - _onTable[0].value() == -1 ||
                        _players[i].hand(j).value() - _onTable[0].value() == 12)
                    _players[i].combination(4,_onTable[3].value());

                if (_players[i].hand(j).value() - _onTable[4].value() == 1)
                    _players[i].combination(4,_players[i].hand(j).value());


            }
        }
    }
}

void Game::Flush()
{
    for (int i = 0; i < 2; i++)
    {
        size_t count = 0;
        size_t max_val =0;
        if(_players[i].hand(0).suit() == _players[i].hand(1).suit())
        {
            if (_players[i].hand(0).value() > _players[i].hand(1).value())
                max_val = _players[i].hand(0).value();
            else
                max_val = _players[i].hand(1).value();

            for (int j = 0; j < 5; j++)
                if (_players[i].hand(0).suit() == _onTable[j].suit())
                {
                    count++;
                    if (_onTable[j].value() > max_val)
                        max_val = _onTable[j].value();
                }
            if (count >= 3)
                _players[i].combination(5,max_val);
        }
        else
        {
            for(int j = 0; j < 2; j++)
            {
                count = 0;
                max_val = _players[i].hand(j).value();
                for (int k = 0; k < 5; k++)
                {
                    if (_players[i].hand(j).suit() == _onTable[k].suit())
                    {
                        count++;
                        if (_onTable[k].value() > max_val)
                            max_val = _onTable[k].value();
                    }
                }
                if (count >= 4)
                    _players[i].combination(5,max_val);
            }
        }
    }
}

void Game::FullHouse()
{
    Card table_p1[7];
    Card table_p2[7];
    table_p1[0] = _players[0].hand(0);
    table_p1[1] = _players[0].hand(1);
    table_p2[0] = _players[1].hand(0);
    table_p2[1] = _players[1].hand(1);
    for (int i = 2; i < 7; i++)
    {
        table_p1[i] = _onTable[i-2];
        table_p2[i] = _onTable[i-2];
    }
    Msort(table_p1,7);
    Msort(table_p2,7);
    Msort(_onTable,5);
    size_t max_val;
    if ((_onTable[0].value() == _onTable[1].value() &&
         _onTable[2].value() == _onTable[3].value() &&
         _onTable[3].value() == _onTable[4].value()) ||
            (_onTable[0].value() == _onTable[1].value() &&
             _onTable[1].value() == _onTable[2].value() &&
             _onTable[3].value() == _onTable[4].value()))
    {
        for (int i =0; i <2; i++)
        {
            if(_players[i].hand(0).value() == _onTable[0].value() ||
                    _players[i].hand(1).value() == _onTable[0].value())
                _players[i].combination(6,_onTable[4].value());
            if(_players[i].hand(0).value() == _onTable[4].value() ||
                    _players[i].hand(1).value() == _onTable[4].value())
                _players[i].combination(6,_onTable[4].value());
        }
        return;
    }
    for (int i = 1; i < 7; i++)
    {
        if (table_p1[i].value() == table_p1[i-1].value())
        {
            if (table_p1[i].value() == table_p1[i+1].value())
            {
                max_val = table_p1[i].value();
                for (int j = i+2; j < 7; j++)
                {
                    if (table_p1[j].value() == table_p1[j-1].value())
                    {
                        if (max_val < table_p1[j].value())
                            max_val = table_p1[j].value();
                        _players[0].combination(6,max_val);
                        break;
                    }

                }
            }
            else
            {
                max_val = table_p1[i].value();
                for (int j = i+2; j < 7; j++)
                {
                    if (table_p1[j].value() == table_p1[j-1].value() &&
                            table_p1[j].value() == table_p1[j+1].value())
                    {
                        if (max_val < table_p1[j].value())
                            max_val = table_p1[j].value();
                        _players[0].combination(6,max_val);
                        break;
                    }
                }
            }
        }
    }
    for (int i = 1; i < 7; i++)
    {
        if (table_p2[i].value() == table_p2[i-1].value())
        {
            if (table_p2[i].value() == table_p2[i+1].value())
            {
                max_val = table_p2[i].value();
                for (int j = i+2; j < 7; j++)
                {
                    if (table_p2[j].value() == table_p2[j-1].value())
                    {
                        if (max_val < table_p2[j].value())
                            max_val = table_p2[j].value();
                        _players[1].combination(6,max_val);
                        break;
                    }

                }
            }
            else
            {
                max_val = table_p2[i].value();
                for (int j = i+2; j < 7; j++)
                {
                    if (table_p2[j].value() == table_p2[j-1].value() &&
                            table_p2[j].value() == table_p2[j+1].value())
                    {
                        if (max_val < table_p2[j].value())
                            max_val = table_p2[j].value();
                        _players[1].combination(6,max_val);
                        break;
                    }
                }
            }
        }
    }
}

void Game::FourOfKind()
{
    for (int i =0; i < 2; i++)
    {
        if (_players[i].hand(0).value() == _players[i].hand(1).value())
        {
            for (int j =0; j < 5; j++)
            {
                if (this->_onTable[j].value() == _players[i].hand(0).value() &&
                        this->_onTable[j+1].value() == _players[i].hand(0).value())
                {
                    _players[i].combination(7,_players[i].hand(0).value());
                    break;
                }
            }
        }
        else
        {
            for(int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    if(_players[i].hand(j).value() == _onTable[k].value() &&
                            _players[i].hand(j).value() == _onTable[k+1].value() &&
                            _players[i].hand(j).value() == _onTable[k+2].value())
                    {
                        _players[i].combination(7,_players[i].hand(0).value());
                        break;
                    }
                }
            }
        }
    }
}

void Game::StraightFlush()
{
    Card table_p1[7];
    Card table_p2[7];

    table_p1[0] = _players[0].hand(0);
    table_p1[1] = _players[0].hand(1);
    table_p2[0] = _players[1].hand(0);
    table_p2[1] = _players[1].hand(1);
    for (int i = 2; i < 7; i++)
    {
        table_p1[i] = _onTable[i-2];
        table_p2[i] = _onTable[i-2];
    }
    Msort(table_p1,7);
    Msort(table_p2,7);
    Msort(_onTable,5);
    int count =0, same_suits = 0;
    bool straight_onTable = false;
    bool flush_onTable = false;
    for (int i =1; i < 5; i++)
    {
        if(_onTable[i].value() - _onTable[i-1].value() == 1)
            count ++;
        if (_onTable[i].suit() == _onTable[i-1].suit())
            same_suits++;
    }

    if (count >= 4)
        straight_onTable = true;
    if (same_suits >= 4)
        flush_onTable = true;
    count =0;
    if (!straight_onTable)
    {
        size_t max_val = 0;
        for (int i =1; i <7; i++)
        {
            if ((table_p1[i].value() - table_p1[i-1].value() == 1 || table_p1[i].value() - table_p1[i-1].value() == -12)&&
                    table_p1[i].suit() == table_p1[i-1].suit())
            {
                count++;
                max_val = table_p1[i].value();
                if (count >= 4)
                    break;
            }
            else
                if (i > 2 && count < 4)
                    return;
        }

        if (count >= 4 )
        {
            _players[0].combination(8,max_val);
        }
        count = 0;

        for (int i =1; i <7; i++)
        {
            if ((table_p2[i].value() - table_p2[i-1].value() == 1 ||
                 table_p2[i].value() - table_p2[i-1].value() == -12)&&  table_p2[i].suit() == table_p2[i-1].suit())
            {
                count++;
                max_val = table_p1[i].value();
                if (count >= 4)
                    break;
            }
            else
                if (i > 2 && count < 4)
                    return;
        }

        if (count >= 4)
        {
            _players[1].combination(8,table_p2[6].value());
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if(_players[i].hand(j).value() - _onTable[0].value() == -1 ||
                        _players[i].hand(j).value() - _onTable[0].value() == 12)
                {_players[i].combination(4,_onTable[3].value());}

                if (_players[i].hand(j).value() - _onTable[4].value() == 1)
                {_players[i].combination(8,_players[i].hand(j).value());}


            }
        }
    }
}

void Game::RoyalFlush()
{
    Card table_p1[7];
    Card table_p2[7];
    table_p1[0] = _players[0].hand(0);
    table_p1[1] = _players[0].hand(1);
    table_p2[0] = _players[1].hand(0);
    table_p2[1] = _players[1].hand(1);
    for (int i = 2; i < 7; i++)
    {
        table_p1[i] = _onTable[i-2];
        table_p2[i] = _onTable[i-2];
    }
    Msort(table_p1,7);
    Msort(table_p2,7);
    Msort(_onTable,5);
    size_t count=0;
    if (_onTable[0].value() == 8)
    {
        for(int i=1; i < 5; i++)
        {
            if (_onTable[i].value() - _onTable[i-1].value() == 1 &&
                    _onTable[i].suit() == _onTable[0].suit())
                count++;
        }
        if (count == 4)
            return;
    }
    else
    {
        for(int i=0; i< 7; i++)
        {
            if (table_p1[i].value() - table_p1[i-1].value() == 1 &&
                    table_p1[i].suit() == table_p1[0].suit() &&
                    table_p1[i-1].value() >= 8)
                count++;
        }
        if (count == 4)
        {_players[0].combination(9,12);return;}
        count =0;
        for(int i=0; i< 7; i++)
        {
            if (table_p2[i].value() - table_p2[i-1].value() == 1 &&
                    table_p2[i].suit() == table_p2[0].suit() &&
                    table_p2[i-1].value() >= 8)
                count++;
        }
        if (count == 4)
        {_players[1].combination(9,12);return;}
    }
}

void Game::Msort(Card arr[],int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j =0 ; j < n-i - 1; j++)
            if (arr[j].value() >= arr[j+1].value())
                swap(arr[j],arr[j+1]);

}

void Game::setGame(Card * table, Player * players)
{
    for (int i =0; i < 5; i++)
    {
        if (i < 2)
        {
            this->_players[i] = players[i];
        }
        this->_onTable[i] = table[i];
    }
}

void Game::setRandomGame()
{
    srand(time(NULL));
    vector<int> idx;
    int i=0;
    generate_n(back_inserter(idx),51,[&i]()->int {return i++;});
    random_shuffle(idx.begin(),idx.end());
    //copy(idx.begin(),idx.end(),ostream_iterator<int>(cout," "));
    int id;
    for (id =0; id < 5;id++ )
        _onTable[id] = _deck[idx[id]];
    for (int i = 0; i < 2; i++)
        for (int j =0; j < 2; id++,j++)
            _players[i].hand(j) = _deck[idx[id]];
}

vector<QString> Game::makeInfo()
{
    QString fmsg = "START";
    QString smsg = fmsg;
    for (int i =0; i < 5; i++)
    {
        fmsg+=QString::number(_onTable[i].suit()) + "," + QString::number(_onTable[i].value()) + " ";
    }

    fmsg.truncate(fmsg.size() - 1);
    smsg = fmsg + " ";
    fmsg = smsg;
    for (int i =0; i < 2; i++)
    {
        fmsg+=QString::number(_players[0].hand(i).suit()) + "," + QString::number(_players[0].hand(i).value()) + " ";
        smsg+=QString::number(_players[1].hand(i).suit()) + "," + QString::number(_players[1].hand(i).value()) + " ";
    }
    fmsg[fmsg.size()-1] = ' ';
    smsg[smsg.size()-1] = ' ';
    vector <QString> vec;
    vec.push_back(fmsg);
    vec.push_back(smsg);
    return vec;
}

