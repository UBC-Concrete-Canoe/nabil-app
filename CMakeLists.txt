cmake_minimum_required(VERSION 3.20)
project(coco)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Packages
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenCASCADE REQUIRED)
find_package(VTK REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

# Linkage
target_link_libraries(${PROJECT_NAME} PRIVATE 
    # Qt6
    Qt6::Widgets 
    Qt6::OpenGLWidgets

    # Open Cascade (OCCT) - These must be in this order
    TKernel       # Base
    TKMath        # Math
    TKG2d         # 2D Geometry
    TKG3d         # 3D Geometry
    TKBRep        # Boundary Representation
    TKTopAlgo     # Topological Algorithms
    TKPrim        # <--- THIS FIXES THE SPHERE ERROR
    TKService     # Graphics Services
    TKV3d         # 3D Viewer
    TKIVtk        # The Bridge to VTK

    # VTK
    ${VTK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIR} ${VTK_INCLUDE_DIRS})

vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)
