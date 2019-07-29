#ifndef MYPLANE_H
#define MYPLANE_H

#include "plane.h"

class MyPlane : protected Plane
{
    friend class Control;
public:
    MyPlane();
    MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, int movespeed, int shootspeed);
    void addLife(int var);
    int getSkill(int attribute);
    bool useSkill(int attribute);

protected:
    int movespeed;        //移速
    int shootspeed;       //射速
    int skills[5] = {0,0,0,0,0};  //技能值
    int bullettype;       //子弹类型
};

#endif // MYPLANE_H
