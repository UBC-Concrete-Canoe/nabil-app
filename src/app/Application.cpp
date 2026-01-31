#include "Application.h"
#include "ui/MainWindow.h"
#include "render/RenderController.h"
#include <iostream>

Application::Application(){
    // Create the instance of the controller
    m_renderController = std::make_unique<Render::RenderController>();
}

// Default destructor is fine, but must be defined here where RenderController is fully known
Application::~Application() = default;


void Application::run()
{

    // --- STEP 1: Initialize the Renderer ---
    // We do this before the UI shows up to ensure the 3D engine is ready
    if (m_renderController->init()) {
        std::cout << "[App] Renderer initialized." << std::endl;
        
        // Create the debug cube in memory
        m_renderController->debugDrawCube();
    } else {
        std::cerr << "[App] ERROR: Could not initialize Renderer!" << std::endl;
    }

    // --- STEP 2: Launch the UI ---
    MainWindow *window = new MainWindow();
    
    // TODO: In the next step (Integration), you will need to pass the renderer 
    // to the window so the window can actually display the view.
    // Example: window->setRenderController(m_renderController.get());

    window->show();
    
}
