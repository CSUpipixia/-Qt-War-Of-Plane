#ifndef MISSILE_H
#define MISSILE_H
#include <math.h>
#include "object.h"

class Missile : public Object
{
    friend class Control;
public:
    Missile();
    Missile(int x, int y, const string &imageFile, int life, int power);
    pair<qreal, qreal> updatePosition(int targetX, int targetY);
    bool hit(QGraphicsScene *Scene);

protected:
    int life;  //生命值
    int power; //子弹杀伤力
    int speed; //速度
    int xdir;  //x方向
    int ydir;  //y方向
};

#endif // MISSILE_H
