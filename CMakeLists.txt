cmake_minimum_required(VERSION 3.5)

project(DnDWinBugTestCase LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    find_package(PkgConfig)
    pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
    include_directories(
            ${GLIB_INCLUDE_DIRS}
            ${GSTREAMER_INCLUDE_DIRS}
    )
    link_directories(
            ${GLIB_LIBRARY_DIRS}
            ${GSTREAMER_LIBRARY_DIRS}
    )
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(GST_BASE_PATH "C:/gstreamer/1.0/msvc_x86")
        set(GST_BASE_PATH_NATIVE "C:\\gstreamer\\1.0\\msvc_x86")
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(GST_BASE_PATH "C:/gstreamer/1.0/msvc_x86_64")
        set(GST_BASE_PATH_NATIVE "C:\\gstreamer\\1.0\\msvc_x86_64")
    endif ()
    message("Using GST_BASE_PATH: ${GST_BASE_PATH}")
    message("Using GST_BASE_PATH_NATIVE: ${GST_BASE_PATH_NATIVE}")
    include_directories(
            ${GST_BASE_PATH}/include/gstreamer-1.0
            ${GST_BASE_PATH}/include/glib-2.0
            ${GST_BASE_PATH}/lib/glib-2.0/include
            ${GST_BASE_PATH}/include/glib-2.0/gobject
    )
    link_directories(
            "${GST_BASE_PATH}/lib"
    )
    link_libraries(
            gstreamer-1.0
            glib-2.0
            gobject-2.0
            gstbase-1.0
            gstapp-1.0
            gstaudio-1.0
            gstpbutils-1.0
            gstcontroller-1.0
            gstvideo-1.0
            winmm
    )
endif ()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

    add_executable(DnDWinBugTestCase
            main.cpp
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            )

target_link_libraries(DnDWinBugTestCase PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GSTREAMER_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (DEPLOY_DEPS)
        get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
        execute_process(
                COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
                RESULT_VARIABLE return_code
                OUTPUT_VARIABLE qt5_install_prefix
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        install(DIRECTORY "${GST_BASE_PATH}/bin/" DESTINATION "bin")
        if (qt5_install_prefix)
            find_program(WINDEPLOYQT windeployqt HINTS ${qt5_install_prefix} PATH_SUFFIXES bin)
            if (WINDEPLOYQT)
                add_custom_command(
                        TARGET DnDWinBugTestCase
                        COMMAND "${WINDEPLOYQT}"
                        ARGS "${CMAKE_BINARY_DIR}\\DnDWinBugTestCase.exe"
                )
            else ()
                message("Can't find windeployqt")
            endif ()
        else ()
            message("QT_INSTALL_DIR not defined")
        endif ()
        add_custom_command(
                TARGET DnDWinBugTestCase
                COMMAND "xcopy"
                ARGS "${GST_BASE_PATH_NATIVE}\\*" "/s" "/y"
                COMMAND "xcopy"
                ARGS "${GST_BASE_PATH_NATIVE}\\bin\\*.dll" "/y"
        )
    endif ()
endif()