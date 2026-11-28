# macOS shared-view highlight rendering

## Cause and fix

OCCT 7.9.3 and 8.0.1 share an `OpenGl_ShaderManager` across the views' GL
contexts. Their Cocoa `OpenGl_Context::MakeCurrent()` activates the native
context but does not call `myShaderManager->SetContext(this)`. The Windows/X11
implementation does. Consequently shader binding can update the first context's
cached active program while color updates use another context's state. This
explains correct selection ownership but highlight colors bleeding across
unselected geometry in the other views.

Upstream sources:

- [8.0.1 Cocoa context activation](https://github.com/Open-Cascade-SAS/OCCT/blob/V8_0_1/src/Visualization/TKOpenGl/OpenGl/OpenGl_Context_1.mm)
- [7.9.3 Cocoa context activation](https://github.com/Open-Cascade-SAS/OCCT/blob/V7_9_3/src/OpenGl/OpenGl_Context_1.mm)
- [7.9.3 context sharing and Windows/X11 activation](https://github.com/Open-Cascade-SAS/OCCT/blob/V7_9_3/src/OpenGl/OpenGl_Context.cxx)

`CocoGraphicDriver` creates a macOS render window whose `Activate()` updates the
shader manager after successful native activation. This covers OCCT's internal
multi-view redraws, including `UpdateCurrentViewer()` and controller flushes;
setting the manager once before a controller flush would not cover all views.
Other platforms use the inherited OCCT implementation. The fix preserves native
context/resource sharing, automatic highlighting, and the existing point drawers.
It requires no Homebrew dependency rebuild.

For a manual check, hover and select points in each viewport, move to empty
space, and verify that only the target changes color. Recheck this behavior when
upgrading OCCT because the workaround depends on its OpenGL window API.
