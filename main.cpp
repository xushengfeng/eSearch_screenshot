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
           qDebug()<<screen_list.at(0)->geometry();
       }

    QScreen *screen = QApplication::primaryScreen();
    screen->grabWindow(0).save("123","png");
//    w.show();
    return a.exec();
}
