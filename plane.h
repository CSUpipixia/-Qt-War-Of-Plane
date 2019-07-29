#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include <QtWidgets>
#include <cstring>
#include <vector>

using namespace std;

class Plane : public Object
{
    friend class Control;
public:
    Plane();
    //x坐标，y坐标，图片，图形元素，类型
    Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part);
    bool crash(QGraphicsScene *scene, int power);//碰撞检测

protected:
    //生命值
    int life;
};

#endif // PLANE_H
