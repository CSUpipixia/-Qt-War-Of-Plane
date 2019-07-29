#include "skillprop.h"

SkillProp::SkillProp()
{

}

SkillProp::SkillProp(int x, int y, const string &imageFile, int attribute) :
    Object(SKILLPROP, imageFile)
{
    setPos(x, y);
    this->attribute = attribute;
    srand(time(NULL));
    bool i = rand() % 2;
    if (i)
    {
        this->xdir = 3;
    }
    else
    {
        this->xdir = -3;
    }
}

pair<qreal, qreal> SkillProp::updatePosition()
{
    return make_pair(x()+xdir, y()+3);
}

void SkillProp::changeXdir()
{
    this->xdir *= -1;
}

bool SkillProp::crash(QGraphicsScene *scene)
{

}

