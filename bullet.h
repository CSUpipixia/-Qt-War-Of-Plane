#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include <utility>
#include <fstream>

using namespace std;

class Bullet:public Object
{
    friend class Control;
public:
    Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power);
    pair<qreal, qreal> updatePosition();
    bool hit(QGraphicsScene *Scene);

protected:
    QPointF dir;//子弹方向，同时也控制了子弹速度
    int power;//子弹杀伤力
};

#endif // BULLET_H
