#ifndef LIFESUPPLY_H
#define LIFESUPPLY_H

#include "object.h"

class LifeSupply: public Object
{
    friend class Control;
public:
    LifeSupply();
    LifeSupply(int x, int y, const string &imageFile, int energy);
    pair<qreal, qreal> updatePosition();

private:
    QPointF dir;   //补给移动方向
    int energy;    //补给补充生命值
};

#endif // LIFESUPPLY_H
