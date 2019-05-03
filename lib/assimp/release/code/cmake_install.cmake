# Install script for directory: E:/Burdel/Download/assimp-4.1.0/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Program Files/Assimp/libassimp.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Program Files/Assimp/libassimp.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Burdel/Download/mingwx86_64posixseh/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/anim.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/ai_assert.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/camera.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/color4.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/color4.inl"
    "E:/Burdel/Download/assimp-4.1.0/release/code/../include/assimp/config.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/defs.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Defines.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/cfileio.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/light.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/material.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/material.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/matrix3x3.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/matrix3x3.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/matrix4x4.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/matrix4x4.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/mesh.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/postprocess.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/quaternion.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/quaternion.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/scene.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/metadata.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/texture.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/types.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/vector2.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/vector2.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/vector3.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/vector3.inl"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/version.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/cimport.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/importerdesc.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Importer.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/DefaultLogger.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/ProgressHandler.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/IOStream.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/IOSystem.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Logger.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/LogStream.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/NullLogger.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/cexport.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Exporter.hpp"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/DefaultIOStream.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/DefaultIOSystem.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/SceneCombiner.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Compiler/pushpack1.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Compiler/poppack1.h"
    "E:/Burdel/Download/assimp-4.1.0/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

