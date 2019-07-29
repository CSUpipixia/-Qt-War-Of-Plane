#include "control.h"

const int Control::barHeight = 20;
const QPointF Control::welcomeTextPos = QPointF(600, 800);
const QPointF Control::scoreTextPos = QPointF(50, 20);
const QPointF Control::lifeTextPos = QPointF(1100,18);
const QPointF Control::lifeBarPos = QPointF(1200,31);
const QPointF Control::progressTextPos = QPointF(350,18);
const QPointF Control::progressBarPos = QPointF(450,31);
const QPointF Control::gameLostTextPos = QPointF(300, 150);
const QPointF Control::gameWinTextPos = QPointF(320, 150);

const QPointF Control::skillKTextPos = QPointF(50, 60);
const QPointF Control::skillLTextPos = QPointF(50, 85);

//const int gameProgressItv = 100;

const int enemyBulletShootTimerItv = 2000;

const int allBulletMoveTimerItv = 10;
const int lifeSupplyMoveTimerItv = 10;
const int skillPropMoveTimerItv = 10;
const int skillKeepTimerItv = 5000;

const int enemyPlaneMoveTimerItv = 50;
const int enemyPlaneGenerateTimerItv = 3000;
const int bossGenerateTimeItv = 6000;
const int bigBossGenerateTimeItv = 8000;

Control::Control(Account *account)
{
    setSceneRect(0,0,1500,900);

    //初始化账户
    this->account = account;

    //游戏状态初始化
    isPause = false;
    hasStarted = false;
    hasShooted = false;
    godModel = false;

    //联机配置
    udpSocket = new QUdpSocket(this);
    port = 23333;
    myReady = false;
    otherReady = false;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(pendingDatagrams()));
    sendMessage(NewUser);

    //我方飞机初始化
    this->myLife = 50;
    this->myMoveSpeed = 30;
    this->myShootSpeed = 300;

    // 我方子弹初始化
    QString pixmap_mybullet_url = ":/new/prefix1/images/mybullet.png";
    QImage mybullet_image(pixmap_mybullet_url);
    if(mybullet_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        mybullet_image.save(pixmap_mybullet_url,"PNG");
    }
    this->myBulletImageFile = ":/new/prefix1/images/mybullet.png";

    QString pixmap_mygreatbullet_url = ":/new/prefix1/images/mygreatbullet.png";
    QImage mygreatbullet_image(pixmap_mygreatbullet_url);
    if(mygreatbullet_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        mygreatbullet_image.save(pixmap_mygreatbullet_url,"PNG");
    }
    this->myGreatBulletImageFile = ":/new/prefix1/images/mygreatbullet.png";
    this->myBulletPower = 1;
    this->myGreatBulletPower = 2;

    //敌方飞机初始化
    QString pixmap_enemyplane_url = ":/new/prefix1/images/enemyplane.png";
    QImage enemyplane_image(pixmap_enemyplane_url);
    if(enemyplane_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        enemyplane_image.save(pixmap_enemyplane_url,"PNG");
    }
    this->enemyPlaneImageFile = ":/new/prefix1/images/enemyplane.png";
    this->enemyLife = 1;

    //敌方子弹初始化
    QString pixmap_enemybullet_url = ":/new/prefix1/images/enemybullet.png";
    QImage enemybullet_image(pixmap_enemybullet_url);
    if(enemybullet_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        enemybullet_image.save(pixmap_enemybullet_url,"PNG");
    }
    this->enemyBulletImageFile = ":/new/prefix1/images/enemybullet.png";
    this->enemyBulletPower = 1;

    //敌方boss初始化
    QString pixmap_boss_url = ":/new/prefix1/images/boss.png";
    QImage boss_image(pixmap_boss_url);
    if(boss_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        boss_image.save(pixmap_boss_url,"PNG");
    }
    this->bossImageFile = ":/new/prefix1/images/boss.png";
    this->bossLife = 5;

    //敌方boss子弹初始化
    QString pixmap_bossbullet_url = ":/new/prefix1/images/bossbullet.png";
    QImage bossbullet_image(pixmap_bossbullet_url);
    if(bossbullet_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        bossbullet_image.save(pixmap_bossbullet_url,"PNG");
    }
    this->bossBulletImageFile = ":/new/prefix1/images/bossbullet.png";
    this->bossBulletPower = 2;

    //敌方bigboss初始化
    QString pixmap_bigboss_url = ":/new/prefix1/images/bigboss.png";
    QImage bigboss_image(pixmap_bigboss_url);
    if(bigboss_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        bigboss_image.save(pixmap_bigboss_url,"PNG");
    }
    this->bigBossImageFile = ":/new/prefix1/images/bigboss.png";
    this->bigBossLife = 10;

    //导弹初始化
    QString pixmap_missile_url = ":/new/prefix1/images/missile.png";
    QImage missile_image(pixmap_missile_url);
    if(missile_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        missile_image.save(pixmap_missile_url,"PNG");
    }
    this->missileImageFile = ":/new/prefix1/images/missile.png";
    this->missileLife = 1;
    this->missilePower = 1;

    //补给初始化
    QString pixmap_lifesupply_url = ":/new/prefix1/images/lifesupply.png";
    QImage lifesupply_image(pixmap_lifesupply_url);
    if(lifesupply_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        lifesupply_image.save(pixmap_lifesupply_url,"PNG");
    }
    this->lifeSupplyImageFile = ":/new/prefix1/images/lifesupply.png";
    this->supplyAddLife = 5;

    //技能包初始化
    QString pixmap_skillprop1_url = ":/new/prefix1/images/skillprop1.png";
    QImage skillprop1_image(pixmap_skillprop1_url);
    if(skillprop1_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        skillprop1_image.save(pixmap_skillprop1_url,"PNG");
    }
    this->skillProp1ImageFile = ":/new/prefix1/images/skillprop1.png";

    QString pixmap_skillprop2_url = ":/new/prefix1/images/skillprop2.png";
    QImage skillprop2_image(pixmap_skillprop2_url);
    if(skillprop2_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        skillprop2_image.save(pixmap_skillprop2_url,"PNG");
    }
    this->skillProp2ImageFile = ":/new/prefix1/images/skillprop2.png";

    QString pixmap_skillprop3_url = ":/new/prefix1/images/skillprop3.png";
    QImage skillprop3_image(pixmap_skillprop3_url);
    if(skillprop3_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        skillprop3_image.save(pixmap_skillprop3_url,"PNG");
    }
    this->skillProp3ImageFile = ":/new/prefix1/images/skillprop3.png";

    QString pixmap_skillprop4_url = ":/new/prefix1/images/skillprop4.png";
    QImage skillprop4_image(pixmap_skillprop4_url);
    if(skillprop4_image.isNull()){
        qDebug()<<"读取图片错误";
    }
    else{
        skillprop4_image.save(pixmap_skillprop4_url,"PNG");
    }
    this->skillProp4ImageFile = ":/new/prefix1/images/skillprop4.png";

    //初始化背景音乐
    playList = new QMediaPlaylist;
    playList->addMedia(QUrl("qrc:///music/victory.mp3"));
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); //单曲循环
    player = new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->play();

    // 遮罩面板
    QWidget *mask = new QWidget;
    mask->setPalette(QColor(0,0,0,80));
    mask->resize(width(),height());
    maskWidget = addWidget(mask);
    maskWidget->setPos(0,0);
    maskWidget->setZValue(1);
    maskWidget->hide();

    // 欢迎
    welcomeText = new QGraphicsTextItem;
    welcomeText->setPos(welcomeTextPos);
    welcomeText->setHtml(tr("<font color=white>welcome, %1!</font>").arg(account->getUsername()));
    welcomeText->setScale(2);
    welcomeText->setFont(QFont("Courier"));
    addItem(welcomeText);
    welcomeText->hide();

    // 标题
    warOfPlaneText = new QGraphicsTextItem;
    warOfPlaneText->setPos(QPointF(250, 70));
    warOfPlaneText->setHtml(tr("<font color=white>WAR OF PLANE</font>"));
    warOfPlaneText->setScale(7);
    warOfPlaneText->setFont(QFont("Algerian"));
    addItem(warOfPlaneText);
    warOfPlaneText->hide();

    // 生命值
    lifeText = new QGraphicsTextItem;
    lifeText->setPos(lifeTextPos);
    lifeText->setHtml(tr("<font color=white>生命: </font>"));
    lifeText->setScale(1.5);
    lifeText->setFont(QFont("Courier"));
    addItem(lifeText);

    // 进度值
    progressText = new QGraphicsTextItem;
    progressText->setPos(progressTextPos);
    progressText->setHtml(tr("<font color=white>进度: </font>"));
    progressText->setScale(1.5);
    progressText->setFont(QFont("Courier"));
    addItem(progressText);

    // 得分提示
    score = 0;
    scoreText = new QGraphicsTextItem;
    scoreText->setPos(scoreTextPos);
    scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
    scoreText->setScale(1.5);
    scoreText->setFont(QFont("Courier"));
    addItem(scoreText);

    // 选择战机提示
    choosePlaneText = new QGraphicsTextItem;
    choosePlaneText->setPos(QPointF(550, 10));
    choosePlaneText->setHtml(tr("<font color=white>Choose Your Plane</font>"));
    choosePlaneText->setScale(2);
    choosePlaneText->setFont(QFont("Algerian"));
    addItem(choosePlaneText);
    choosePlaneText->hide();

    // 游戏失败提示
    gameLostText = new QGraphicsTextItem;
    gameLostText->setPos(gameLostTextPos);
    gameLostText->setHtml(tr("<font color=white>Game Lost, Your Score: %1</font>").arg(score));
    gameLostText->setScale(3);
    gameLostText->setFont(QFont("Courier"));
    gameLostText->setZValue(2);
    addItem(gameLostText);
    gameLostText->hide();

    //游戏通过提示
    gameWinText = new QGraphicsTextItem;
    gameWinText->setPos(gameLostTextPos);
    gameWinText->setHtml(tr("<font color=white>You Win, Your Score: %1</font>").arg(score));
    gameWinText->setScale(3);
    gameWinText->setFont(QFont("Courier"));
    gameWinText->setZValue(2);
    addItem(gameWinText);
    gameWinText->hide();

    //技能3：K提示
    skillKText = new QGraphicsTextItem;
    skillKText->setPos(skillKTextPos);
    skillKText->setHtml("<font color=white>K: 清除所有敌机</font>");
    skillKText->setFont(QFont("Courier"));
    addItem(skillKText);
    //技能4：L提示
    skillLText = new QGraphicsTextItem;
    skillLText->setPos(skillLTextPos);
    skillLText->setHtml("<font color=white>L: 清除所有子弹</font>");
    skillLText->setFont(QFont("Courier"));
    addItem(skillLText);

    skillKText->hide();
    skillLText->hide();

    // 生命条
    lifeFrameBar = new QGraphicsRectItem(lifeBarPos.x(), lifeBarPos.y(), myLife*4, barHeight);
    lifeFrameBar->setPen(QPen(Qt::white));
    addItem(lifeFrameBar);

    lifeBar = new QGraphicsRectItem(lifeBarPos.x(), lifeBarPos.y(), myLife*4, barHeight);
    lifeBar->setBrush(QBrush(Qt::green));
    addItem(lifeBar);

    // 进度条
    progressFrameBar = new QGraphicsRectItem(progressBarPos.x(),progressBarPos.y(), 600, barHeight);
    progressFrameBar->setPen(QPen(Qt::white));
    addItem(progressFrameBar);

    progressBar = new QGraphicsRectItem(progressBarPos.x(), progressBarPos.y(), 600, barHeight);
    progressBar->setBrush(QBrush(Qt::blue));
    addItem(progressBar);

    lifeText->hide();
    progressText->hide();
    scoreText->hide();
    lifeFrameBar->hide();
    lifeBar->hide();
    progressFrameBar->hide();
    progressBar->hide();

    /* 所有按钮初始化 */
    // 进入游戏按钮
    QPushButton *enterGameBtm = new QPushButton("Enter Game");
    enterGameBtm->setFont(QFont("Algerian",22));
    enterGameBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(enterGameBtm, SIGNAL(clicked()),this,SLOT(enterGame()));
    enterGameButton = addWidget(enterGameBtm);
    enterGameButton->setPos(width()/2 - 100, height()/2 - 100);
    enterGameButton->setZValue(2);
    enterGameButton->hide();

    // 开始游戏按钮
    QPushButton *startGameBtm = new QPushButton("Start Game");
    startGameBtm->setFont(QFont("Algerian",22));
    startGameBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(startGameBtm, SIGNAL(clicked()),this,SLOT(startGame()));
    startGameButton = addWidget(startGameBtm);
    startGameButton->setPos(width()/2 - 100, height()/2 - 100);
    startGameButton->setZValue(2);
    startGameButton->hide();

    // 联机游戏按钮
    QPushButton *onlineGameBtn = new QPushButton("Online Game");
    onlineGameBtn->setFont(QFont("Algerian",22));
    onlineGameBtn->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(onlineGameBtn, SIGNAL(clicked()),this,SLOT(onlineGame()));
    onlineGameButton = addWidget(onlineGameBtn);
    onlineGameButton->setPos(width()/2 - 105, height()/2);
    onlineGameButton->setZValue(2);
    onlineGameButton->setEnabled(false);
    onlineGameButton->hide();

    //选择关卡一
    QPushButton *chooseLevelOneBtm = new QPushButton("Level One");
    chooseLevelOneBtm->setFont(QFont("Algerian",22));
    chooseLevelOneBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(chooseLevelOneBtm, SIGNAL(clicked()),this,SLOT(startLevelOne()));
    chooseLevelOneButton = addWidget(chooseLevelOneBtm);
    chooseLevelOneButton->setPos(width()/2 - 80, height()/2 - 100);
    chooseLevelOneButton->setZValue(2);
    chooseLevelOneButton->hide();

    //选择关卡二
    QPushButton *chooseLevelTwoBtm = new QPushButton("Level Two");
    chooseLevelTwoBtm->setFont(QFont("Algerian",22));
    chooseLevelTwoBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(chooseLevelTwoBtm, SIGNAL(clicked()),this,SLOT(startLevelTwo()));
    chooseLevelTwoButton = addWidget(chooseLevelTwoBtm);
    chooseLevelTwoButton->setPos(width()/2 - 80, height()/2);
    chooseLevelTwoButton->setZValue(2);
    chooseLevelTwoButton->hide();

    //选择关卡三
    QPushButton *chooseLevelThreeBtm = new QPushButton("Level Three");
    chooseLevelThreeBtm->setFont(QFont("Algerian",22));
    chooseLevelThreeBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(chooseLevelThreeBtm, SIGNAL(clicked()),this,SLOT(startLevelThree()));
    chooseLevelThreeButton = addWidget(chooseLevelThreeBtm);
    chooseLevelThreeButton->setPos(width()/2 - 100, height()/2 + 100);
    chooseLevelThreeButton->setZValue(2);
    chooseLevelThreeButton->hide();

    //选择关卡四
    QPushButton *chooseLevelFourBtm = new QPushButton("Level Four");
    chooseLevelFourBtm->setFont(QFont("Algerian",22));
    chooseLevelFourBtm->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(chooseLevelFourBtm, SIGNAL(clicked()),this,SLOT(startLevelFour()));
    chooseLevelFourButton = addWidget(chooseLevelFourBtm);
    chooseLevelFourButton->setPos(width()/2 - 92, height()/2 + 200);
    chooseLevelFourButton->setZValue(2);
    chooseLevelFourButton->hide();

    //下一关
    QPushButton *nextLevelBtn = new QPushButton("Next Level");
    nextLevelBtn->setFont(QFont("Algerian",22));
    nextLevelBtn->setStyleSheet("QPushButton{background: transparent; color: white;}"
                                "QPushButton:hover{color: red;}");
    connect(nextLevelBtn, SIGNAL(clicked()),this,SLOT(nextLevelGame()));
    nextLevelButton = addWidget(nextLevelBtn);
    nextLevelButton->setPos(width()/2 - 100, height()/2 - 100);
    nextLevelButton->setZValue(2);
    nextLevelButton->hide();

    //选择均衡战机
    QPixmap balancePlaneIcon;
    balancePlaneIcon.load(":/new/prefix1/images/balanceplaneicon.png");
    QToolButton *chooseBalancePlaneBtn = new QToolButton();
    chooseBalancePlaneBtn->setFixedSize(300, 600);
    chooseBalancePlaneBtn->setIcon(balancePlaneIcon);
    chooseBalancePlaneBtn->setIconSize(QSize(300, 600));
    connect(chooseBalancePlaneBtn, SIGNAL(clicked()),this,SLOT(chooseBalancePlane()));
    chooseBalancePlaneButton = addWidget(chooseBalancePlaneBtn);
    chooseBalancePlaneButton->setPos(1125, 125);
    chooseBalancePlaneButton->setZValue(2);
    chooseBalancePlaneButton->hide();

    //选择移速战机
    QPixmap movespeedPlaneIcon;
    movespeedPlaneIcon.load(":/new/prefix1/images/movespeedplaneicon.png");
    QToolButton *chooseMoveSpeedPlaneBtn = new QToolButton();
    chooseMoveSpeedPlaneBtn->setFixedSize(300, 600);
    chooseMoveSpeedPlaneBtn->setIcon(movespeedPlaneIcon);
    chooseMoveSpeedPlaneBtn->setIconSize(QSize(300, 600));
    connect(chooseMoveSpeedPlaneBtn, SIGNAL(clicked()),this,SLOT(chooseMoveSpeedPlane()));
    chooseMoveSpeedPlaneButton = addWidget(chooseMoveSpeedPlaneBtn);
    chooseMoveSpeedPlaneButton->setPos(75, 125);
    chooseMoveSpeedPlaneButton->setZValue(2);
    chooseMoveSpeedPlaneButton->hide();

    //选择攻速战机
    QPixmap shootspeedPlaneIcon;
    shootspeedPlaneIcon.load(":/new/prefix1/images/shootspeedplaneicon.png");
    QToolButton *chooseShootSpeedPlaneBtn = new QToolButton();
    chooseShootSpeedPlaneBtn->setFixedSize(300, 600);
    chooseShootSpeedPlaneBtn->setIcon(shootspeedPlaneIcon);
    chooseShootSpeedPlaneBtn->setIconSize(QSize(300, 600));
    connect(chooseShootSpeedPlaneBtn, SIGNAL(clicked()),this,SLOT(chooseShootSpeedPlane()));
    chooseShootSpeedPlaneButton = addWidget(chooseShootSpeedPlaneBtn);
    chooseShootSpeedPlaneButton->setPos(775, 125);
    chooseShootSpeedPlaneButton->setZValue(2);
    chooseShootSpeedPlaneButton->hide();

    //选择生命战机
    QPixmap lifePlaneIcon;
    lifePlaneIcon.load(":/new/prefix1/images/lifeplaneicon.png");
    QToolButton *chooseLifePlaneBtn = new QToolButton();
    chooseLifePlaneBtn->setFixedSize(300, 600);
    chooseLifePlaneBtn->setIcon(lifePlaneIcon);
    chooseLifePlaneBtn->setIconSize(QSize(300, 600));
    connect(chooseLifePlaneBtn, SIGNAL(clicked()),this,SLOT(chooseLifePlane()));
    chooseLifePlaneButton = addWidget(chooseLifePlaneBtn);
    chooseLifePlaneButton->setPos(425, 125);
    chooseLifePlaneButton->setZValue(2);
    chooseLifePlaneButton->hide();

    // 继续游戏
    QPushButton *continueGameBtn = new QPushButton(tr("Continue Game"));
    continueGameBtn->setFont(QFont("Algerian",22));
    continueGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(continueGameBtn,SIGNAL(clicked()),this,SLOT(continueGame()));
    continueGameButton = addWidget(continueGameBtn);
    continueGameButton->setPos(width()/2 - 130, height()/2 - 100);
    continueGameButton->setZValue(2);
    continueGameButton->hide();

    // 重新开始游戏
    QPushButton *retryGameBtn = new QPushButton(tr("Retry Game"));
    retryGameBtn->setFont(QFont("Algerian",22));
    retryGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(retryGameBtn,SIGNAL(clicked()),this,SLOT(retryGame()));
    retryGameButton = addWidget(retryGameBtn);
    retryGameButton->setPos(width()/2 - 108, height()/2);
    retryGameButton->setZValue(2);
    retryGameButton->hide();

    // 返回大厅
    QPushButton *returnHallBtn = new QPushButton(tr("Return"));
    returnHallBtn->setFont(QFont("Algerian",22));
    returnHallBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(returnHallBtn,SIGNAL(clicked()),this,SLOT(returnHallGame()));
    returnHallButton = addWidget(returnHallBtn);
    returnHallButton->setPos(width()/2 - 75, height()/2 + 200);
    returnHallButton->setZValue(2);
    returnHallButton->hide();

    // 返回游戏结束页面
    QPushButton *returnGameOverBtn = new QPushButton(tr("Return"));
    returnGameOverBtn->setFont(QFont("Algerian",22));
    returnGameOverBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(returnGameOverBtn,SIGNAL(clicked()),this,SLOT(returnToGameOver()));
    returnGameOverButton = addWidget(returnGameOverBtn);
    returnGameOverButton->setPos(width()/2 - 75, height() - 100);
    returnGameOverButton->setZValue(2);
    returnGameOverButton->hide();

    // 游戏帮助
    QPushButton *helpGameBtn = new QPushButton(tr("Game Help"));
    helpGameBtn->setFont(QFont("Algerian",22));
    helpGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(helpGameBtn,SIGNAL(clicked()),this,SLOT(showHelpMessage()));
    helpGameButton = addWidget(helpGameBtn);
    helpGameButton->setPos(width()/2 - 92, height()/2 + 100);
    helpGameButton->setZValue(2);
    helpGameButton->hide();

    // 查看排名
    QPushButton *checkRankGameBtn = new QPushButton(tr("Check Rank"));
    checkRankGameBtn->setFont(QFont("Algerian",22));
    checkRankGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(checkRankGameBtn,SIGNAL(clicked()),this,SLOT(showTheRank()));
    checkRankGameButton = addWidget(checkRankGameBtn);
    checkRankGameButton->setPos(width()/2 - 105, height()/2 + 100);
    checkRankGameButton->setZValue(2);
    checkRankGameButton->hide();

    // 退出游戏
    QPushButton *exitGameBtn = new QPushButton(tr("Exit Game"));
    exitGameBtn->setFont(QFont("Algerian",22));
    exitGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
                               "QPushButton:hover{color:red;}");
    connect(exitGameBtn,SIGNAL(clicked()),this,SLOT(exitGame()));
    exitGameButton = addWidget(exitGameBtn);
    exitGameButton->setPos(width()/2 - 90, height()/2 + 200);
    exitGameButton->setZValue(2);
    helpGameButton->hide();

    //分数面板
    scoreTable = new QTableView();

    scoreTable->setGeometry(250, 175, 1000, 550);
    scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止修改
    scoreTable->horizontalHeader()->setDefaultSectionSize(250); //列宽250
    scoreTable->verticalHeader()->setDefaultSectionSize(50); //行高50
    scoreTable->horizontalHeader()->hide(); //隐藏列头
    scoreTable->verticalHeader()->hide(); //隐藏行头
    scoreTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //无水平滚动条
    scoreTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //无垂直滚动条
    scoreTable->setStyleSheet("background-color:rgba(0,0,0,0)");
    scoreTable->setShowGrid(false); //设置网格线不可见

    scoreTableWidget = addWidget(scoreTable);
    scoreTableWidget->setZValue(2);
    scoreTableWidget->hide();

    //玩家面板
    QTableWidget *usersTable = new QTableWidget(10,5); //10行5列
    usersTable->setGeometry(200, 200, 1100, 500);
    usersTableWidget = addWidget(usersTable);
    usersTableWidget->hide();

    welcomeGame();
}

//启动所有定时器
bool Control::startALLTimer()
{
    gameProgressId = startTimer(gameProgressItv);
    enemyBulletShootTimerId = startTimer(enemyBulletShootTimerItv);
    allBulletMoveTimerId = startTimer(allBulletMoveTimerItv);
    enemyPlaneMoveTimerId = startTimer(enemyPlaneMoveTimerItv);
    enemyPlaneGenerateTimerId = startTimer(enemyPlaneGenerateTimerItv);
    if (level >= 2)
    {
        lifeSupplyMoveTimerId = startTimer(lifeSupplyMoveTimerItv);
        if (level >= 3)
        {
            bossGenerateTimeId = startTimer(bossGenerateTimeItv);
            skillPropMoveTimerId = startTimer(skillPropMoveTimerItv);
            if (level >= 4)
            {
                bigBossGenerateTimeId = startTimer(bigBossGenerateTimeItv);
            }
        }
    }

    return true;
}

//关闭全局定时器
bool Control::killAllTimer()
{
    killTimer(gameProgressId);
    killTimer(myBulletShootTimerId);
    killTimer(enemyBulletShootTimerId);
    killTimer(allBulletMoveTimerId);
    killTimer(enemyPlaneMoveTimerId);
    killTimer(enemyPlaneGenerateTimerId);
    if (level >= 2)
    {
        killTimer(lifeSupplyMoveTimerId);
        if (level >= 3)
        {
            killTimer(bossGenerateTimeId);
            killTimer(skillPropMoveTimerId);
            if (level >= 4)
            {
                killTimer(bigBossGenerateTimeId);
            }
        }
    }

    return true;
}

//定时器事件
void Control::timerEvent(QTimerEvent *event)
{
    if (hasStarted && !isPause)
    {
        if(event->timerId()==myBulletShootTimerId && hasShooted) //发射子弹
            shootBullet();
        if(event->timerId()==myPlaneWMoveTimerId || event->timerId()==myPlaneSMoveTimerId
                || event->timerId()==myPlaneAMoveTimerId || event->timerId()==myPlaneDMoveTimerId)
        {
            if(event->timerId()==myPlaneWMoveTimerId)
                changeMyPlanePosition(myplane, myplane->x()+myPlaneWMove.x(), myplane->y()+myPlaneWMove.y());
            if(event->timerId()==myPlaneSMoveTimerId)
                changeMyPlanePosition(myplane, myplane->x()+myPlaneSMove.x(), myplane->y()+myPlaneSMove.y());
            if(event->timerId()==myPlaneAMoveTimerId)
                changeMyPlanePosition(myplane, myplane->x()+myPlaneAMove.x(), myplane->y()+myPlaneAMove.y());
            if(event->timerId()==myPlaneDMoveTimerId)
                changeMyPlanePosition(myplane, myplane->x()+myPlaneDMove.x(), myplane->y()+myPlaneDMove.y());
        }
        else if(event->timerId()==enemyPlaneGenerateTimerId) //随机生成敌机
        {
            for(int i=0;i<2;i++)
                generateEnemyPlane();
        }
        else if(event->timerId()==enemyBulletShootTimerId)   //敌机发射子弹
            shootEnemyBullets();
        else if(event->timerId()==allBulletMoveTimerId)      //移动子弹
        {
            updateMyBullets();
            updateEnemyBullets();
            updateMissiles();
        }
        else if(event->timerId()==enemyPlaneMoveTimerId)     //更新敌机位置
            updateEnemyPlanes();
        else if (level >= 2)
        {
            if(event->timerId()==lifeSupplyMoveTimerId)     //更新补给位置
                updateLifeSupply();
            else if (level >= 3)
            {
                if(event->timerId()==bossGenerateTimeId)       //随机生成boss
                    generateBoss();
                else if(event->timerId()==skillPropMoveTimerId)     //更新道具位置
                    updateSkillProp();
                else if(event->timerId()==skillKeepTimerId)     //技能持续时间
                    skillOver();
                else if(level >= 4)
                    if (event->timerId()==bigBossGenerateTimeId)
                        generateBigBoss();
            }
        }

        if (event->timerId()==gameProgressId)
            updateProgressBar(progressBar, progressFrameBar, progressBarPos, 1);
    }
}

//按下按钮事件
void Control::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat() && hasStarted)
    {
        if (!isPause)
        {
            if (event->key() == Qt::Key_J)
            {
                if (!hasShooted)
                    myBulletShootTimerId = startTimer(myplane->shootspeed);
                hasShooted = true;
            }
            //方向键
            if (event->key() == Qt::Key_W)
            {
                if(myPlaneWMove==QPointF(0,0))
                    myPlaneWMoveTimerId = startTimer(myplane->movespeed);
                myPlaneWMove = QPointF(0, -10);
            }
            if (event->key() == Qt::Key_S)
            {
                if(myPlaneSMove==QPointF(0,0))
                    myPlaneSMoveTimerId = startTimer(myplane->movespeed);
                myPlaneSMove = QPointF(0, 10);
            }
            if (event->key() == Qt::Key_A)
            {
                if(myPlaneAMove==QPointF(0,0))
                    myPlaneAMoveTimerId = startTimer(myplane->movespeed);
                myPlaneAMove = QPointF(-10, 0);
            }
            if (event->key() == Qt::Key_D)
            {
                if(myPlaneDMove==QPointF(0,0))
                    myPlaneDMoveTimerId = startTimer(myplane->movespeed);
                myPlaneDMove = QPointF(10, 0);
            }
            //主动技能
            if (event->key() == Qt::Key_K)
            {
                useSkill(3);
            }
            if (event->key() == Qt::Key_L)
            {
                useSkill(4);
            }
            //上帝模式
            if (event->key() == Qt::Key_R && account->getUsername() == "xue")
            {
                godModel = godModel?false:true;
            }
        }
        if(event->key()==Qt::Key_Space)
            pauseGame();
    }
}

//释放按钮事件
void Control::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat() && hasStarted)
    {
        if(event->key() == Qt::Key_W)
        {
            myPlaneWMove = QPointF(0, 0);
            killTimer(myPlaneWMoveTimerId);
        }
        if (event->key() == Qt::Key_S)
        {
            myPlaneSMove = QPointF(0, 0);
            killTimer(myPlaneSMoveTimerId);
        }
        if (event->key() == Qt::Key_A)
        {
            myPlaneAMove = QPointF(0, 0);
            killTimer(myPlaneAMoveTimerId);
        }
        if (event->key() == Qt::Key_D)
        {
            myPlaneDMove = QPointF(0, 0);
            killTimer(myPlaneDMoveTimerId);
        }
        if (event->key() == Qt::Key_J)
        {
            hasShooted = false;
            killTimer(myBulletShootTimerId);
        }
    }
}

//改变我方飞机位置
bool Control::changeMyPlanePosition(MyPlane *myplane, int newX, int newY)
{
    //位置不发生改变直接返回true
    if(myplane->x() ==newX && myplane->y() == newY)
        return true;

    //边界检查
    if(newX<0 || newX>width() - 90 || newY<0 || newY>height() - 90)
    {
         return true;
    }

    //与补给碰撞
    if(level >= 2)
    {
        for(vector<LifeSupply*>::iterator it = lifesupplys.begin(); it!=lifesupplys.end(); )
        {
            if(myplane->collidesWithItem(*it))
            {
                int addLife = min(myplane->life+supplyAddLife, myLife) - myplane->life;
                myplane->crash(this, addLife);
                updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, +addLife);
                removeItem(*it);
                delete *it;
                it = lifesupplys.erase(it);
            }
            else
                it++;
        }
    }

    //与道具相撞
    if(level >= 3)
    {
        for(vector<SkillProp*>::iterator it = skillprops.begin(); it!=skillprops.end(); )
        {
            if(myplane->collidesWithItem(*it))
            {
                myplane->getSkill((*it)->attribute);
                removeItem(*it);
                delete *it;
                it = skillprops.erase(it);
                skillKeepTimerId = startTimer(skillKeepTimerItv);
            }
            else
                it++;
        }
    }

    //与飞机相撞
    for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
    {
        bool alive = true; //检查it是否存活
        if (myplane->collidesWithItem(*it))
        {

            if ((*it)->type == ORD)
            {
                if(!godModel)
                {
                    alive = (*it)->crash(this, -1);
                    updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -1);
                }
            }
            else if ((*it)->type == BOSS)
            {
                if(!godModel)
                {
                    alive = (*it)->crash(this, -2);
                    updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -2);
                }
            }
        }

        if (alive)
            it++;
        else //若飞机坠毁，则将此飞机去掉
        {
            delete *it;
            it = enemyplanes.erase(it);
        }
    }

    //移动
    if(myplane->life > 0)//生命值需大于0
    {
        myplane->moveBy(newX-myplane->x(), newY-myplane->y());//移动
        myplane->update();//刷新屏幕
    }

    return myplane->life > 0;
}

//改变敌方飞机位置
bool Control::changeEnemyPlanePosition(EnemyPlane *enemyplane, int newX, int newY)
{
    //位置不发生改变直接返回true
    if(enemyplane->x() ==newX && enemyplane->y() == newY)
        return true;

    //左右边界检查
    if(newX<0 || newX>width() - enemyplane->pixmap().width())
    {
        enemyplane->changeXdir();//改变敌机左右方向
        return true;
    }

    //上下边界检查
    if(newY<0 || newY>height())
    {
        if (newY>height())
        {
            if(!godModel)
            {
                myplane->crash(this, -1);
                updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -1);
            }
            enemyplane->delScreen(this);//超出下界删除
            return false;
        }
        else
            return true;
    }

    //与我方飞机相撞
    if (enemyplane->collidesWithItem(myplane))
    {
        enemyplane->delScreen(this);
        if (enemyplane->type == ORD)
        {
            if(!godModel)
            {
                myplane->crash(this, -1);
                updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -1);
            }
        }
        else if (enemyplane->type == BOSS || enemyplane->type == BIGBOSS)
        {
            if(!godModel)
            {
                myplane->crash(this, -2);
                updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -2);
            }
        }

        return false;
    }

    //移动
    if(enemyplane->life > 0)//生命值需大于0
    {
        enemyplane->moveBy(newX-enemyplane->x(), newY-enemyplane->y());//移动
        enemyplane->update();//刷新屏幕
    }

    return enemyplane->life > 0;
}

//全局更新子弹位置
bool Control::changeBulletPosition(Bullet * bullet, int newX, int newY)
{
    //检查位置是否有变化
    if (bullet->x() == newX && bullet->y() == newY)
        return true;

    /* 检查子弹是否击中某一飞机 */
    // 首先检查玩家飞机
    if (bullet->part==ENEMY && bullet->collidesWithItem(myplane))
    {
        if (!godModel)
        {
            myplane->crash(this, -bullet->power);
            updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -bullet->power);
        }

        bullet->hit(this);
    }
    else if(bullet->part==ME)
    {   
        // 检查敌机：若敌机已经没有生命值，就从enemyplanes中删去
        for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
        {
            bool alive = true;
            if (bullet->collidesWithItem(*it))
            {
                alive = (*it)->crash(this, -bullet->power);
                bullet->hit(this);
                updateScore(score+=50);
            }

            if(alive)
                it++;
            else
            {
                if ((*it)->type==ORD)
                    generateLifeSupply(bullet->x(), bullet->y());
                if ((*it)->type==BOSS || (*it)->type==BIGBOSS)
                    generateSkillProp(bullet->x(), bullet->y());
                delete *it;
                it = enemyplanes.erase(it);
            }
        }
        // 检查导弹
        for (vector<Missile*>::iterator it = missiles.begin(); it != missiles.end(); )
        {
            bool alive = true;
            if (bullet->collidesWithItem(*it))
            {
                alive = (*it)->hit(this);
                bullet->hit(this);
                updateScore(score+=30);
            }

            if(alive)
                it++;
            else
            {
                delete *it;
                it = missiles.erase(it);
            }
        }
    }

    /* 若子弹还具有杀伤力则更新位置并同步屏幕 */
    if (bullet->power > 0)
    {
        /* 若此时子弹试图打出边界，则销毁子弹 */
        if (newX <= 0 || newX >= width() || newY <= 0 || newY >= height())
        {
            bullet->delScreen(this);
            return false;
        }
        bullet->synScreen(this);
        bullet->moveBy(newX-bullet->x(), newY-bullet->y());
        bullet->update();
    }

    return bullet->power>0;
}

//全局更新导弹位置
bool Control::changeMissilePosition(Missile *missile, int newX, int newY)
{
    /* 检查位置是否有变化，无变化则返回true */
    if (missile->x() == newX && missile->y() == newY)
        return true;

    //与我方飞机相撞
    if (missile->collidesWithItem(myplane))
    {
        if (!godModel)
        {
            myplane->crash(this, -missile->power);
            updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, -missile->power);
        }

        missile->hit(this);
    }

    //移动
    if(missile->life > 0)//生命值需大于0
    {
        missile->moveBy(newX-missile->x(), newY-missile->y());//移动
        missile->update();//刷新屏幕
    }

    return missile->power>0;
}

//全局更新补给位置
bool Control::changeLifeSupplyPosition(LifeSupply *lifesupply, int newX, int newY)
{
    // 检查位置是否有变化，无变化则返回true
    if (lifesupply->x() == newX && lifesupply->y() == newY)
        return true;

    //边界检查
    if(newX<0 || newX>width() || newY<0 || newY>height())
    {
        if (newY>height())
        {
            lifesupply->delScreen(this);//超出下界删除
            return false;
        }
        else
            return true;
    }

    //与我方飞机相撞
    if (lifesupply->collidesWithItem(myplane))
    {
        int addLife = min(myplane->life + supplyAddLife, myLife) - myplane->life;
        if (!godModel)
        {
            myplane->crash(this, addLife);
            updateLifeBar(lifeBar, lifeFrameBar, lifeBarPos, addLife);
        }
        lifesupply->delScreen(this);

        return false;
    }

    if(lifesupply->energy>0)
    {
        // 若此时补给试图打出边界，则销毁补给
        if (newX <= 0 || newX >= width() || newY <= 0 || newY >= height())
        {
            lifesupply->delScreen(this);
            return false;
        }
        lifesupply->synScreen(this);
        lifesupply->moveBy(newX-lifesupply->x(), newY-lifesupply->y());
        lifesupply->update();
    }

    return lifesupply->energy>0;
}

//全局更新技能道具
bool Control::changeSkillPropPosition(SkillProp *skillprop, int newX, int newY)
{
    /* 检查位置是否有变化，无变化则返回true */
    if (skillprop->x() == newX && skillprop->y() == newY)
        return true;

    //左右边界检查
    if(newX<0 || newX>width() - 50)
    {
        skillprop->changeXdir();//改变左右方向
        return true;
    }


    //上下边界检查
    if(newY<0 || newY>height())
    {
        skillprop->delScreen(this);//超出下界删除
        return false;
    }

    //与我方飞机相撞
    if (skillprop->collidesWithItem(myplane))
    {
        updateScore(score+=30);
        switch (myplane->getSkill(skillprop->attribute)) {
        case 3:
            skillKText->show();
            break;
        case 4:
            skillLText->show();
            break;
        default:
            break;
        }

        skillprop->delScreen(this);
        skillKeepTimerId = startTimer(skillKeepTimerItv);
        return false;
    }


    skillprop->synScreen(this);
    skillprop->moveBy(newX-skillprop->x(), newY-skillprop->y());
    skillprop->update();
    return true;
}

void Control::useSkill(int attribute)
{
    switch (attribute) {
    case 3: //技能3：清除所有敌机
        if (myplane->useSkill(3))
        {
            for(auto iter:enemyplanes)
            {
                removeItem(iter);
                delete iter;
            }
            enemyplanes.clear();
            skillKText->hide();
        }
        break;
    case 4: //技能4：清除所有子弹
        if (myplane->useSkill(4))
        {
            for(auto it:enemybullets)
            {
                removeItem(it);
                delete it;
            }
            enemybullets.clear();
            for(auto it:missiles)
            {
                removeItem(it);
                delete it;
            }
            missiles.clear();
            skillLText->hide();
        }
        break;
    default:
        break;
    }
}

//技能结束
void Control::skillOver()
{
    switch (myplane->bullettype) {
    case 1:
        myplane->bullettype = 0;
        killTimer(skillKeepTimerId);
        break;
    case 2:
        myplane->bullettype = 0;
        killTimer(skillKeepTimerId);
        break;
    case 3:
        myplane->bullettype = 2;
        break;
    default:
        break;
    }
}

//更新分数
void Control::updateScore(int var)
{
    scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(var));
    scoreText->setScale(1.5);
    scoreText->setFont(QFont("Courier"));
    scoreText->show();
}

//更新血量条
void Control::updateLifeBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var)
{
    var = var * 4;
    qreal wid = bar->rect().width();
    if(var<0)
    {
        wid = max((qreal)0, wid + var);
    }
    else
    {
        wid = min(frameBar->rect().width(), wid+var);
    }
    bar->setRect(pos.x(), pos.y(), wid, bar->rect().height());
    if (bar->rect().width() >= frameBar->rect().width()/2)
    {
         bar->setBrush(QBrush(Qt::green));
    }
    else if (bar->rect().width() < frameBar->rect().width()/5)
    {
        bar->setBrush(QBrush(Qt::red));
    }
    else
    {
        bar->setBrush(QBrush(Qt::yellow));
    }
    if (bar->rect().width() <= 0)
        loseGame();
    bar->update();
}

//更新进度
void Control::updateProgressBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var)
{
    qreal wid = bar->rect().width();
    if(var<0)
    {
        wid = max((qreal)0, wid + var);
    }
    else
    {
        wid = min(frameBar->rect().width(), wid+var);
    }
    bar->setRect(pos.x(), pos.y(), wid, bar->rect().height());
    bar->update();
    if (bar->rect().width() == frameBar->rect().width())
        gameOver();
}

//玩家飞机发射子弹
void Control::shootBullet()
{
    /* 玩家飞机发出新子弹，新子弹在玩家飞机炮管外的位置 */
    if(myplane->bullettype == 0)
    {
        Bullet *bullet = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2-8, myplane->y()-30,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet);
        addItem(bullet);
    }
    else if(myplane->bullettype == 1)
    {
        Bullet *bullet1 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2+40, myplane->y()+10,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet1);
        addItem(bullet1);

        Bullet *bullet2 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2, myplane->y()-30,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet2);
        addItem(bullet2);

        Bullet *bullet3 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2-40, myplane->y()+10,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet3);
        addItem(bullet3);
    }
    else if(myplane->bullettype == 2)
    {
        Bullet *bullet = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2, myplane->y()-30,
                                    myGreatBulletImageFile, QPointF(0,-3), myGreatBulletPower);
        mybullets.push_back(bullet);
        addItem(bullet);
    }
    else if(myplane->bullettype == 3)
    {
        Bullet *bullet1 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2+40, myplane->y()+10,
                                    myGreatBulletImageFile, QPointF(0,-3), myGreatBulletPower);
        mybullets.push_back(bullet1);
        addItem(bullet1);

        Bullet *bullet2 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2, myplane->y()-30,
                                    myGreatBulletImageFile, QPointF(0,-3), myGreatBulletPower);
        mybullets.push_back(bullet2);
        addItem(bullet2);

        Bullet *bullet3 = new Bullet(ME, myplane->x()+myplane->pixmap().width()/2-40, myplane->y()+10,
                                    myGreatBulletImageFile, QPointF(0,-3), myGreatBulletPower);
        mybullets.push_back(bullet3);
        addItem(bullet3);
    }
}

//更新我的子弹位置
void Control::updateMyBullets()
{
    //对所有子弹循环
    for (vector<Bullet*>::iterator it = mybullets.begin(); it != mybullets.end(); )
    {
        pair<int, int> loc = (*it)->updatePosition();//更新坐标
        if (changeBulletPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = mybullets.erase(it);
        }
    }
}

//更新补给位置
void Control::updateLifeSupply()
{
    //对所有补给循环
    for (vector<LifeSupply*>::iterator it = lifesupplys.begin(); it != lifesupplys.end(); )
    {
        pair<int, int> loc = (*it)->updatePosition(); //更新坐标
        if (changeLifeSupplyPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = lifesupplys.erase(it);
        }
    }
}

//更新道具位置
void Control::updateSkillProp()
{
    //对所有道具循环
    for (vector<SkillProp*>::iterator it = skillprops.begin(); it != skillprops.end(); )
    {
        pair<int, int> loc = (*it)->updatePosition(); //更新坐标
        if (changeSkillPropPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = skillprops.erase(it);
        }
    }
}

//随机生成一架敌机
bool Control::generateEnemyPlane()
{
    /* 随机在第一行生成敌机 */
    srand(time(NULL));//初始化时间种子
    QPixmap pixmap(QPixmap(QString::fromStdString(enemyPlaneImageFile)));
    int cnt = 0;
    int x = rand() % (int)(width()-160); //敌机最左端位置
    for(cnt=0;cnt<100;cnt++)
    {
        bool flag = true; //此位置是否合法
        for(auto iter : enemyplanes)//对vector循环
        {
//            QApplication::processEvents();
            if(iter->sceneBoundingRect().intersects(QRectF(x, 0, pixmap.width(), pixmap.height())))
            {
                flag = false;
                break;
            }
        }

        if(flag)
            break;
        else
            x = rand() % (int)(width()-160);
    }

    /* 若生成100次随机都未能找到合适的位置则退出 */
    if(cnt>=100)
        return false;

    /* 新增敌机 */
    EnemyPlane *enemy = new EnemyPlane(x, 0, enemyPlaneImageFile, this, ORD, enemyLife);
    enemyplanes.push_back(enemy);
    return true;
}

//随机生成boss
bool Control::generateBoss()
{
    srand(time(NULL));
    QPixmap pixmap(QPixmap(QString::fromStdString(bossImageFile)));
    int cnt = 0;
    int x = rand() % (int)(width()-300); //敌机最左端位置
    for(cnt=0;cnt<100;cnt++)
    {
        bool flag = true; //此位置是否合法
        for(auto iter:enemyplanes)
        {
            if(iter->sceneBoundingRect().intersects(QRectF(x, 0, pixmap.width(), pixmap.height())))
            {
                flag = false;
                break;
            }
        }

        if(flag)
            break;
        else
            x = rand() % (int)(width()-300);
    }

    /* 若生成100次随机都未能找到合适的位置则退出 */
    if(cnt>=100)
        return false;

    /* 新增敌机 */
    EnemyPlane *enemy = new EnemyPlane(x, 0, bossImageFile, this, BOSS, bossLife);
    enemyplanes.push_back(enemy);
    return true;
}

//随机生成大BOSS
bool Control::generateBigBoss()
{
    srand(time(NULL));
    QPixmap pixmap(QPixmap(QString::fromStdString(bigBossImageFile)));
    int cnt = 0;
    int x = rand() % (int)(width()-300); //敌机最左端位置
    for(cnt=0;cnt<100;cnt++)
    {
        bool flag = true; //此位置是否合法
        for(auto iter:enemyplanes)
        {
            if(iter->sceneBoundingRect().intersects(QRectF(x, 0, pixmap.width(), pixmap.height())))
            {
                flag = false;
                break;
            }
        }

        if(flag)
            break;
        else
            x = rand() % (int)(width()-300);
    }

    /* 若生成100次随机都未能找到合适的位置则退出 */
    if(cnt>=100)
        return false;

    /* 新增敌机 */
    EnemyPlane *enemy = new EnemyPlane(x, 0, bigBossImageFile, this, BIGBOSS, bigBossLife);
    enemyplanes.push_back(enemy);
    return true;
}

//生成补给
bool Control::generateLifeSupply(int x, int y)
{
    srand(time(NULL));
    switch (level) {
    case 1: //第一关不掉落补给
        break;
    case 2:
        if(rand()%8==0) //第二关12.5%概率掉落补给
        {
            LifeSupply *lifeSupply = new LifeSupply(x, y, lifeSupplyImageFile, supplyAddLife);
            lifesupplys.push_back(lifeSupply);
            addItem(lifeSupply);
            return true;
        }
        break;
    case 3:
        if(rand()%6==0) //第二关18.6%概率掉落补给
        {
            LifeSupply *lifeSupply = new LifeSupply(x, y, lifeSupplyImageFile, supplyAddLife);
            lifesupplys.push_back(lifeSupply);
            addItem(lifeSupply);
            return true;
        }
        break;
    case 4:
        if(rand()%5==0) //第三关20%概率掉落补给
        {
            LifeSupply *lifeSupply = new LifeSupply(x, y, lifeSupplyImageFile, supplyAddLife);
            lifesupplys.push_back(lifeSupply);
            addItem(lifeSupply);
            return true;
        }
        break;
    default:
        break;
    }

    return false;
}

//生成技能道具
bool Control::generateSkillProp(int x, int y)
{
    srand(time(NULL));
    switch (level) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        if(rand()%4==0) //第三关击落BOSS25%概率掉落技能包
        {
            SkillProp *skillprop;
            switch (rand()%4) {
            case 0:
                skillprop = new SkillProp(x, y, skillProp1ImageFile, 1);
                break;
            case 1:
                skillprop = new SkillProp(x, y, skillProp2ImageFile, 2);
                break;
            case 2:
                skillprop = new SkillProp(x, y, skillProp3ImageFile, 3);
                break;
            case 3:
                skillprop = new SkillProp(x, y, skillProp4ImageFile, 4);
                break;
            default:
                return false;
                break;
            }
            skillprops.push_back(skillprop);
            addItem(skillprop);
            return true;
        }
        break;
    case 4:
        if(rand()%2==0) //第四关击落BOSS50%概率掉落技能包
        {
            SkillProp *skillprop;
            switch (rand()%4) {
            case 0:
                skillprop = new SkillProp(x, y, skillProp1ImageFile, 1);
                break;
            case 1:
                skillprop = new SkillProp(x, y, skillProp2ImageFile, 2);
                break;
            case 2:
                skillprop = new SkillProp(x, y, skillProp3ImageFile, 3);
                break;
            case 3:
                skillprop = new SkillProp(x, y, skillProp4ImageFile, 4);
                break;
            default:
                return false;
                break;
            }
            skillprops.push_back(skillprop);
            addItem(skillprop);
            return true;
        }
        break;
    default:
        break;
    };

    return false;
}

//根据敌机方向更新敌机位置
bool Control::updateEnemyPlanes()
{
    /* 若当前敌机少于1，则自动生成敌机，数目随机但小于3 */
    if (enemyplanes.size() < 1)
    {
        int genNum = rand() % 3;
        for (int i = 0; i < genNum; i++)
            generateEnemyPlane();
    }

    /* 所有敌机移动位置 */
    for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
    {
        pair<int, int> loc = (*it)->updatePostion();
        if (changeEnemyPlanePosition(*it, loc.first, loc.second))
            it++;
        else
            it = enemyplanes.erase(it);
    }

    return myplane->life > 0;
}

//敌机发射子弹
void Control::shootEnemyBullets()
{
    /* 仍存活的敌机发出新子弹，新子弹在敌机炮管外的位置 */
    for (vector<EnemyPlane*>::iterator iter = enemyplanes.begin(); iter != enemyplanes.end(); iter++)
    {
        if ((*iter)->life > 0)
        {
            if((*iter)->type==ORD)
            {
                Bullet *bullet = new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                        enemyBulletImageFile, QPointF(0,1), enemyBulletPower);
                enemybullets.push_back(bullet);
                addItem(bullet);
            }
            else if((*iter)->type==BOSS)
            {
                Bullet *bullet0 = new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                     bossBulletImageFile, QPointF(0,1), bossBulletPower);
                enemybullets.push_back(bullet0);
                addItem(bullet0);

                Bullet *bullet1 = new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                     bossBulletImageFile, QPointF(-1,1), bossBulletPower);
                bullet1->setRotation(45);
                enemybullets.push_back(bullet1);
                addItem(bullet1);

                Bullet *bullet2 = new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                     bossBulletImageFile, QPointF(1,1), bossBulletPower);
                bullet2->setRotation(-45);
                enemybullets.push_back(bullet2);
                addItem(bullet2);
            }
            else if((*iter)->type==BIGBOSS)
            {
                Missile *missile = new Missile((*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                      missileImageFile, missileLife, missilePower);
                missiles.push_back(missile);
                addItem(missile);
            }
        }
    }
}

//更新敌机子弹位置
void Control::updateEnemyBullets()
{
    for(vector<Bullet*>::iterator it = enemybullets.begin(); it!=enemybullets.end(); )
    {
        pair<int ,int> loc = (*it)->updatePosition();
        if(changeBulletPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = enemybullets.erase(it);
        }
    }
}

//更新导弹位置
void Control::updateMissiles()
{
    for(vector<Missile*>::iterator it = missiles.begin(); it!=missiles.end(); )
    {
        pair<int ,int> loc = (*it)->updatePosition(myplane->x()+myplane->pixmap().width()/2, myplane->y()+myplane->pixmap().height()/2);
        if(changeMissilePosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = missiles.erase(it);
        }
    }
}

//大厅
void Control::welcomeGame()
{
    level = 1;
    score = 0;

    enterGameButton->show();
    onlineGameButton->show();
    helpGameButton->show();
    exitGameButton->show();

    warOfPlaneText->show();
    welcomeText->show();
}

//进入游戏
void Control::enterGame()
{
    welcomeText->hide();
    enterGameButton->hide();
    onlineGameButton->hide();
    helpGameButton->hide();
    exitGameButton->hide();

    switch (account->getUnlockLevel()) {
    case 1:
        chooseLevelTwoButton->setEnabled(false);
        chooseLevelThreeButton->setEnabled(false);
        chooseLevelFourButton->setEnabled(false);
        break;
    case 2:
        chooseLevelTwoButton->setEnabled(true);
        chooseLevelThreeButton->setEnabled(false);
        chooseLevelFourButton->setEnabled(false);
        break;
    case 3:
        chooseLevelTwoButton->setEnabled(true);
        chooseLevelThreeButton->setEnabled(true);
        chooseLevelFourButton->setEnabled(false);
        break;
    case 4:
        chooseLevelTwoButton->setEnabled(true);
        chooseLevelThreeButton->setEnabled(true);
        chooseLevelFourButton->setEnabled(true);
        break;
    }

    chooseLevelOneButton->show();
    chooseLevelTwoButton->show();
    chooseLevelThreeButton->show();
    chooseLevelFourButton->show();
}

//选择关卡一
void Control::startLevelOne()
{
    level = 1;
    gameProgressItv = 25;
    warOfPlaneText->hide();
    chooseLevelOneButton->hide();
    chooseLevelTwoButton->hide();
    chooseLevelThreeButton->hide();
    chooseLevelFourButton->hide();

    choosePlaneText->show();
    chooseBalancePlaneButton->show();
    chooseMoveSpeedPlaneButton->show();
    chooseShootSpeedPlaneButton->show();
    chooseLifePlaneButton->show();
}

//选择关卡二
void Control::startLevelTwo()
{
    level = 2;
    gameProgressItv = 25;
    warOfPlaneText->hide();
    chooseLevelOneButton->hide();
    chooseLevelTwoButton->hide();
    chooseLevelThreeButton->hide();
    chooseLevelFourButton->hide();

    choosePlaneText->show();
    chooseBalancePlaneButton->show();
    chooseMoveSpeedPlaneButton->show();
    chooseShootSpeedPlaneButton->show();
    chooseLifePlaneButton->show();
}

//选择关卡三
void Control::startLevelThree()
{
    level = 3;
    gameProgressItv = 25;
    warOfPlaneText->hide();
    chooseLevelOneButton->hide();
    chooseLevelTwoButton->hide();
    chooseLevelThreeButton->hide();
    chooseLevelFourButton->hide();

    choosePlaneText->show();
    chooseBalancePlaneButton->show();
    chooseMoveSpeedPlaneButton->show();
    chooseShootSpeedPlaneButton->show();
    chooseLifePlaneButton->show();
}

//选择关卡四
void Control::startLevelFour()
{
    level = 4;
    gameProgressItv = 100;
    warOfPlaneText->hide();
    chooseLevelOneButton->hide();
    chooseLevelTwoButton->hide();
    chooseLevelThreeButton->hide();
    chooseLevelFourButton->hide();

    choosePlaneText->show();
    chooseBalancePlaneButton->show();
    chooseMoveSpeedPlaneButton->show();
    chooseShootSpeedPlaneButton->show();
    chooseLifePlaneButton->show();
}

void Control::nextLevelGame()
{
    if(myplane->life>=0)
        removeItem(myplane);

    for(auto iter:enemyplanes)
    {
        removeItem(iter);
        delete iter;
    }
    enemyplanes.clear();

    for(auto it:mybullets)
    {
        removeItem(it);
        delete it;
    }
    mybullets.clear();

    for(auto it:enemybullets)
    {
        removeItem(it);
        delete it;
    }
    enemybullets.clear();

    for(auto it:lifesupplys)
    {
        removeItem(it);
        delete it;
    }
    lifesupplys.clear();

    for(auto it:skillprops)
    {
        removeItem(it);
        delete it;
    }
    skillprops.clear();

    for(auto it:missiles)
    {
        removeItem(it);
        delete it;
    }
    missiles.clear();

    hasStarted = false;
    isPause = false;

    gameLostText->hide();
    gameWinText->hide();

    maskWidget->hide();
    nextLevelButton->hide();
    continueGameButton->hide();
    retryGameButton->hide();
    helpGameButton->hide();
    returnHallButton->hide();
    checkRankGameButton->hide();

    lifeBar->hide();
    lifeText->hide();
    lifeFrameBar->hide();
    progressBar->hide();
    progressText->hide();
    progressFrameBar->hide();
    scoreText->hide();

    switch (level) {
    case 1:
        startLevelTwo();
        break;
    case 2:
        startLevelThree();
        break;
    case 3:
        startLevelFour();
        break;
    default:
        break;
    }
}

//均衡战机
void Control::chooseBalancePlane()
{
    this->myLife = 50;
    this->myMoveSpeed = 30;
    this->myShootSpeed = 300;
    this->myPlaneImageFile = ":/new/prefix1/images/balanceplane.png";

    choosePlaneText->hide();
    chooseBalancePlaneButton->hide();
    chooseMoveSpeedPlaneButton->hide();
    chooseShootSpeedPlaneButton->hide();
    chooseLifePlaneButton->hide();

    startGame();
}

//移速战机
void Control::chooseMoveSpeedPlane()
{
    this->myLife = 42;
    this->myMoveSpeed = 19;
    this->myShootSpeed = 300;
    this->myPlaneImageFile = ":/new/prefix1/images/movespeedplane.png";

    choosePlaneText->hide();
    chooseBalancePlaneButton->hide();
    chooseMoveSpeedPlaneButton->hide();
    chooseShootSpeedPlaneButton->hide();
    chooseLifePlaneButton->hide();

    startGame();
}

//攻速战机
void Control::chooseShootSpeedPlane()
{
    this->myLife = 40;
    this->myMoveSpeed = 30;
    this->myShootSpeed = 200;
    this->myPlaneImageFile = ":/new/prefix1/images/shootspeedplane.png";

    choosePlaneText->hide();
    chooseBalancePlaneButton->hide();
    chooseMoveSpeedPlaneButton->hide();
    chooseShootSpeedPlaneButton->hide();
    chooseLifePlaneButton->hide();

    startGame();
}

//生命战机
void Control::chooseLifePlane()
{
    this->myLife = 60;
    this->myMoveSpeed = 34;
    this->myShootSpeed = 300;
    this->myPlaneImageFile = ":/new/prefix1/images/lifeplane.png";

    choosePlaneText->hide();
    chooseBalancePlaneButton->hide();
    chooseMoveSpeedPlaneButton->hide();
    chooseShootSpeedPlaneButton->hide();
    chooseLifePlaneButton->hide();

    startGame();
}

//开始游戏
void Control::startGame()
{
    score = 0;
    //重置分数为0
    updateScore(0);
    player->play();

    //开始游戏
    hasStarted = true;

    //显示文本
    lifeText->show();
    progressText->show();

    progressFrameBar->show();
    lifeFrameBar->show();

    lifeFrameBar->setRect(lifeBarPos.x(), lifeBarPos.y(), myLife*4, lifeFrameBar->rect().height());
    lifeFrameBar->setPen(QPen(Qt::white));
    lifeFrameBar->update();
    lifeFrameBar->show();

    lifeBar->setRect(lifeBarPos.x(), lifeBarPos.y(), myLife*4, lifeBar->rect().height());
    lifeBar->setBrush(Qt::green);
    lifeBar->update();
    lifeBar->show();

    progressBar->setRect(progressBarPos.x(), progressBarPos.y(), 2, progressBar->rect().height());
    progressBar->setBrush(Qt::blue);
    progressBar->update();
    progressBar->show();

    myPlaneWMove = QPointF(0, 0);
    myPlaneSMove = QPointF(0, 0);
    myPlaneAMove = QPointF(0, 0);
    myPlaneDMove = QPointF(0, 0);

    //添加玩家飞机
    myplane = new MyPlane(width() / 2, height() / 2, myPlaneImageFile, this, myLife, myMoveSpeed, myShootSpeed);
    myplane->synScreen(this);

    //启动全局定时器
    startALLTimer();

    //60秒游戏结束
    gameProgressId = startTimer(gameProgressItv);
}

//暂停游戏
void Control::pauseGame()
{
    if(hasStarted)
        if(!isPause)
        {
            hasShooted = false;
            isPause = true;

            killAllTimer();

            maskWidget->show();
            continueGameButton->show();
            retryGameButton->show();
            helpGameButton->show();
            returnHallButton->show();
        }
        else
        {
            continueGame();
        }
}

//继续游戏
void Control::continueGame()
{
    if(hasStarted)
    {
        if(isPause)
        {
            isPause = false;
            startALLTimer();

            maskWidget->hide();
            continueGameButton->hide();
            retryGameButton->hide();
            helpGameButton->hide();
            returnHallButton->hide();
        }
    }
}

//游戏结束
void Control::gameOver()
{
    killAllTimer();

    skillKText->hide();
    skillLText->hide();

    gameWinText->setHtml(tr("<font color=white>You Win, Your Score: %1</font>").arg(score));
    gameWinText->show();

    DBHelper *dBHelper = dBHelperInstance;
    QSqlDatabase database = dBHelper->database;
    //存储本局数据
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        QSqlQuery sql_query;
        QDateTime currentTime =QDateTime::currentDateTime();

        QString insert_sql = tr("insert into gamescore (username, level, score, time) values (\'%1\', %2, %3, \'%4\')").arg(account->getUsername()).arg(level).arg(score).arg(currentTime.toString("yyyy.MM.dd"));
        if( !sql_query.exec(insert_sql) )
        {
            qDebug()<< "Database Error: Failed to update." << sql_query.lastError();
        }
        else
        {
            qDebug()<< "insert!";
        }

        //更新等级
        if (level < 4 && level == account->getUnlockLevel())
        {
            account->setUnlockLevel(level + 1);

            QSqlQuery sql_query;
            QString update_sql = tr("update account set unlocklevel = \'%1\' where username = \'%2\'").arg(level+1).arg(account->getUsername());
            if( !sql_query.exec(update_sql) )
            {
                qDebug()<< "Database Error: Failed to update." << sql_query.lastError();
            }
            else
            {
                qDebug()<< "update!";
            }
        }

        database.close();
    }

    maskWidget->show();
    hasStarted = false;
    hasShooted = false;

    if (level < 4)
        nextLevelButton->show();

    retryGameButton->show();
    checkRankGameButton->show();
    returnHallButton->show();
}

//重新开始游戏
void Control::retryGame()
{
    score = 0;

    if(myplane->life>=0)
        removeItem(myplane);

    for(auto iter:enemyplanes)
    {
        removeItem(iter);
        delete iter;
    }
    enemyplanes.clear();

    for(auto it:mybullets)
    {
        removeItem(it);
        delete it;
    }
    mybullets.clear();

    for(auto it:enemybullets)
    {
        removeItem(it);
        delete it;
    }
    enemybullets.clear();

    for(auto it:lifesupplys)
    {
        removeItem(it);
        delete it;
    }
    lifesupplys.clear();

    for(auto it:skillprops)
    {
        removeItem(it);
        delete it;
    }
    skillprops.clear();

    for(auto it:missiles)
    {
        removeItem(it);
        delete it;
    }
    missiles.clear();

    isPause = false;
    killAllTimer();

    gameLostText->hide();
    gameWinText->hide();
    maskWidget->hide();
    nextLevelButton->hide();
    continueGameButton->hide();
    retryGameButton->hide();
    helpGameButton->hide();
    checkRankGameButton->hide();
    returnHallButton->hide();

    startGame();
}

//返回游戏结束页面
void Control::returnToGameOver()
{
    scoreTableWidget->hide();
    returnGameOverButton->hide();

    if (level < 4)
        nextLevelButton->show();
    gameWinText->show();
    retryGameButton->show();
    checkRankGameButton->show();
    returnHallButton->show();
}

//返回大厅
void Control::returnHallGame()
{
    if(myplane->life>=0)
        removeItem(myplane);

    for(auto iter:enemyplanes)
    {
        removeItem(iter);
        delete iter;
    }
    enemyplanes.clear();

    for(auto it:mybullets)
    {
        removeItem(it);
        delete it;
    }
    mybullets.clear();

    for(auto it:enemybullets)
    {
        removeItem(it);
        delete it;
    }
    enemybullets.clear();

    for(auto it:lifesupplys)
    {
        removeItem(it);
        delete it;
    }
    lifesupplys.clear();

    for(auto it:skillprops)
    {
        removeItem(it);
        delete it;
    }
    skillprops.clear();

    for(auto it:missiles)
    {
        removeItem(it);
        delete it;
    }
    missiles.clear();

    hasStarted = false;
    isPause = false;

    gameLostText->hide();
    gameWinText->hide();

    maskWidget->hide();
    nextLevelButton->hide();
    continueGameButton->hide();
    retryGameButton->hide();
    helpGameButton->hide();
    returnHallButton->hide();
    checkRankGameButton->hide();

    lifeBar->hide();
    lifeText->hide();
    lifeFrameBar->hide();
    progressBar->hide();
    progressText->hide();
    progressFrameBar->hide();
    scoreText->hide();

    welcomeGame();
}

//游戏失败
void Control::loseGame()
{
    hasStarted = false;
    hasShooted = false;

    skillKText->hide();
    skillLText->hide();

    killAllTimer();
    maskWidget->show();

    gameLostText->setHtml(tr("<font color=white>Game Over, Your Score: %1</font>").arg(score));
    gameLostText->show();

    retryGameButton->show();
    helpGameButton->show();
    returnHallButton->show();
}

//退出游戏
void Control::exitGame()
{
    exit(0);
}

//显示排名
void Control::showTheRank()
{
    if (level < 4)
        nextLevelButton->hide();
    gameWinText->hide();
    retryGameButton->hide();
    checkRankGameButton->hide();
    returnHallButton->hide();

    QStandardItemModel *scoreModel = new QStandardItemModel(11, 4);
    scoreTable->setModel(scoreModel);
    scoreModel->setItem(0, 0, new QStandardItem("NO."));
    scoreModel->setItem(0, 1, new QStandardItem("Player"));
    scoreModel->setItem(0, 2, new QStandardItem("Score"));
    scoreModel->setItem(0, 3, new QStandardItem("Time"));

    int row = 1;

    DBHelper *dBHelper = dBHelperInstance;
    QSqlDatabase database = dBHelper->database;

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        QSqlQuery sql_query;
        QString select_sql = tr("select * from gamescore where level = \'%1\' order by score desc").arg(level);
        if( !sql_query.exec(select_sql) )
        {
            qDebug()<< "Database Error: Failed to select." << sql_query.lastError();
        }
        else
        {
            while (sql_query.next() && row <= 10)
            {
                scoreModel->setItem(row, 0, new QStandardItem(QString::number(row)));
                scoreModel->setItem(row, 1, new QStandardItem(sql_query.value(1).toString()));
                scoreModel->setItem(row, 2, new QStandardItem(sql_query.value(3).toString()));
                scoreModel->setItem(row, 3, new QStandardItem(sql_query.value(4).toString()));
                row++;
            }
            database.close();
        }
    }

    database.close();

    for (int i=0; i<row; i++)
    {
        for (int j=0; j<4; j++)
        {
            scoreModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
            scoreModel->item(i, j)->setForeground(QBrush(QColor(255, 255, 255)));
            scoreModel->item(i, j)->setFont(QFont("Times", 13, QFont::Black ));
            if (i%2 == 0)
                scoreModel->item(i, j)->setBackground(QBrush(QColor(144, 144, 144, 80)));
        }
    }

    scoreTableWidget->show();

    returnGameOverButton->show();
}

//显示帮助页面
void Control::showHelpMessage()
{
    QString helpText;
    helpText += tr("\n操作:\n");
    helpText += tr("W: 上;S: 下;A: 左; D: 右\n");
    helpText += tr("J: 发射子弹\n");
    helpText += tr("\n使用技能:\n");
    helpText += tr("K: 清除所有敌机\n");
    helpText += tr("L: 清除所有子弹\n");
    QMessageBox::information(NULL,tr("游戏帮助"),helpText);
}

//进入联机游戏
void Control::onlineGame()
{
    enterGameButton->hide();
    onlineGameButton->hide();
    helpGameButton->hide();
    exitGameButton->hide();

    usersTableWidget->show();
}


QString Control::getMyIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) //输出IPV4的地址
            return address.toString();
    }
       return 0;
}

//发送数据包
void Control::sendMessage(MessageType type, QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly); //发送数据包
    QString localHostName = QHostInfo::localHostName(); //获取主机名
    QString address = getMyIp(); //获取IPV4的地址
//    out<<type<<getUserName()<<localHostName;
    switch(type)
    {
        case Message:
            break;
        case NewUser:
            out<<address;
            break;
        case Ready:
            break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

// 接收数据包
void Control::pendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;  //报文
        datagram.resize(udpSocket->pendingDatagramSize());  //设置报文长度
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;  //发送消息类型
        in>>messageType;
        QString userName,localHostName,ipAddress,message;
        QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (messageType) {
        case Message:

            break;
        case Ready:

            break;
        default:
            break;
        }
    }
}
