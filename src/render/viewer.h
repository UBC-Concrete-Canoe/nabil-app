#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

// Local includes
#include "viewerInteractor.h"

// OpenCascade includes
#include <TopoDS_Shape.hxx>
#include <WNT_Window.hxx>

// Standard includes
#include <vector>

class V3d_Viewer;
class V3d_View;
class AIS_InteractiveContext;
class AIS_ViewController;

//-----------------------------------------------------------------------------

//! Simple 3D viewer.
class Viewer
{
public:

  Viewer(const int left,
         const int top,
         const int width,
         const int height);

public:

  Viewer& operator<<(const TopoDS_Shape& shape)
  {
    this->AddShape(shape);
    return *this;
  }

  void AddShape(const TopoDS_Shape& shape);

  void StartMessageLoop();

private:

  static LRESULT WINAPI
    wndProcProxy(HWND hwnd,
                 UINT message,
                 WPARAM wparam,
                 LPARAM lparam);

  LRESULT CALLBACK
    wndProc(HWND hwnd,
            UINT message,
            WPARAM wparam,
            LPARAM lparam);

  void init(const HANDLE& windowHandle);

/* API-related things */
private:

  std::vector<TopoDS_Shape> m_shapes; //!< Shapes to visualize.

/* OpenCascade's things */
private:

  Handle(V3d_Viewer)             m_viewer;
  Handle(V3d_View)               m_view;
  Handle(AIS_InteractiveContext) m_context;
  Handle(WNT_Window)             m_wntWindow;
  Handle(ViewerInteractor)       m_evtMgr;

/* Lower-level things */
private:

  HINSTANCE m_hInstance; //!< Handle to the instance of the module.
  HWND      m_hWnd;      //!< Handle to the instance of the window.
  bool      m_bQuit;     //!< Indicates whether user want to quit from window.

};
