#include "missile.h"

Missile::Missile()
{

}

Missile::Missile(int x, int y, const string &imageFile, int life, int power) :
    Object(MISSILE, imageFile)
{
    setPos(x, y);
    this->life = life;
    this->power = power;
    this->speed = 3;
}

pair<qreal, qreal> Missile::updatePosition(int targetX, int targetY)
{
    if (targetY >= this->y())
    {
        if (this->x() >= targetX) //第一象限
        {
            double x = this->x() - targetX;  //水平距离
            double y = targetY - this->y();  //垂直距离
            double distance = sqrt(x*x+y*y); //总距离
            double rad = acos(y/distance);   //弧度
            double angle = (rad*180)/M_PI;   //旋转角度
            setRotation(angle);

            xdir = - speed * x / distance;
            ydir = speed * y / distance;
        }
        else //第二象限
        {
            double x = targetX - this->x();   //水平距离
            double y = targetY - this->y();   //垂直距离
            double distance = sqrt(x*x+y*y);  //总距离
            double rad = acos(y/distance);    //弧度
            double angle = -(rad*180)/M_PI;   //旋转角度
            setRotation(angle);

            xdir = speed * x / distance;
            ydir = speed * y / distance;
        }
    }
    else
    {
        if (targetX >= this->x()) //第三象限
        {
            double x = targetX - this->x();       //水平距离
            double y = this->y() - targetY;       //垂直距离
            double distance = sqrt(x*x+y*y);      //总距离
            double rad = acos(y/distance);        //弧度
            double angle = (rad*180)/M_PI + 180;  //旋转角度
            setRotation(angle);

            xdir = speed * x / distance;
            ydir = - speed * y / distance;
        }
        else //第四象限
        {
            double x = targetX - this->x();  //水平距离
            double y = targetY - this->y();  //垂直距离
            double distance = sqrt(x*x+y*y); //总距离
            double rad = acos(y/distance);   //弧度
            double angle = (rad*180)/M_PI;   //旋转角度
            setRotation(angle);

            xdir = speed * x / distance;
            ydir = speed * y / distance;
        }
    }
    return make_pair(x()+xdir, y()+ydir);
}

bool Missile::hit(QGraphicsScene *scene)
{
    if (life <= 0 || power <= 0)
        return false;
    life--;
    power--;
    if (life <= 0 || power <= 0) //生命或者威力在此次hit之后耗尽
    {
        delScreen(scene);
        return false;
    }
    else
        return true;
}
