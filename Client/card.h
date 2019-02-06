#ifndef CARD_H
#define CARD_H
#include <QPixmap>
#include <QImage>
#include <QDebug>
class Card
{
private:
    enum Suit{
     HEART = 0,
     DIAMOND = 1,
     CLUB = 2,
     SPADE = 3
    };
    unsigned short _suit;
    unsigned short _value;
public:
    Card();
    Card(unsigned short suit, unsigned short value);
    void show();
    unsigned short value() const;
    unsigned short suit() const;
    unsigned short & value();
    unsigned short & suit();
    Card & operator = (const Card&);
    QString  picture();
};

#endif // CARD_H
