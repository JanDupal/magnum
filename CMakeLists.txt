#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

# CMake 2.8.8 required for OBJECT library target
cmake_minimum_required(VERSION 2.8.8)
project(Magnum)

include(CMakeDependentOption)

option(TARGET_GLES "Build for OpenGL ES instead of desktop OpenGL" OFF)
cmake_dependent_option(TARGET_GLES2 "Build for OpenGL ES 2" ON "TARGET_GLES" OFF)
cmake_dependent_option(TARGET_DESKTOP_GLES "Build for OpenGL ES on desktop" OFF "TARGET_GLES" OFF)

# Parts of the library
option(WITH_DEBUGTOOLS "Build DebugTools library" ON)
cmake_dependent_option(WITH_MESHTOOLS "Build MeshTools library" ON "NOT WITH_DEBUGTOOLS" ON)
cmake_dependent_option(WITH_PHYSICS "Build Physics library" ON "NOT WITH_DEBUGTOOLS" ON)
cmake_dependent_option(WITH_PRIMITIVES "Builf Primitives library" ON "NOT WITH_DEBUGTOOLS" ON)
cmake_dependent_option(WITH_SCENEGRAPH "Build SceneGraph library" ON "NOT WITH_DEBUGTOOLS;NOT WITH_PHYSICS" ON)
cmake_dependent_option(WITH_SHADERS "Build Shaders library" ON "NOT WITH_DEBUGTOOLS" ON)
option(WITH_TEXT "Build Text library" ON)
cmake_dependent_option(WITH_TEXTURETOOLS "Build TextureTools library" ON "NOT WITH_TEXT" ON)
option(WITH_MAGNUMINFO "Build magnum-info utility" ON)

# Application libraries
if(CORRADE_TARGET_NACL)
    option(WITH_NACLAPPLICATION "Build NaClApplication library" OFF)
else()
    option(WITH_GLXAPPLICATION "Build GlxApplication library" OFF)
    cmake_dependent_option(WITH_WINDOWLESSGLXAPPLICATION "Build WindowlessGlxApplication library" OFF "NOT WITH_MAGNUMINFO" ON)
    cmake_dependent_option(WITH_XEGLAPPLICATION "Build XEglApplication library" OFF "TARGET_GLES" OFF)
    cmake_dependent_option(WITH_GLUTAPPLICATION "Build GlutApplication library" OFF "NOT TARGET_GLES" OFF)
    option(WITH_SDL2APPLICATION "Build Sdl2Application library" OFF)
endif()

option(BUILD_STATIC "Build static libraries (default are shared)" OFF)
cmake_dependent_option(BUILD_STATIC_PIC "Build static libraries with position-independent code" OFF "BUILD_STATIC" OFF)
option(BUILD_TESTS "Build unit tests." OFF)
if(BUILD_TESTS)
    enable_testing()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(Corrade REQUIRED)

# If targeting NaCl or Emscripten, set explicit OpenGL ES 2.0 support
if(CORRADE_TARGET_NACL OR CORRADE_TARGET_EMSCRIPTEN)
    set(TARGET_GLES 1)
    set(TARGET_GLES2 1)

    # Newlib toolchain supports only static linking
    if(CORRADE_TARGET_NACL_NEWLIB)
        set(BUILD_STATIC OFF)
    endif()
endif()

if(BUILD_STATIC)
    set(MAGNUM_BUILD_STATIC 1)
endif()

# Check dependencies
if(NOT TARGET_GLES OR TARGET_DESKTOP_GLES)
    find_package(OpenGL REQUIRED)
elseif(TARGET_GLES2)
    find_package(OpenGLES2 REQUIRED)
else()
    find_package(OpenGLES3 REQUIRED)
endif()
if(NOT TARGET_GLES)
    find_package(GLEW REQUIRED)
endif()

# Configuration variables (saved later to corradeConfigure.h)
if(TARGET_GLES)
    set(MAGNUM_TARGET_GLES 1)
    if(TARGET_GLES2)
        set(MAGNUM_TARGET_GLES2 1)
    else()
        set(MAGNUM_TARGET_GLES3 1)
    endif()
endif()
if(TARGET_DESKTOP_GLES)
    set(MAGNUM_TARGET_DESKTOP_GLES 1)
endif()

if(NOT BUILD_STATIC)
    set(SHARED_OR_STATIC SHARED)
else()
    set(SHARED_OR_STATIC STATIC)
endif()

# Installation paths
include(CorradeLibSuffix)
set(MAGNUM_BINARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(MAGNUM_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(MAGNUM_CMAKE_MODULE_INSTALL_DIR ${CMAKE_ROOT}/Modules)
set(MAGNUM_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Magnum)

add_subdirectory(external)
add_subdirectory(modules)
add_subdirectory(src)
