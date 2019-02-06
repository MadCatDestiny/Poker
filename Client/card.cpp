#include "card.h"



Card::Card()
{

}

Card::Card(unsigned short suit, unsigned short value)
{
    switch (suit) {
    case 0 : this->_suit = Suit::HEART;break;
    case 1 : this->_suit = Suit::DIAMOND;break;
    case 2 : this->_suit = Suit::CLUB;break;
    case 3 : this->_suit = Suit::SPADE;
    }
    this->_value = value;
}

void Card::show()
{

    qDebug() << this->_suit << " : "<<this->_value<<"\n";
}

unsigned short Card::value() const
{
    return this->_value;
}

unsigned short Card::suit() const
{
    return this->_suit;
}

unsigned short &Card::value()
{
    return this->_value;
}

unsigned short &Card::suit()
{
    return this->_suit;
}

Card &Card::operator =(const Card & t1)
{
    this->_suit = t1.suit();
    this->_value = t1.value();

    return *this;
}

QString Card::picture()
{
    QString pic;
    pic.resize(10);
    pic = ":/"+QString::number(this->_suit) +"_"+ QString::number(this->_value) + ".bmp";
    return pic;
}
