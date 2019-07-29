#include "enemyplane.h"
#include <cstdlib>
#include <ctime>

EnemyPlane::EnemyPlane()
{

}

EnemyPlane::EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life):
    Plane(x, y, imageFile, scene, life, ENEMY)
{
    this->type = type;
    this->ydir = 2;
    srand(time(NULL));
    this->xdir = rand() % 5 - 2;
}

pair<int,int> EnemyPlane::updatePostion()
{
    return make_pair(x()+xdir, y()+ydir); //前进2
}

void EnemyPlane::changeXdir()
{
    this->xdir *= -1;
}
