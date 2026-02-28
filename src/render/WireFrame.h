#pragma once

//OCCT
#include <AIS_InteractiveContext.hxx>
#include <AIS_Polyline.hxx>
#include <Standard_Handle.hxx>

//not sure if we need these
#include <vector>
#include <memory>

/**
 * @brief Manages wireframe rendering of control points and hull lines.
 *
 * Provides an interface for adding control points and hull lines as
 * AIS_Polyline objects to the OCCT interactive context. Handles creation,
 * display, and removal of these wireframe elements.
 */

class HullModel; //to avoid dependancy




