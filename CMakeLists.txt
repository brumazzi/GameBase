cmake_minimum_required(VERSION 3.31.5)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME Game)
set(CMAKE_CXX_FLAGS "-O2 -Wl,-rpath=./lib64 -L./lib64 -rdynamic -g")
set(PKG_CONFIG_LIBS sfml-all yaml-cpp)
set(STATIC_LIBS unistring dl imgui-sfml box2d m)
set(SHARED_LIBS tinyxml2 GL glfw)

set(SOURCE_FILES
    src/effect.cpp
    src/game.cpp
    src/main.cpp
    src/object.cpp
    src/physic.cpp
    src/resource-audio.cpp
    src/resource.cpp
    src/scene.cpp
    src/settings.cpp
    src/splash.cpp
    src/translate.cpp
    src/ui.cpp
    src/utils.cpp
    src/vars.cpp
    src/ui/settings.cpp

    # src/Loaders/string.cpp
)

project(${PROJECT_NAME})
include_directories(./include)
link_directories(./lib64)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# BEGIN FIND DEPENDENCES
find_library(DL_LIBRARY NAMES dl)
if (NOT DL_LIBRARY)
    message(FATAL_ERROR "Library dl not found!")
else()
    message(STATUS "Library dl found: ${DL_LIBRARY}")
endif()

find_library(M_LIBRARY NAMES m)
if (NOT M_LIBRARY)
    message(FATAL_ERROR "Library m not found!")
else()
    message(STATUS "Library m found: ${M_LIBRARY}")
endif()

find_library(UNISTRING_LIBRARY NAMES unistring)
if (NOT UNISTRING_LIBRARY)
    message(FATAL_ERROR "Library libunistring not found!")
else()
    message(STATUS "Library libunistring found: ${UNISTRING_LIBRARY}")
endif()

find_library(BOX2D_LIBRARY NAMES box2d)
if (NOT BOX2D_LIBRARY)
    message(FATAL_ERROR "Library box2d not found!")
else()
    message(STATUS "Library box2d found: ${BOX2D_LIBRARY}")
endif()
# END FIND DEPENDENCES


# BEGIN IMPORT STATIC LIBRARIES
# add_library(unistring STATIC IMPORTED)
# set_property(TARGET unistring PROPERTY IMPORTED_LOCATION "/usr/lib64/libunistring.a")
# END IMPORT STATIC LIBRARIES

find_package(PkgConfig REQUIRED)
pkg_check_modules(EXTERN_LIBS REQUIRED IMPORTED_TARGET ${PKG_CONFIG_LIBS})
link_libraries(${SHARED_LIBS})


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PkgConfig::EXTERN_LIBS ${STATIC_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "build/")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/fonts)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/maps/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/musics)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/sounds)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/textures)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p build/resources/translates)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp src/resources/fonts/*.ttf build/resources/fonts/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp src/resources/translates/*.yaml build/resources/translates/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp src/resources/textures/* build/resources/textures/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp -R src/resources/musics/ build/resources/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp -R src/resources/sounds/ build/resources/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp -R src/resources/gui build/resources/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp -R src/resources/stages build/resources/)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cp -R lib64 build/)
