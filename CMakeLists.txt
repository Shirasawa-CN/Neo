cmake_minimum_required(VERSION 3.5)

project(Neo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Lua/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/QScintilla)

set(PLUGIN_SOURCES
        plugin/pluginloader.h
        plugin/pluginloader.cpp
        plugin/interface.h
)

set(EDITOR_SOURCES 
        editor/editorbase.h
        editor/editorbase.cpp
        editor/editorobject.h
        editor/editorobject.cpp
        editor/programlanguage.h
        editor/editormanager.h
        editor/editormanager.cpp
        editor/editorhighlighter.cpp
        editor/editorhighlighter.h
)

set(SERVICE_SOURCES
        service/userconfig.h
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

        ${EDITOR_SOURCES}
        ${SERVICE_SOURCES}
        ${PLUGIN_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Neo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Neo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Neo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Neo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(LUA_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Lua/src/liblua.a)

if(WIN32)
    message("Build on Windows")
    set(QScintilla_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/QScintilla/src/libqscintilla2_qt6.dll)
endif()

if(UNIX)
    message("Build on UNIX-LIKE")
    set(QSCINTILLA_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/QScintilla/src/libqscintilla2_qt6.so)
endif()

target_link_libraries(Neo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC ${LUA_LIBRARY} ${QSCINTILLA_LIBRARY})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Neo)
endif()
set_target_properties(Neo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Neo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Neo)
endif()
