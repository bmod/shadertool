cmake_minimum_required(VERSION 3.9)
project(shadertool)

# First, let cmake know where the Qt library path is, we go from there.
if (WIN32)
    if (MSVC)
        # Add possible Qt installation paths to the HINTS section
        # The version probably doesn't have to match exactly (5.8.? is probably fine)
        find_path(QT_DIR lib/cmake/Qt5/Qt5Config.cmake
                HINTS
                ${CMAKE_CURRENT_LIST_DIR}/../Qt/5.9.1/msvc2015_64
                )
    else()
        find_path(QT_DIR lib/cmake/Qt5/Qt5Config.cmake
                HINTS
                ${CMAKE_CURRENT_LIST_DIR}/../Qt/5.9.1/mingw53_32
                )
    endif()
endif()
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR})


macro(qt5_copy_dll APP DLL)
    # find the release *.dll file
    get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
    # find the debug *d.dll file
    get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)

    add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}>
            $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}>
            $<TARGET_FILE_DIR:${APP}>)
endmacro()


find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_library(Qt5OpenGL REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE UI_HEADERS src/*.ui)

add_executable(${PROJECT_NAME} ${SOURCES} ${UI_HEADERS} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC src)

qt5_use_modules(${PROJECT_NAME} Core Widgets OpenGL)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::OpenGL gl) # rendertest

qt5_copy_dll(${PROJECT_NAME} Core)
qt5_copy_dll(${PROJECT_NAME} Gui)
qt5_copy_dll(${PROJECT_NAME} Widgets)
qt5_copy_dll(${PROJECT_NAME} OpenGL)

