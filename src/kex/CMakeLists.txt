cmake_minimum_required(VERSION 3.16)

project(DOOM64 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/kex/")

# Dependencies
find_package(ZLIB REQUIRED)
find_package(PNG  REQUIRED)
find_package(SDL3 QUIET)
find_package(FluidSynth REQUIRED)
find_package(OpenGL REQUIRED)

set(EXTRA_SOURCES)
if(APPLE)
  list(APPEND EXTRA_SOURCES Ext/SDLMain.m)
endif()

if(MSVC)
  list(APPEND EXTRA_SOURCES i_exception.c i_opndir.c i_cpu.c)
else()
  list(APPEND EXTRA_SOURCES i_cpu_posix.c)
endif()

add_executable(DOOM64
  ${EXTRA_SOURCES}
  am_draw.c
  am_map.c
  con_console.c
  con_cvar.c
  d_devstat.c
  d_main.c
  d_net.c
  dgl.c
  f_finale.c
  g_actions.c
  g_game.c
  g_settings.c
  gl_draw.c
  gl_main.c
  gl_texture.c
  i_audio.c
  i_main.c
  i_png.c
  i_system.c
  i_video.c
  i_xinput.c
  in_stuff.c
  info.c
  m_cheat.c
  m_fixed.c
  m_keys.c
  m_menu.c
  m_misc.c
  m_password.c
  m_random.c
  m_shift.c
  p_ceilng.c
  p_doors.c
  p_enemy.c
  p_floor.c
  p_inter.c
  p_lights.c
  p_macros.c
  p_map.c
  p_maputl.c
  p_mobj.c
  p_plats.c
  p_pspr.c
  p_saveg.c
  p_setup.c
  p_sight.c
  p_spec.c
  p_switch.c
  p_telept.c
  p_tick.c
  p_user.c
  psnprntf.c
  r_bsp.c
  r_clipper.c
  r_drawlist.c
  r_lights.c
  r_main.c
  r_scene.c
  r_sky.c
  r_things.c
  r_wipe.c
  s_sound.c
  sc_main.c
  st_stuff.c
  tables.c
  w_file.c
  w_merge.c
  w_wad.c
  wi_stuff.c
  z_zone.c
  Ext/md5.c
  Ext/ChocolateDoom/net_client.c
  Ext/ChocolateDoom/net_common.c
  Ext/ChocolateDoom/net_dedicated.c
  Ext/ChocolateDoom/net_io.c
  Ext/ChocolateDoom/net_loop.c
  Ext/ChocolateDoom/net_packet.c
  Ext/ChocolateDoom/net_query.c
  Ext/ChocolateDoom/net_sdl.c
  Ext/ChocolateDoom/net_server.c
  Ext/ChocolateDoom/net_structrw.c
)

target_compile_definitions(DOOM64 PRIVATE dprintf=_dprintf __cdecl=)

target_include_directories(DOOM64 PRIVATE
  ${CMAKE_SOURCE_DIR}/kex
  ${CMAKE_SOURCE_DIR}/kex/Ext
)

# PNG
if(NOT TARGET PNG::PNG)
  if(PNG_INCLUDE_DIRS)
    target_include_directories(DOOM64 PRIVATE ${PNG_INCLUDE_DIRS})
  elseif(PNG_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${PNG_INCLUDE_DIR})
  endif()
endif()
# FluidSynth
if(TARGET FluidSynth::FluidSynth)
else()
  if(FluidSynth_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${FluidSynth_INCLUDE_DIR})
  endif()
endif()
if(NOT TARGET SDL3::SDL3 AND NOT TARGET SDL2::SDL2)
  if(SDL_INCLUDE_DIRS)
    target_include_directories(DOOM64 PRIVATE ${SDL_INCLUDE_DIRS})
  elseif(SDL_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${SDL_INCLUDE_DIR})
  endif()
endif()

# MSVC
if(MSVC)
  target_compile_definitions(DOOM64 PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET DOOM64 PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
  target_link_options(DOOM64 PRIVATE /SUBSYSTEM:CONSOLE)
else()
  target_compile_options(DOOM64 PRIVATE -Wall -Wextra)
endif()

set(VCS_REV "0")
find_package(Git QUIET)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_VARIABLE VCS_REV OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
endif()
if(VCS_REV STREQUAL "0")
  find_program(SVN_EXECUTABLE svn)
  if(SVN_EXECUTABLE)
    execute_process(COMMAND ${SVN_EXECUTABLE} info --show-item revision
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE VCS_REV OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(NOT VCS_REV)
      set(VCS_REV "0")
    endif()
  endif()
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/revconfig.txt")
  file(READ  "${CMAKE_SOURCE_DIR}/revconfig.txt" _REV_IN)
  string(REPLACE "$WCREV$" "${VCS_REV}" _REV_OUT "${_REV_IN}")
  file(WRITE "${CMAKE_BINARY_DIR}/version.h" "${_REV_OUT}")
  target_include_directories(DOOM64 PRIVATE "${CMAKE_BINARY_DIR}")
endif()

# PNG
set(_PNG_LIB PNG::PNG)
if(NOT TARGET PNG::PNG)
  set(_PNG_LIB ${PNG_LIBRARY})
endif()
# ZLIB
set(_ZLIB_LIB ZLIB::ZLIB)
if(NOT TARGET ZLIB::ZLIB)
  set(_ZLIB_LIB ${ZLIB_LIBRARY})
endif()
# SDL
set(_SDL_LIB "")
if(TARGET SDL3::SDL3)
  set(_SDL_LIB SDL3::SDL3)
else()
  message(FATAL_ERROR "SDL3 not found.")
endif()
# FluidSynth
set(_FLUID_LIB FluidSynth::FluidSynth)
if(NOT TARGET FluidSynth::FluidSynth)
  set(_FLUID_LIB ${FLUIDSYNTH_LIBRARIES})
endif()
# OpenGL
set(_GL_LIB OpenGL::GL)
if(NOT TARGET OpenGL::GL)
  if(DEFINED OPENGL_gl_LIBRARY)
    set(_GL_LIB ${OPENGL_gl_LIBRARY})
  else()
    set(_GL_LIB ${OPENGL_LIBRARIES})
  endif()
endif()

target_link_libraries(DOOM64 PRIVATE
  ${_PNG_LIB}
  ${_ZLIB_LIB}
  ${_SDL_LIB}
  ${_SDLNET_LIB}
  ${_FLUID_LIB}
  ${_GL_LIB}
)

if(UNIX AND NOT APPLE)
  find_library(M_LIB m)
  if(M_LIB)
    target_link_libraries(DOOM64 PRIVATE ${M_LIB})
  endif()
endif()

set_target_properties(DOOM64 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)