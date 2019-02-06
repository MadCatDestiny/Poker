#include <player.h>
using namespace  std;
Player::Player()
{
    this->_combination[0] = 0;
    this->_combination[1] = 0;
}

QString Player::make_info() const
{
    QString info;
    info = _name;
    info += ";";
    info += QString::number(_money);
    return info;
}

QString &Player::name()
{
    return this->_name;
}

size_t &Player::Money()
{
    return this->_money;
}

size_t Player::Money() const
{
    return this->_money;
}

Card Player::hand(int i) const
{
    return _hand[i];
}

Card &Player::hand(int i)
{
     return _hand[i];
}

void Player::combination(size_t num_of_comb = 0, size_t val_of_hight_card = 0)
{
    this->_combination[0] = num_of_comb;
    this->_combination[1] = val_of_hight_card;
    cout << "num_of_comb = " << num_of_comb << endl;
    cout << "val_of_hight_card = " << val_of_hight_card <<endl;
}

size_t *Player::combination()
{
    return _combination;
}

Player &Player::operator =(const Player & p1)
{
    this->_name = p1._name;
    this->_money = p1._money;
    for (int i = 0; i < 2; i++)
    {
        this->_hand[i] = p1._hand[i];
        this->_combination[i] = p1._combination[i];
    }
    return *this;
}
