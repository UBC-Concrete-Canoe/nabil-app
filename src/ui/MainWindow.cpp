#include "MainWindow.h"
#include "../render/viewer.h" 

// OCCT Includes for making the box
#include <BRepPrimAPI_MakeBox.hxx>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hull Designer");
    resize(800, 600);

    // 1. Create the Viewer
    Viewer* myViewer = new Viewer(this);
    setCentralWidget(myViewer);

    // 2. Create the Tutorial Box (10x10x20)
    TopoDS_Shape box = BRepPrimAPI_MakeBox(10.0, 10.0, 20.0).Shape();

    // 3. Send it to the viewer
    myViewer->addShape(box);
}