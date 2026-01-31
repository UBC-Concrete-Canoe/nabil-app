#pragma once
#include <memory> // For std::unique_ptr

// Forward declaration (tells the compiler "this class exists")
// This is better than #include because it keeps compile times fast.
namespace Render { class RenderController; }

class Application
{
public:
    Application();
    ~Application(); // Destructor is required now for unique_ptr
    void run();

    private:
    // Application owns the Renderer
    std::unique_ptr<Render::RenderController> m_renderController;
};
