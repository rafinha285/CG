cmake_minimum_required(VERSION 3.16)

project(CG VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Adiciona a pasta 'src' ao caminho de inclusão.
# Isso permite que você faça #include "Window.h" ao invés de #include "src/Window.h" no código.
include_directories(src)

# Removi o caminho antigo do Window.h daqui, pois estava causando erro
set(TS_FILES CG_pt_BR.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
        GraphicsFrame.cpp
        GraphicsFrame.h

        # --- Arquivos nas subpastas de SRC ---
        # 1. Vector
        src/vector/Vector2D.h
        src/vector/Vector3D.h

        # 2. Window (assumindo que Window.h é 2D e Window3D.h é 3D)
        src/window/Window.h
        src/window/Window3D.h

        # 3. Viewport
        src/viewport/Viewport.h

        # 4. Matrix
        src/matrix/Matrix3x3.h
        src/matrix/Matrix4x4.h

        # 5. Shapes
        src/shapes/Shape.h
        src/shapes/Point.h
        src/shapes/Line.h
        src/shapes/Square.h
        src/shapes/Polygon.h
        src/shapes/Shape3D.h
        src/shapes/Point3D.h
        src/shapes/Line3D.h
        src/shapes/Polygon3D.h

        # 6. ObjReader
        src/objreader/ObjLoader.h
        src/objreader/ObjLoader.cpp
        src/Camera.h
        src/vector/Vector3D.cpp # Não se esqueça do .cpp!
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CG
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(CG SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(CG
                ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(CG PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CG)
endif()

set_target_properties(CG PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CG
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CG)
endif()