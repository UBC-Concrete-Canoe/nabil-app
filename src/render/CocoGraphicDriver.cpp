#include "CocoGraphicDriver.h"

#ifdef __APPLE__
#include <OpenGl_ShaderManager.hxx>
#include <OpenGl_Window.hxx>

namespace
{
class CocoGlWindow : public OpenGl_Window
{
public:
	bool Activate() override
	{
		if (!OpenGl_Window::Activate())
		{
			return false;
		}

		// OCCT 7.9.3 and 8.0.1's Cocoa MakeCurrent() omit this update,
		// unlike the Windows/X11 implementation. The manager is shared by
		// all views, but BindProgram() and color updates use per-context state.
		const Handle(OpenGl_Context)& context = GetGlContext();
		context->ShaderManager()->SetContext(context.operator->());
		return true;
	}
};
} // namespace

Handle(OpenGl_Window)
CocoGraphicDriver::CreateRenderWindow(
	const Handle(Aspect_Window)& nativeWindow,
	const Handle(Aspect_Window)& sizeWindow,
	const Aspect_RenderingContext context
)
{
	// Match the base factory's resource sharing; only activation differs.
	const Handle(OpenGl_Context) sharedContext = GetSharedContext();
	Handle(OpenGl_Window) window = new CocoGlWindow();
	window->Init(this, nativeWindow, sizeWindow, context, myCaps, sharedContext);
	return window;
}
#endif
