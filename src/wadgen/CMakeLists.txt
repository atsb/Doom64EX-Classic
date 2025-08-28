cmake_minimum_required(VERSION 3.16)

project(WadGen LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)
find_package(PNG  REQUIRED)

add_executable(WadGen
  DeflateN64.c
  Files.c
  Gfx.c
  Level.c
  Mem.c
  Png.c
  MD5.c
  Rom.c
  SndFont.c
  Sound.c
  Sprite.c
  Texture.c
  Wad.c
  WadGen.c
)

if(TARGET PNG::PNG)
else()
  if(PNG_INCLUDE_DIRS)
    target_include_directories(WadGen PRIVATE ${PNG_INCLUDE_DIRS})
  elseif(PNG_INCLUDE_DIR)
    target_include_directories(WadGen PRIVATE ${PNG_INCLUDE_DIR})
  endif()
endif()

if(MSVC)
  target_compile_definitions(WadGen PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
  target_compile_options(WadGen PRIVATE -Wall -Wextra)
endif()

# Link dependencies
set(_PNG_LIB  PNG::PNG)
if(NOT TARGET PNG::PNG)
  set(_PNG_LIB  ${PNG_LIBRARY})
endif()

set(_ZLIB_LIB ZLIB::ZLIB)
if(NOT TARGET ZLIB::ZLIB)
  set(_ZLIB_LIB ${ZLIB_LIBRARY})
endif()

target_link_libraries(WadGen PRIVATE ${_PNG_LIB} ${_ZLIB_LIB})

if(WIN32)
  target_link_libraries(WadGen PRIVATE comctl32)
elseif(UNIX AND NOT APPLE)
  find_library(M_LIB m)
  if(M_LIB)
    target_link_libraries(WadGen PRIVATE ${M_LIB})
  endif()
endif()

set_target_properties(WadGen PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
