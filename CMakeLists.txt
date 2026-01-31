cmake_minimum_required(VERSION 3.20)
project(coco)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Packages
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenCASCADE REQUIRED)
find_package(VTK REQUIRED)



add_executable(${PROJECT_NAME})

target_sources(coco PRIVATE
    src/main.cpp

    src/app/Application.cpp

    src/ui/MainWindow.cpp
    src/ui/OcctWidget.cpp

    src/core/HullModel.cpp
    src/render/OcctViewport.cpp
)

target_include_directories(coco PRIVATE src)

# --- macOS Specific Configuration ---
if(APPLE)
    # 1. Force main.cpp to be treated as Objective-C++ to support NSView* casts
    set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    
    # 2. Link AppKit (for windowing) and OpenGL (for rendering)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework AppKit" "-framework OpenGL")
endif()

# --- General Linkage ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets

    # OCCT Libraries
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
    
    # VTK Libraries
    ${VTK_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIR} ${VTK_INCLUDE_DIRS})

# VTK Initialization
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

# --- Windows Deployment ---
if(WIN32 AND TARGET ${PROJECT_NAME})
    find_program(WINDEPLOYQT_EXE NAMES windeployqt.exe windeployqt
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/Qt6/bin"
              "${Qt6_DIR}/../../../bin"
        NO_DEFAULT_PATH
    )

    if(WINDEPLOYQT_EXE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND $<$<CONFIG:Release>:${WINDEPLOYQT_EXE}>
                    $<$<CONFIG:Release>:--no-compiler-runtime>
                    $<$<CONFIG:Release>:--verbose> $<$<CONFIG:Release>:0>
                    $<$<CONFIG:Release>:$<TARGET_FILE:${PROJECT_NAME}>>
            COMMENT "Running windeployqt for Release build..."
            VERBATIM
        )
    endif()
endif()