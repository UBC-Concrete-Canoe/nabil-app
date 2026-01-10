#include <QApplication>
#include <QMainWindow>
#include <QVTKOpenGLNativeWidget.h>

// OCCT Includes: Geometry Creation
#include <BRepPrimAPI_MakeSphere.hxx>
#include <TopoDS_Shape.hxx>

// VTK Includes: Rendering Pipeline
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>

// OCCT to VTK Bridge (This library is provided by the TKIVtk toolkit)
#include <IVtkTools_ShapeDataSource.hxx>

int main(int argc, char *argv[])
{
    // 1. Initialize Qt Application
    QApplication app(argc, argv);

    // 2. Create OCCT Geometry (A simple sphere)
    Standard_Real radius = 50.0;
    TopoDS_Shape occtSphere = BRepPrimAPI_MakeSphere(radius).Shape();

    // 3. Setup the VTK Pipeline
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // 4. Use the OCCT-VTK Bridge (The most important part)
    auto shapeSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
    shapeSource->SetShape(new IVtkOCC_Shape(occtSphere));

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(shapeSource->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.1, 0.8, 0.3); // Emerald Green

    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1); // Dark Charcoal

    // 5. Create the Qt Wrapper Widget
    QMainWindow mainWindow;
    auto *vtkWidget = new QVTKOpenGLNativeWidget(&mainWindow);
    vtkWidget->setRenderWindow(renderWindow);

    mainWindow.setCentralWidget(vtkWidget);
    mainWindow.resize(1024, 768);
    mainWindow.setWindowTitle("Coco Project: OCCT + VTK + Qt6 Verification");
    mainWindow.show();

    // 6. Final Console Confirmation
    printf("--- Verification Scene Initialized ---\n");
    printf("OCCT Sphere generated and bridged to VTK pipeline.\n");

    return app.exec();
}