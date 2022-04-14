#include <node.h>
#include <v8.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <list>
namespace demo
{

    using v8::ArrayBuffer;
    using v8::Context;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Uint8ClampedArray;
    using v8::Value;

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

        for (int i = 0; i < ScreenCount(disp()); ++i)
        {
            Window rootWin = RootWindow(disp(), i);

            int status = XGetWindowProperty(disp(), rootWin, netClList, 0L,
                                            ~0L, false, AnyPropertyType,
                                            &actualType, &format, &num,
                                            &bytes, (uint8_t **)&data);

            if (status != Success)
            {
                printf("Failed getting root "
                       "window properties");
                continue;
            }

            for (unsigned long i = 0; i < num; ++i)
            {
                // printf(reinterpret_cast<const char*>(i));
                res.push_back(data[1]);
            }

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

        if (tp.encoding == XA_STRING)
        {
            res = (char *)tp.value;
        }
        else
        {
            int ret = XmbTextPropertyToTextList(disp(), &tp, &list, &n);

            if (ret >= Success && n > 0 && *list)
            {
                res = *list;
                XFreeStringList(list);
            }
        }

        XFree(tp.value);

        return res;
    }
    // 传入系统类型参数
    void Method(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();

        double p = args[0].As<Number>()->Value();

        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> obj = Object::New(isolate);

        FILE *fp;
        char buffer[4];
        fp = popen("echo $XDG_SESSION_TYPE", "r");
        fgets(buffer, sizeof(buffer), fp);
        if (*buffer == *"x11") // x11
        {
            auto display = XOpenDisplay(nullptr);
            auto root_window = DefaultRootWindow(display);

            Window root_return, parent_return;
            Window *child_list = nullptr;
            unsigned int child_num = 0;
            XQueryTree(display, root_window, &root_return, &parent_return, &child_list, &child_num);

            int i = 0;

            for (Window win : getTopLevelWindows())
            {
                Local<Object> screen = Object::New(isolate);
                //获取窗口属性
                XWindowAttributes attrs;
                XGetWindowAttributes(display, win, &attrs);

                XImage *pImage = XGetImage(display, win, 0, 0, attrs.width, attrs.height, AllPlanes, ZPixmap);
                int ww = attrs.width;
                int hh = attrs.height;
                Local<Number> w = Number::New(isolate, ww);
                Local<Number> h = Number::New(isolate, hh);
                screen->Set(context, String::NewFromUtf8(isolate, "width").ToLocalChecked(), w).FromJust();
                screen->Set(context, String::NewFromUtf8(isolate, "height").ToLocalChecked(), h).FromJust();
                long li = 0;
                int l[attrs.height * attrs.width * 4 + 1];
                // std::list<int>l;
                int x, y;
                unsigned long red_mask = pImage->red_mask;
                unsigned long green_mask = pImage->green_mask;
                unsigned long blue_mask = pImage->blue_mask;
                for (y = 0; y < attrs.height; y++)
                {
                    for (x = 0; x < attrs.width; x++)
                    {
                        unsigned long pixel = XGetPixel(pImage, x, y);
                        unsigned int blue = pixel & blue_mask;
                        unsigned int green = (pixel & green_mask) >> 8;
                        unsigned int red = (pixel & red_mask) >> 16;
                        l[li] = red;
                        l[li + 1] = blue;
                        l[li + 2] = green;
                        l[li + 3] = 255;
                        li += 4;
                        // l.push_back(red);
                        // l.push_back(green);
                        // l.push_back(blue);
                        // l.push_back(255);
                    }
                }
                // int l[] = {255, 255, 1};
                void *temp = l;
                Local<ArrayBuffer> img_b = ArrayBuffer::New(isolate, temp, li);
                // ArrayBuffer::New(isolate, 0);
                // memcpy(img_b->GetContents().Data(), l, 1);
                Local<Uint8ClampedArray> img = Uint8ClampedArray::New(img_b, 0, li);
                // Local<Number> img = Number::New(isolate, i);

                screen->Set(context, String::NewFromUtf8(isolate, "img").ToLocalChecked(), img).FromJust();
                obj->Set(context, i, screen).FromJust();
                // l.clear();
                i++;
            }

            XFree(child_list);
            XCloseDisplay(display);
        }
        else // wayland
        {
        }
        pclose(fp);

        args.GetReturnValue().Set(obj);
    }

    void Initialize(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "capture", Method);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
