#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>

#include <ctime>
#include <vector>
#include <QDebug>
#include <cassert>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <QTableWidget>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStandardItemModel>
#include <windows.h>

#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkInterface>

#include "plane.h"
#include "myplane.h"
#include "enemyplane.h"
#include "lifesupply.h"
#include "bullet.h"
#include "account.h"
#include "skillprop.h"
#include "missile.h"
#include "dbhelper.h"

using namespace std;

class QUdpSocket;

enum MessageType{Message, NewUser, Ready};

class Control : public QGraphicsScene
{
    Q_OBJECT
public:
    Control(Account *account);

protected:
    void timerEvent(QTimerEvent *event);    //定时器事件
    void keyPressEvent(QKeyEvent *event);   //按下按键事件
    void keyReleaseEvent(QKeyEvent *event); //释放按键事件

private:
    //登录账户
    Account *account;

    //联机配置
    QUdpSocket *udpSocket;
    qint16 port;
    bool myReady;
    bool otherReady;

    //图元位置
    const static int barHeight;
    const static QPointF welcomeTextPos;
    const static QPointF scoreTextPos;
    const static QPointF lifeTextPos;
    const static QPointF lifeBarPos;
    const static QPointF progressTextPos;
    const static QPointF progressBarPos;
    const static QPointF gameLostTextPos;
    const static QPointF gameWinTextPos;
    const static QPointF skillKTextPos;
    const static QPointF skillLTextPos;

    //我方飞机参数
    string myPlaneImageFile;
    int myLife;        //生命值，默认50
    int myMoveSpeed;   //移速，默认30
    int myShootSpeed;  //射速，默认300

    //我方子弹参数
    string myBulletImageFile;
    string myGreatBulletImageFile;
    int myBulletType;
    int myBulletPower;
    int myGreatBulletPower;

    //敌机参数
    string enemyPlaneImageFile;
    int enemyLife;

    //敌机子弹参数
    string enemyBulletImageFile;
    int enemyBulletPower;

    //boss参数
    string bossImageFile;
    int bossLife;

    //boss子弹参数
    string bossBulletImageFile;
    int bossBulletPower;

    //bigboss参数
    string bigBossImageFile;
    int bigBossLife;

    //导弹参数
    string missileImageFile;
    int missileLife;
    int missilePower;

    //补给参数
    string lifeSupplyImageFile;
    int supplyAddLife;

    //技能包参数
    string skillProp1ImageFile;
    string skillProp2ImageFile;
    string skillProp3ImageFile;
    string skillProp4ImageFile;

    //背景音乐
    QMediaPlayer *player;
    QMediaPlaylist *playList;

    //飞行物
    MyPlane *myplane;                   //我方飞机
    vector<Bullet*> mybullets;          //子弹
    vector<EnemyPlane *> enemyplanes;   //敌机
    vector<Bullet*> enemybullets;       //敌机子弹
    vector<Missile*> missiles;          //导弹
    vector<LifeSupply*> lifesupplys;    //生命供应
    vector<SkillProp*> skillprops;      //技能道具

    //游戏进度
    int gameProgressItv;

    //定时器
    int gameProgressId;
    int myBulletShootTimerId;
    int enemyBulletShootTimerId;
    int allBulletMoveTimerId;
    int enemyPlaneMoveTimerId;
    int lifeSupplyMoveTimerId;
    int skillPropMoveTimerId;
    int skillKeepTimerId;

    int enemyPlaneGenerateTimerId;
    int bossGenerateTimeId;
    int bigBossGenerateTimeId;

    //生命值
    QGraphicsRectItem *lifeFrameBar;
    QGraphicsRectItem *lifeBar;

    //进度条
    QGraphicsRectItem *progressFrameBar;
    QGraphicsRectItem *progressBar;

    QGraphicsTextItem *welcomeText;      //欢迎
    QGraphicsTextItem *warOfPlaneText;   //标题
    QGraphicsTextItem *scoreText;        //得分
    QGraphicsTextItem *lifeText;         //生命值
    QGraphicsTextItem *progressText;     //进度
    QGraphicsTextItem *choosePlaneText;  //选择战机
    QGraphicsTextItem *gameLostText;     //游戏失败
    QGraphicsTextItem *gameWinText;      //游戏通过

    QGraphicsTextItem *skillKText;       //技能K提示
    QGraphicsTextItem *skillLText;       //技能L提示

    int level;         //游戏关卡
    bool hasStarted;   //是否开始游戏
    bool hasShooted;   //是否开始射击
    bool isOnlineGame; //是否为联机游戏
    bool isPause;      //游戏是否暂停
    int score;         //得分
    bool godModel;     //上帝模式

    //移动飞机
    int myPlaneWMoveTimerId;
    int myPlaneSMoveTimerId;
    int myPlaneAMoveTimerId;
    int myPlaneDMoveTimerId;
    QPointF myPlaneWMove;
    QPointF myPlaneSMove;
    QPointF myPlaneAMove;
    QPointF myPlaneDMove;

    //按钮
    QGraphicsWidget *registerGameButton;    //注册游戏
    QGraphicsWidget *loginGameButton;       //登录游戏
    QGraphicsWidget *enterGameButton;       //进入游戏
    QGraphicsWidget *startGameButton;       //开始游戏
    QGraphicsWidget *onlineGameButton;      //联机游戏
    QGraphicsWidget *checkRankGameButton;   //查看排名
    QGraphicsWidget *helpGameButton;        //查看帮助
    QGraphicsWidget *continueGameButton;    //继续游戏
    QGraphicsWidget *retryGameButton;       //重新开始游戏
    QGraphicsWidget *nextLevelButton;       //下一关
    QGraphicsWidget *returnGameOverButton;  //返回结束页面
    QGraphicsWidget *returnHallButton;      //返回大厅
    QGraphicsWidget *exitGameButton;        //退出游戏

    //选择关卡
    QGraphicsWidget *chooseLevelOneButton;
    QGraphicsWidget *chooseLevelTwoButton;
    QGraphicsWidget *chooseLevelThreeButton;
    QGraphicsWidget *chooseLevelFourButton;

    //选择战机
    QGraphicsWidget *chooseBalancePlaneButton;
    QGraphicsWidget *chooseMoveSpeedPlaneButton;
    QGraphicsWidget *chooseShootSpeedPlaneButton;
    QGraphicsWidget *chooseLifePlaneButton;

    QGraphicsWidget *maskWidget; //遮罩面板

    //分数排名面板
    QGraphicsWidget *scoreTableWidget;
    QTableView *scoreTable;
    //玩家面板
    QGraphicsWidget *usersTableWidget;

    //全局定时器开关
    bool startALLTimer();
    bool killAllTimer();

    //更新分数
    void updateScore(int var);
    //更新血条
    void updateLifeBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var);
    //更新进度
    void updateProgressBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var);

    bool changeEnemyPlanePosition(EnemyPlane *enemyplane, int newX, int newY);  //改变敌机飞机位置
    bool changeMyPlanePosition(MyPlane *myplane, int newX, int newY);           //改变我方飞机位置
    bool changeBulletPosition(Bullet *bullet, int newX, int newY);              //改变子弹位置
    bool changeMissilePosition(Missile *missile, int newX, int newY);           //改变子弹位置
    bool changeLifeSupplyPosition(LifeSupply *lifesupply, int newX, int newY);  //改变补给位置
    bool changeSkillPropPosition(SkillProp *skillprop, int newX, int newY);     //改变道具位置

    void skillOver();           //技能结束
    void shootBullet();         //玩家飞机发射子弹
    void updateMyBullets();     //更新玩家所有子弹位置

    void updateLifeSupply();    //更新补给位置
    void updateSkillProp();     //更新道具位置

    bool generateEnemyPlane();  //随机生成一架敌机
    bool generateBoss();        //生成Boss
    bool generateBigBoss();     //生成大Boss
    bool generateLifeSupply(int x, int y);  //生成生命包
    bool generateSkillProp(int x, int y);  //生成道具

    bool updateEnemyPlanes();   //根据敌机方向更新敌机位置

    void shootEnemyBullets();   //所有敌机发射子弹
    void updateEnemyBullets();  //更新敌机所有子弹位置
    void updateMissiles();      //更新导弹位置

    void welcomeGame(); //初始界面
    void gameOver();    //游戏结束

    void useSkill(int attribute); //使用技能

    //联机游戏
    QString getMyIp(); //获取我的ID
    void userAddNew(QString userName,QString localHostName,QString ipAddress); //加入新用户
    void userLeft(QString userName,QString localHostName, QString time); //用户离开
    void sendMessage(MessageType type,QString serverAddress=""); //发送消息

protected slots:
    void enterGame();             //进入游戏
    void startLevelOne();         //进入关卡一
    void startLevelTwo();         //进入关卡二
    void startLevelThree();       //进入关卡三
    void startLevelFour();        //进入关卡四
    void chooseBalancePlane();    //选择均衡战机
    void chooseMoveSpeedPlane();  //选择移速战机
    void chooseShootSpeedPlane(); //选择攻速战机
    void chooseLifePlane();       //选择生命战机
    void startGame();             //开始游戏
    void onlineGame();            //联机游戏
    void showTheRank();           //显示帮助信息
    void showHelpMessage();       //显示帮助信息
    void pauseGame();             //暂停游戏
    void continueGame();          //继续游戏
    void retryGame();             //重新开始
    void nextLevelGame();             //下一关
    void returnToGameOver();      //返回游戏结束
    void returnHallGame();        //返回大厅
    void loseGame();              //玩家生命值用尽
    void exitGame();              //退出游戏
    void pendingDatagrams();      //等待其他玩家信号
};

#endif // CONTROL_H
