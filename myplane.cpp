#include "myplane.h"
#include <iostream>

using namespace std;

MyPlane::MyPlane()
{

}

MyPlane::MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, int movespeed, int shootspeed)
    : Plane(x, y, imageFile, scene, life ,ME)
{
    this->movespeed = movespeed;
    this->shootspeed = shootspeed;
    this->bullettype = 0;
}

void MyPlane::addLife(int var)
{
    this->life = this->life + var;
}

int MyPlane::getSkill(int attribute)
{
    switch (attribute) {
    case 1: //技能一，发射三颗子弹
        if(bullettype == 0)
            bullettype = 1;
        else if(bullettype == 2)
            bullettype = 3;
        return 1;
    case 2:
        if(bullettype == 0)
            bullettype = 2;
        else if(bullettype == 1)
            bullettype = 3;
        return 2;
    case 3:
        if (skills[3]>=1)
            skills[3] = 1;
        else
            skills[3]++;
        break;
    case 4:
        if (skills[4]>=1)
            skills[4] = 1;
        else
            skills[4]++;
        break;
    default:
        break;
    }
    return attribute;
}

//使用主动技能, 返回是否成功使用
bool MyPlane::useSkill(int attribute)
{
    switch (attribute) {
    case 3:
        if (skills[3] <= 0)
            skills[3] = 0;
        else
        {
            skills[3]--;
            return true;
        }
        break;
    case 4:
        if (skills[4] <= 0)
            skills[4] = 0;
        else
        {
            skills[4]--;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}
