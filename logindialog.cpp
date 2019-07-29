#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent):
    QDialog(parent)
{
    account = new Account();

    this->setAutoFillBackground(true);
    this->setFixedSize(550, 430);
    this->setWindowFlags(Qt::FramelessWindowHint);           //去除边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);  //去除背景

    frame = new QFrame(this);
    frame->setGeometry(10,10,530,410);
    frame->setStyleSheet("QFrame{border-radius:10px;background-image:url(:/new/prefix1/images/widgetbackground.png)}");

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);   //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::gray);  //设置阴影颜色
    effect->setBlurRadius(20);   //设定阴影模糊度

    frame->setGraphicsEffect(effect);

    //关闭面板
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(490, 10, 50, 50);
    closeBtn->setDefault(false);
//    closeBtn->setEnabled(false);
    closeBtn->setIcon(QPixmap(":/ico/ico/close.png"));
    closeBtn->setStyleSheet("QPushButton{background-image: url(:/ico/ico/close.png); color:white; border-radius:10px; background-color:rgba(0,0,0,0)}"
                            "QPushButton:hover{color:red; border-radius:10px; background-color:rgba(255,0,0,250)}");

    //用户名标签
    usrLabel = new QLabel(this);
    usrLabel->setPixmap(QPixmap(":/ico/ico/user.png"));
    usrLabel->setGeometry(70,255,40,40);

    //用户名输入行
    usrLineEdit = new QLineEdit(this);
    usrLineEdit->setPlaceholderText("请输入用户名");
    usrLineEdit->setGeometry(110,250,330,50);

    //密码标签
    pwdLabel = new QLabel(this);
    pwdLabel->setPixmap(QPixmap(":/ico/ico/password.png"));
    pwdLabel->setGeometry(70,305,40,40);

    //密码输入行
    pwdLineEdit = new QLineEdit(this);
    pwdLineEdit->setPlaceholderText(tr("请输入密码"));
    pwdLineEdit->setEchoMode(QLineEdit::Password);
    pwdLineEdit->setGeometry(110,300,330,50);

    //用户名和密码错误提示
    loginMessage = new QLabel(this);
    loginMessage->setText(tr("用户名或\n"
                             "密码错误"));
    QPalette color;
    color.setColor(QPalette::WindowText,Qt::red);
    loginMessage->setPalette(color);
    loginMessage->move(450, 260);
    loginMessage->hide();

    //登录按钮
    loginBtn = new QPushButton(this);
    loginBtn->setText(tr("登 录"));
    loginBtn->setGeometry(280,360,160,50);
    loginBtn->setDefault(false);
    loginBtn->setStyleSheet("QPushButton{color:white; border-radius:5px; background-color:rgb(0,163,255)}"
                            "QPushButton:hover{color:white; border-radius:5px; background-color:rgb(60,195,245)}"
                            "QPushButton:hover:pressed{color:white; border-radius:5px;background-color:rgb(9,140,188)}");

    //注册按钮
    registerBtn = new QPushButton(this);
    registerBtn->setText(tr("注 册"));
    registerBtn->setGeometry(110,360,160,50);
    registerBtn->setDefault(false);
    registerBtn->setStyleSheet("QPushButton{color:white; border-radius:5px; background-color:rgb(0,163,255)}"
                            "QPushButton:hover{color:white; border-radius:5px; background-color:rgb(60,195,245)}"
                            "QPushButton:hover:pressed{color:white; border-radius:5px;background-color:rgb(9,140,188)}");


    //注册用户名标签
    registerUsrLabel = new QLabel(this);
    registerUsrLabel->setPixmap(QPixmap(":/ico/ico/user.png"));
    registerUsrLabel->setGeometry(70,240,40,40);
    registerUsrLabel->hide();

    //注册密码标签
    registerPwdLabel = new QLabel(this);
    registerPwdLabel->setPixmap(QPixmap(":/ico/ico/password.png"));
    registerPwdLabel->setGeometry(70,280,40,40);
    registerPwdLabel->hide();

    //再次输入密码标签
    retryPwdLabel = new QLabel(this);
    retryPwdLabel->setPixmap(QPixmap(":/ico/ico/password.png"));
    retryPwdLabel->setGeometry(70,320,40,40);
    retryPwdLabel->hide();

    //注册用户名输入行
    registerUsrLineEdit = new QLineEdit(this);
    registerUsrLineEdit->setPlaceholderText("请输入用户名");
    registerUsrLineEdit->setGeometry(110,240,330,40);
    registerUsrLineEdit->hide();

    //注册密码输入行
    registerPwdLineEdit = new QLineEdit(this);
    registerPwdLineEdit->setPlaceholderText("请输入密码");
    registerPwdLineEdit->setEchoMode(QLineEdit::Password);
    registerPwdLineEdit->setGeometry(110,280,330,40);
    registerPwdLineEdit->hide();

    //再次输入密码输入行
    retryPwdLineEdit = new QLineEdit(this);
    retryPwdLineEdit->setPlaceholderText("请再次输入密码");
    retryPwdLineEdit->setEchoMode(QLineEdit::Password);
    retryPwdLineEdit->setGeometry(110,320,330,40);
    retryPwdLineEdit->hide();

    //用户名为空提示
    usrEmptyMessage = new QLabel(this);
    usrEmptyMessage->setText(tr("不能为空"));
    usrEmptyMessage->setPalette(color);
    usrEmptyMessage->move(450, 250);
    usrEmptyMessage->hide();

    //用户名已被注册提示
    usrMessage = new QLabel(this);
    usrMessage->setText(tr("已被注册"));
    usrMessage->setPalette(color);
    usrMessage->move(450, 250);
    usrMessage->hide();

    //密码为空提示
    pwdEmptyMessage = new QLabel(this);
    pwdEmptyMessage->setText(tr("不能为空"));
    pwdEmptyMessage->setPalette(color);
    pwdEmptyMessage->move(450, 293);
    pwdEmptyMessage->hide();

    //两次密码不一致提示
    pwdMessage = new QLabel(this);
    pwdMessage->setText(tr("密码不一致"));
    pwdMessage->setPalette(color);
    pwdMessage->move(450, 333);
    pwdMessage->hide();

    //确定注册按钮
    registerStartBtn = new QPushButton(this);
    registerStartBtn->setText(tr("确 定"));
    registerStartBtn->setGeometry(280,370,160,40);
    registerStartBtn->hide();
    registerStartBtn->setDefault(false);
    registerStartBtn->setStyleSheet("QPushButton{color:white; border-radius:5px; background-color:rgb(0,163,255)}"
                            "QPushButton:hover{color:white; border-radius:5px; background-color:rgb(60,195,245)}"
                            "QPushButton:hover:pressed{color:white; border-radius:5px;background-color:rgb(9,140,188)}");

    //返回按钮
    returnBtn = new QPushButton(this);
    returnBtn->setText(tr("返 回"));
    returnBtn->setGeometry(110,370,160,40);
    returnBtn->hide();
    returnBtn->setDefault(false);
    returnBtn->setStyleSheet("QPushButton{color:white; border-radius:5px; background-color:rgb(0,163,255)}"
                            "QPushButton:hover{color:white; border-radius:5px; background-color:rgb(60,195,245)}"
                            "QPushButton:hover:pressed{color:white; border-radius:5px;background-color:rgb(9,140,188)}");


    connect(closeBtn, &QPushButton::clicked, this, &LoginDialog::closeBtn_clicked);
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::loginBtn_clicked);
    connect(registerBtn, &QPushButton::clicked, this, &LoginDialog::registerBtn_clicked);
    connect(registerStartBtn, &QPushButton::clicked, this, &LoginDialog::registerStartBtn_clicked);
    connect(returnBtn, &QPushButton::clicked, this, &LoginDialog::returnBtn_clicked);
}

LoginDialog::~LoginDialog()
{

}

bool LoginDialog::usernameUseable(QString username)
{
    DBHelper *dBHelper = dBHelperInstance;
    QSqlDatabase database = dBHelper->database;

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        //按用户名查询
        QSqlQuery sql_query;
        QString select_sql = tr("select count(*) from account where username = \'%1\'").arg(username);
        if( !sql_query.exec(select_sql) )
        {
            qDebug()<< "Database Error: Failed to select." << sql_query.lastError();
        }
        else
        {
            while (sql_query.next())
            {
                database.close();
                return !sql_query.value(0).toInt();
            }
        }
    }

    return false;
}

void LoginDialog::closeBtn_clicked()
{
    exit(0);
}

void LoginDialog::loginBtn_clicked()
{
    QString username = usrLineEdit->text().trimmed();
    QString password = pwdLineEdit->text();

    QString selectUsername;
    QString selectPassword;
    int selectUnlockLevel;

    DBHelper *dBHelper = dBHelperInstance;
    QSqlDatabase database = dBHelper->database;
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        //按用户名查询
        QSqlQuery sql_query;
        QString select_sql = tr("select * from account where username = \'%1\'").arg(username);
        if( !sql_query.exec(select_sql) )
        {
            qDebug()<< "Database Error: Failed to select." << sql_query.lastError();
        }
        else
        {
            while (sql_query.next())
            {
                selectUsername = sql_query.value(1).toString();
                selectPassword = sql_query.value(2).toString();
                selectUnlockLevel = sql_query.value(3).toInt();
            }
            database.close();
        }
    }

    if(password == selectPassword && username.length()>0 && password.length()>0)
    {
        account->setUserName(selectUsername);
        account->setPassword(selectPassword);
        account->setUnlockLevel(selectUnlockLevel);
        accept();
    } else {
        loginMessage->show();
        usrLineEdit->clear();
        pwdLineEdit->clear();
        usrLineEdit->setFocus();
    }

}

void LoginDialog::registerBtn_clicked()
{
    usrLabel->hide();
    pwdLabel->hide();
    loginMessage->hide();
    usrLineEdit->hide();
    pwdLineEdit->hide();
    loginBtn->hide();
    registerBtn->hide();

    registerUsrLabel->show();
    registerPwdLabel->show();
    retryPwdLabel->show();
    registerUsrLineEdit->show();
    registerPwdLineEdit->show();
    retryPwdLineEdit->show();
    registerStartBtn->show();
    returnBtn->show();
    registerUsrLineEdit->setFocus();
}

void LoginDialog::registerStartBtn_clicked()
{
    QString username = registerUsrLineEdit->text().trimmed();
    QString password = registerPwdLineEdit->text();
    QString passwordRetry = retryPwdLineEdit->text();

    if (username.length() == 0)
    {
        pwdMessage->hide();
        pwdEmptyMessage->hide();
        usrMessage->hide();
        usrEmptyMessage->show();
        usrEmptyMessage->show();
        registerPwdLineEdit->clear();
        retryPwdLineEdit->clear();
        usrEmptyMessage->setFocus();
    }
    else if (!usernameUseable(username))
    {
        pwdMessage->hide();
        usrEmptyMessage->hide();
        pwdEmptyMessage->hide();
        usrMessage->show();
        registerUsrLineEdit->clear();
        registerPwdLineEdit->clear();
        retryPwdLineEdit->clear();
        usrEmptyMessage->setFocus();
    }
    else if (password.length() == 0)
    {
        pwdMessage->hide();
        usrEmptyMessage->hide();
        usrMessage->hide();
        pwdEmptyMessage->show();
        registerPwdLineEdit->setFocus();
    }
    else if (password != passwordRetry)
    {
        usrMessage->hide();
        usrEmptyMessage->hide();
        pwdEmptyMessage->hide();
        pwdMessage->show();
        registerPwdLineEdit->clear();
        retryPwdLineEdit->clear();
        registerPwdLineEdit->setFocus();
    }
    else
    {
        DBHelper *dBHelper = dBHelperInstance;
        QSqlDatabase database = dBHelper->database;
        if (!database.open())
        {
            qDebug() << "Error: Failed to connect database." << database.lastError();
        }
        else
        {
            QSqlQuery sql_query;
            QString insert_sql = tr("insert into account (username, password, unlocklevel) values (\'%1\', \'%2\', 1)").arg(username).arg(password);
            if(!sql_query.exec(insert_sql))
            {
                qDebug()<< "Database Error: Failed to insert." << sql_query.lastError();
            }
            else
            {
                QMessageBox::warning(this, tr("注册成功！"),
                            tr("注册成功！"),
                            QMessageBox::Ok);
                registerUsrLineEdit->clear();
                registerPwdLineEdit->clear();
                retryPwdLineEdit->clear();

                registerUsrLabel->hide();
                registerPwdLabel->hide();
                retryPwdLabel->hide();
                usrMessage->hide();
                usrEmptyMessage->hide();
                pwdEmptyMessage->hide();
                pwdMessage->hide();
                registerUsrLineEdit->hide();
                registerPwdLineEdit->hide();
                retryPwdLineEdit->hide();
                returnBtn->hide();
                registerStartBtn->hide();

                usrLabel->show();
                pwdLabel->show();
                usrLineEdit->show();
                pwdLineEdit->show();
                loginBtn->show();
                registerBtn->show();
                usrLineEdit->setFocus();
            }
            database.close();
        }
    }
}

void LoginDialog::returnBtn_clicked()
{
    registerUsrLabel->hide();
    registerPwdLabel->hide();
    retryPwdLabel->hide();
    usrMessage->hide();
    usrEmptyMessage->hide();
    pwdEmptyMessage->hide();
    pwdMessage->hide();
    registerUsrLineEdit->hide();
    registerPwdLineEdit->hide();
    retryPwdLineEdit->hide();
    registerStartBtn->hide();
    returnBtn->hide();

    usrLabel->show();
    pwdLabel->show();
    usrLineEdit->show();
    pwdLineEdit->show();
    loginBtn->show();
    registerBtn->show();
    usrLineEdit->setFocus();
}

Account* LoginDialog::getAccount()
{
    return this->account;
}

