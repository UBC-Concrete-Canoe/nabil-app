#include <QApplication>
#include "app/Application.h"


//added by honey
#include "render/RenderController.h"
#include "render/viewer.h" 
#include "render/viewerInteractor.h" 
#include <BRepTools.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include "ui/MainWindow.h"



int main(int argc, char *argv[])
{
    
    QApplication app(argc, argv);

    // Create and show the window directly
    MainWindow window;
    window.show();

    //Application application;
    //application.run();
    return app.exec();
    

}
