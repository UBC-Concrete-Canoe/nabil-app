#include <QApplication>
#include "app/Application.h"
#include "ui/MainWindow.h"

// Linux specific
#ifdef Q_OS_LINUX
#include <cstdlib>
#endif

int main(int argc, char *argv[])
{

    // OpenCascade does not work under wayland, need to force X11/XWayland
#ifdef Q_OS_LINUX
    setenv("QT_QPA_PLATFORM", "xcb", 1);
#endif

    QApplication app(argc, argv);
    Application application;
    application.run();
    return app.exec();
}
