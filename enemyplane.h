#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "plane.h"

//三种类型，ord、boss和bigboss
enum EnemyType {ORD, BOSS, BIGBOSS};

class EnemyPlane : protected Plane
{
    friend class Control;
public:

    EnemyPlane();
    EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType, int life);
    //返回两个int型坐标值
    pair<int, int> updatePostion();
    void changeXdir();

protected:
    EnemyType type;

private:
    int xdir;
    int ydir;
};

#endif // ENEMYPLANE_H
