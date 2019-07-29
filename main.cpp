#include "logindialog.h"
#include "control.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog logindialog;
    
//    QString qss;
//    QFile qssFile(":/qss/loginQss.qss");
//    qssFile.open(QFile::ReadOnly);

//    if(qssFile.isOpen())
//    {
//        qss = QLatin1String(qssFile.readAll());
//        logindialog.setStyleSheet(qss);
//        qssFile.close();
//    }

    if (logindialog.exec() == QDialog::Accepted)
    {
        Control *ctrl = new Control(logindialog.getAccount());

        QGraphicsView *view = new QGraphicsView;
        view->setSceneRect(0,0,1500,900);
        view->setFixedSize(1502,902);
        view->setScene(ctrl); //将场景放入试图
        view->setWindowIcon(QIcon(QPixmap(":/ico/ico/logo.png")));

        view->setWindowTitle(QObject::tr("飞机大战"));
        view->setBackgroundBrush(QPixmap(":/new/prefix1/images/background.jpg"));
        view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        view->show();
        return a.exec();
    }

    return a.exec();
}
