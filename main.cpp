#include "widget.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QList<QScreen *> screen_list= QGuiApplication::screens();
       for(int i=0;i<screen_list.count();i++)
       {
           qDebug()<<screen_list.at(i)->geometry();
           screen_list.at(i)->grabWindow(0).save(QString::number(i),"png");
       }
//    w.show();
    return a.exec();
}
