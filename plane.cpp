#include "plane.h"
#include <cstdlib>

Plane::Plane()
{

}

Plane::Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part)
    : Object(part, imageFile)
{
    setPos(x, y);//设置坐标
    this->life = life;//设置生命值
    scene->addItem(this);
    update();
}

//碰撞检测
bool Plane::crash(QGraphicsScene *scene, int power)
{
    if (life <= 0) //生命值不大于0
        return false;

    life += power;
    if (life<= 0) //此次碰撞后生命值不大于0
    {
        delScreen(scene);
        return false;
    }
    else
        return true;
}


