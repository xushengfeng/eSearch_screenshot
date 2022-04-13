#include "widget.h"

#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QScreen>

#ifdef Q_OS_WIN
#endif
#ifdef Q_OS_LINUX
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <xcb/xcb.h>
#endif
#ifdef Q_OS_MAC
#endif

static Display *xdisplay = 0;

Display *disp()
{
    if (!xdisplay)
        xdisplay = XOpenDisplay(NULL);

    return xdisplay;
}

//枚举所有窗口
std::list<Window> getTopLevelWindows()
{
    std::list<Window> res;

    Atom netClList = XInternAtom(disp(), "_NET_CLIENT_LIST", true);
    Atom actualType;
    int format;
    unsigned long num, bytes;
    Window *data = 0;

    for (int i = 0; i < ScreenCount(disp()); ++i) {
        Window rootWin = RootWindow(disp(), i);

        int status = XGetWindowProperty(disp(), rootWin, netClList, 0L,
                                        ~0L, false, AnyPropertyType,
                                        &actualType, &format, &num,
                                        &bytes, (uint8_t **)&data);

        if (status != Success) {
            qDebug("Failed getting root "
                   "window properties");
            continue;
        }

        for (unsigned long i = 0; i < num; ++i)
            res.push_back(data[i]);

        XFree(data);
    }

    return res;
}

std::string getWindowName(Window win)
{
    const char *atom = "_NET_WM_NAME";
    Atom netWmName = XInternAtom(disp(), atom, false);
    int n;
    char **list = 0;
    XTextProperty tp;
    std::string res = "unknown";

    XGetTextProperty(disp(), win, &tp, netWmName);

    if (!tp.nitems)
        XGetWMName(disp(), win, &tp);

    if (!tp.nitems)
        return "error";

    if (tp.encoding == XA_STRING) {
        res = (char *)tp.value;
    } else {
        int ret = XmbTextPropertyToTextList(disp(), &tp, &list, &n);

        if (ret >= Success && n > 0 && *list) {
            res = *list;
            XFreeStringList(list);
        }
    }

    XFree(tp.value);

    return res;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QList<QBuffer *> screen_out;

    QList<QScreen *> screen_list = QGuiApplication::screens();
    for (int i = 0; i < screen_list.count(); i++) {
        // qDebug() << screen_list.at(i)->geometry();

        QPixmap pixmap;
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        screen_list.at(i)->grabWindow(0).save(&buffer, "png");
        screen_out << &buffer;
    }

    auto display = XOpenDisplay(nullptr);
    auto root_window = DefaultRootWindow(display);

    Window root_return, parent_return;
    Window *child_list = nullptr;
    unsigned int child_num = 0;
    XQueryTree(display, root_window, &root_return, &parent_return, &child_list, &child_num);

    int i = 0;
    for (Window win : getTopLevelWindows()) {
        //获取窗口属性
        XWindowAttributes attrs;
        XGetWindowAttributes(display, win, &attrs);
        
        screen_list.at(0)->grabWindow(win).save(getWindowName(win).c_str(),
                                                "png");

        i++;
    }

    XFree(child_list);
    XCloseDisplay(display);
    //    w.show();
    return a.exec();
}
