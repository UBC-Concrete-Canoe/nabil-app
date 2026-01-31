#pragma once

// OpenCascade includes
#include <AIS_ViewController.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TCollection_AsciiString.hxx>

class AIS_InteractiveContext;
class V3d_View;

//! Manages input events.
class ViewerInteractor : public Standard_Transient, public AIS_ViewController
{
public:

  // OCCT RTTI
  DEFINE_STANDARD_RTTI_INLINE(ViewerInteractor, Standard_Transient)

public:

  //! Ctor.
  //! \param[in] view the V3d view instance.
  //! \param[in] ctx  the interactive context.
  ViewerInteractor(const Handle(V3d_View)&               view,
                   const Handle(AIS_InteractiveContext)& ctx);

  //! Dtor.
  virtual ~ViewerInteractor();

public:

  //! Return interactive context.
  const Handle(AIS_InteractiveContext)&
    GetContext() const { return m_ctx; }

  //! Handle mouse button press/release event.
  virtual bool UpdateMouseButtons(const Graphic3d_Vec2i& thePoint,
                                  Aspect_VKeyMouse       theButtons,
                                  Aspect_VKeyFlags       theModifiers,
                                  bool                   theIsEmulated) Standard_OVERRIDE;

  //! Release key.
  virtual void KeyDown(Aspect_VKey theKey,
                       double theTime,
                       double thePressure = 1.0) Standard_OVERRIDE;

  //! Release key.
  virtual void KeyUp(Aspect_VKey theKey,
                     double theTime) Standard_OVERRIDE;

  //! Redraw the View on an Expose Event
  virtual void ProcessExpose();

  //! Handle redraw.
  virtual void handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
                                const Handle(V3d_View)& theView) Standard_OVERRIDE;

  //! Resize View.
  virtual void ProcessConfigure();

  //! Handle KeyPress event.
  void ProcessKeyPress(Aspect_VKey theKey);

private:

  Handle(V3d_View)               m_view; //!< 3D view.
  Handle(AIS_InteractiveContext) m_ctx;  //!< Interactive context.

};
