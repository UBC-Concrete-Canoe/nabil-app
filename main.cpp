// VTK SOLO TEST
// #include <vtkSmartPointer.h>
// #include <vtkCubeSource.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>

// int main()
// {
//     // Create a cube
//     auto cube = vtkSmartPointer<vtkCubeSource>::New();
//     cube->Update();

//     // Mapper
//     auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     mapper->SetInputConnection(cube->GetOutputPort());

//     // Actor
//     auto actor = vtkSmartPointer<vtkActor>::New();
//     actor->SetMapper(mapper);

//     // Renderer
//     auto renderer = vtkSmartPointer<vtkRenderer>::New();
//     renderer->AddActor(actor);
//     renderer->SetBackground(0.1, 0.2, 0.3); // Dark blue-ish

//     // Render Window
//     auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//     renderWindow->AddRenderer(renderer);
//     renderWindow->SetWindowName("VTK Test");

//     // Interactor
//     auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//     interactor->SetRenderWindow(renderWindow);

//     renderWindow->Render();
//     interactor->Start();

//     return 0;
// }

// QT SOLO TEST
// #include <QApplication>
// #include <QMainWindow>
// #include <QWidget>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QPushButton>
// #include <QLineEdit>
// #include <QMenuBar>
// #include <QStatusBar>

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     QMainWindow window;
//     window.setWindowTitle("Qt Widget Test");
//     window.resize(600, 400);

//     // ----- Central widget + layout -----
//     QWidget *central = new QWidget(&window);
//     QVBoxLayout *mainLayout = new QVBoxLayout(central);

//     QLabel *label = new QLabel("Enter text and click the button:");
//     QLineEdit *lineEdit = new QLineEdit();
//     QPushButton *button = new QPushButton("Update label");

//     mainLayout->addWidget(label);
//     mainLayout->addWidget(lineEdit);
//     mainLayout->addWidget(button);

//     window.setCentralWidget(central);

//     // ----- Menu bar -----
//     QMenu *fileMenu = window.menuBar()->addMenu("&File");
//     QAction *exitAction = fileMenu->addAction("Exit");
//     QObject::connect(exitAction, &QAction::triggered, &app, &QApplication::quit);

//     // ----- Status bar -----
//     window.statusBar()->showMessage("Ready");

//     // ----- Signal / slot -----
//     QObject::connect(button, &QPushButton::clicked, [&]()
//                      {
//         label->setText(lineEdit->text().isEmpty()
//                            ? "No text entered"
//                            : lineEdit->text());
//         window.statusBar()->showMessage("Label updated", 2000); });

//     window.show();
//     return app.exec();
// }

// OCCT SOLO TEST

// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
// #include <windows.h>

// #include <AIS_Shape.hxx>
// #include <AIS_InteractiveContext.hxx>
// #include <BRepPrimAPI_MakeBox.hxx>

// #include <OpenGl_GraphicDriver.hxx>
// #include <V3d_Viewer.hxx>
// #include <V3d_View.hxx>

// #include <Aspect_DisplayConnection.hxx>
// #include <WNT_Window.hxx>

// int main()
// {
//     HINSTANCE hInstance = GetModuleHandleW(nullptr);

//     WNDCLASSW wc = {};
//     wc.lpfnWndProc = DefWindowProcW;
//     wc.hInstance = hInstance;
//     wc.lpszClassName = L"OCCTWindowClass";
//     RegisterClassW(&wc);

//     HWND hwnd = CreateWindowW(
//         wc.lpszClassName,
//         L"OCCT Test",
//         WS_OVERLAPPEDWINDOW,
//         CW_USEDEFAULT, CW_USEDEFAULT,
//         800, 600,
//         nullptr, nullptr,
//         hInstance,
//         nullptr);

//     ShowWindow(hwnd, SW_SHOW);

//     Handle(Aspect_DisplayConnection) display =
//         new Aspect_DisplayConnection();

//     Handle(OpenGl_GraphicDriver) driver =
//         new OpenGl_GraphicDriver(display);

//     Handle(V3d_Viewer) viewer =
//         new V3d_Viewer(driver);

//     viewer->SetDefaultLights();
//     viewer->SetLightOn();

//     Handle(AIS_InteractiveContext) context =
//         new AIS_InteractiveContext(viewer);

//     Handle(WNT_Window) window =
//         new WNT_Window(hwnd);

//     Handle(V3d_View) view = viewer->CreateView();
//     view->SetWindow(window);

//     if (!window->IsMapped())
//         window->Map();

//     TopoDS_Shape box =
//         BRepPrimAPI_MakeBox(100, 100, 100).Shape();

//     Handle(AIS_Shape) aisBox =
//         new AIS_Shape(box);

//     context->Display(aisBox, Standard_True);
//     view->FitAll();
//     view->Redraw();

//     MSG msg;
//     while (GetMessageW(&msg, nullptr, 0, 0))
//     {
//         TranslateMessage(&msg);
//         DispatchMessageW(&msg);
//     }

//     return 0;
// }
