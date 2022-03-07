#include "widget.h"

#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QList<QScreen *> screen_list = QGuiApplication::screens();
    for (int i = 0; i < screen_list.count(); i++) {
        qDebug() << screen_list.at(i)->geometry();
        QPixmap pixmap;
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        screen_list.at(i)->grabWindow(0).save(&buffer, "png");
    }
    //    w.show();
    return a.exec();
}
