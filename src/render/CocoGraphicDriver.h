#pragma once

#include <OpenGl_GraphicDriver.hxx>

// Keep OCCT's shared shader manager in sync with the native macOS GL context.
class CocoGraphicDriver : public OpenGl_GraphicDriver
{
public:
	using OpenGl_GraphicDriver::OpenGl_GraphicDriver;

#ifdef __APPLE__
	Handle(OpenGl_Window) CreateRenderWindow(
		const Handle(Aspect_Window)& nativeWindow,
		const Handle(Aspect_Window)& sizeWindow,
		const Aspect_RenderingContext context
	) override;
#endif
};
