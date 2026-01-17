cmake_minimum_required(VERSION 3.20)
project(coco)

# set(VCPKG_APPLOCAL_DEPS ON) # Leave off for now, revisit when doing full release packages
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

if(WIN32 AND TARGET coco)
    # 1. More reliable way to find windeployqt in vcpkg
    find_program(WINDEPLOYQT_EXE NAMES windeployqt.exe windeployqt
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/Qt6/bin"
              "${Qt6_DIR}/../../../bin"
        NO_DEFAULT_PATH # Forces it to use our specific vcpkg/Qt paths
    )

    if(WINDEPLOYQT_EXE)
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXE}")

        add_custom_command(TARGET coco POST_BUILD
            # Use the generator expression only for the executable path.
            # CMake handles the quoting for you—don't add \" manually.
            COMMAND $<$<CONFIG:Release>:${WINDEPLOYQT_EXE}>
                    $<$<CONFIG:Release>:--no-compiler-runtime>
                    $<$<CONFIG:Release>:--verbose> $<$<CONFIG:Release>:0>
                    $<$<CONFIG:Release>:$<TARGET_FILE:coco>>
            COMMENT "Running windeployqt for Release build..."
            VERBATIM
        )
    else()
        message(WARNING "windeployqt not found! Qt deployment will fail.")
    endif()
endif()