#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

#include "dbhelper.h"
#include "account.h"

class QLabel;
class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    Account *getAccount();

private:
    Account *account;
    \
    QFrame *frame;

    QPushButton *closeBtn;

    //登录
    QLabel *usrLabel;
    QLabel *pwdLabel;
    QLabel *loginMessage;
    QLineEdit *usrLineEdit;
    QLineEdit *pwdLineEdit;
    QPushButton *loginBtn;
    QPushButton *registerBtn;

    //注册
    QLabel *registerUsrLabel;
    QLabel *registerPwdLabel;
    QLabel *retryPwdLabel;
    QLabel *usrMessage;
    QLabel *usrEmptyMessage;
    QLabel *pwdEmptyMessage;
    QLabel *pwdMessage;
    QLineEdit *registerUsrLineEdit;
    QLineEdit *registerPwdLineEdit;
    QLineEdit *retryPwdLineEdit;
    QPushButton *registerStartBtn;
    QPushButton *returnBtn;

    bool usernameUseable(QString username);

private slots:
    void closeBtn_clicked();
    void loginBtn_clicked();
    void registerBtn_clicked();
    void registerStartBtn_clicked();
    void returnBtn_clicked();
};

#endif // LOGINDIALOG_H
