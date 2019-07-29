#ifndef SKILLPROP_H
#define SKILLPROP_H
#include <ctime>

#include "object.h"

class SkillProp : public Object
{
    friend class Control;
public:
    SkillProp();
    SkillProp(int x, int y, const string &imageFile, int attribute);
    pair<qreal, qreal> updatePosition();
    void changeXdir();
    bool crash(QGraphicsScene *scene);

protected:
    int xdir;
    int attribute; //属性
};

#endif // SKILLPROP_H
