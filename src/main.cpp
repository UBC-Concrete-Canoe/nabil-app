#include <QApplication>
#include "app/Application.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    // Create and show the window directly
    // MainWindow window;
    // window.show();

    Application application;
    application.run();
    return app.exec();
}
