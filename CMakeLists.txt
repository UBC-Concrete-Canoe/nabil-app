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
    Qt6::Widgets
    Qt6::OpenGLWidgets

    TKernel
    TKMath
    TKG2d
    TKG3d
    TKGeomBase
    TKBRep
    TKTopAlgo
    TKPrim
    TKService
    TKV3d
    TKOpenGl
    ${VTK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIR} ${VTK_INCLUDE_DIRS})

vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)
