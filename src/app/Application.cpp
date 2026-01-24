#include "Application.h"
#include "ui/MainWindow.h"

Application::Application() = default;

void Application::run()
{
    MainWindow *window = new MainWindow();
    window->show();
}
