#include "lifesupply.h"

LifeSupply::LifeSupply()
{

}

LifeSupply::LifeSupply(int x, int y, const string &imageFile, int energy)
    :Object(LIFESUPPLY, imageFile)
{
    setPos(x, y);
    this->dir = QPointF(0,2);
    this->energy = energy;
}

pair<qreal, qreal> LifeSupply::updatePosition()
{
    return make_pair(x(),y()+3);
}
