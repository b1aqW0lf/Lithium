cmake_minimum_required(VERSION 3.5)

project(Lithium LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/app_location.cpp
        src/app_location.h
        src/detect_ffmpeg.h
        src/detect_ffmpeg.cpp
        src/detect_storage.cpp
        src/detect_storage.h
        src/qt5_connection_archive.h
        src/app_version.h
        src/resources.qrc
        src/ffmpeg_commands_list.h
        src/audio_interface.cpp
        src/audio_interface.h
        src/audio_interface.ui
        src/audio_interface_data.h
        src/audio_commands.h
        src/video_ui.cpp
        src/video_ui.h
        src/video_ui.ui
        src/select_source_ui.cpp
        src/select_source_ui.h
        src/select_source_ui.ui
        src/save_as_ui.cpp
        src/save_as_ui.h
        src/save_as_ui.ui
        src/process_mode_ui.cpp
        src/process_mode_ui.h
        src/process_mode_ui.ui
        src/output_display_ui.h
        src/output_display_ui.cpp
        src/output_display_ui.ui
        src/statusbar_ui.h
        src/statusbar_ui.cpp
        src/statusbar_ui.ui
        src/input_probe.h
        src/input_probe.cpp
        src/transcode.h
        src/transcode.cpp
        src/abstract_timer.cpp
        src/abstract_timer.h
        src/encoder_process.h
        src/encoder_process.cpp
        src/file_extension_check.h
        src/file_extension_check.cpp        
        src/input_tree_model.h
        src/input_tree_model.cpp
        src/input_treeview.h
        src/input_treeview.cpp
        src/input_treeview.ui
        src/process_mode_flags.h
        src/lithium_theme.h
        src/lithium_theme.cpp
        src/metadata_checkbox.h
        src/metadata_checkbox.cpp
        src/metadata_checkbox.ui
        src/open_file.h
        src/open_file.cpp
        misc/
        LICENSE
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lithium
        ${PROJECT_SOURCES}
        src/parse_data.h src/parse_data.cpp
    )
else()
    if(ANDROID)
        add_library(Lithium SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Lithium
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Lithium PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Get the current working branch
execute_process(
COMMAND git rev-parse --abbrev-ref HEAD
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_BRANCH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
COMMAND git log -1 --format=%h
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_COMMIT_HASH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest commit's SHA1
execute_process(
COMMAND  git rev-parse --verify HEAD
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE GIT_SHA1
OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}")
add_definitions("-DGIT_BRANCH=${GIT_BRANCH}")
add_definitions("-DGIT_SHA1=${GIT_SHA1}")

configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/misc/version.h
)
