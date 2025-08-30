cmake_minimum_required(VERSION 3.16)

project(DOOM64)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
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
  g_demo.c
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
)

target_include_directories(DOOM64 PRIVATE
  ${CMAKE_SOURCE_DIR}/kex
  ${CMAKE_SOURCE_DIR}/kex/Ext
)

if(NOT TARGET PNG::PNG)
  if(PNG_INCLUDE_DIRS)
    target_include_directories(DOOM64 PRIVATE ${PNG_INCLUDE_DIRS})
  elseif(PNG_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${PNG_INCLUDE_DIR})
  endif()
endif()

if(NOT TARGET SDL3::SDL3 AND NOT TARGET SDL2::SDL2)
  if(SDL_INCLUDE_DIRS)
    target_include_directories(DOOM64 PRIVATE ${SDL_INCLUDE_DIRS})
  elseif(SDL_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${SDL_INCLUDE_DIR})
  endif()
endif()

set(_PNG_LIB PNG::PNG)
if(NOT TARGET PNG::PNG)
  set(_PNG_LIB ${PNG_LIBRARY})
endif()

set(_ZLIB_LIB ZLIB::ZLIB)
if(NOT TARGET ZLIB::ZLIB)
  set(_ZLIB_LIB ${ZLIB_LIBRARY})
endif()

set(_SDL_LIB "")
if(TARGET SDL3::SDL3)
  set(_SDL_LIB SDL3::SDL3)
else()
  message(FATAL_ERROR "SDL3 not found.")
endif()
# FluidSynth (match OpenGL-style pattern)
set(_FLUID_LIB FluidSynth::FluidSynth)
if(NOT TARGET FluidSynth::FluidSynth)
  if(DEFINED FLUIDSYNTH_LIBRARY)
    set(_FLUID_LIB ${FLUIDSYNTH_LIBRARY})
  elseif(DEFINED FLUIDSYNTH_LIBRARIES)
    set(_FLUID_LIB ${FLUIDSYNTH_LIBRARIES})
  else()
    find_library(FLUIDSYNTH_LIBRARY NAMES fluidsynth)
    if(FLUIDSYNTH_LIBRARY)
      set(_FLUID_LIB ${FLUIDSYNTH_LIBRARY})
    else()
      message(FATAL_ERROR "FluidSynth not found.")
    endif()
  endif()
endif()

# Headers for whichever path we took
if(TARGET FluidSynth::FluidSynth)
  get_target_property(_fs_inc FluidSynth::FluidSynth INTERFACE_INCLUDE_DIRECTORIES)
  if(_fs_inc)
    target_include_directories(DOOM64 PRIVATE ${_fs_inc})
  endif()
else()
  if(DEFINED FLUIDSYNTH_INCLUDE_DIRS)
    target_include_directories(DOOM64 PRIVATE ${FLUIDSYNTH_INCLUDE_DIRS})
  elseif(DEFINED FLUIDSYNTH_INCLUDE_DIR)
    target_include_directories(DOOM64 PRIVATE ${FLUIDSYNTH_INCLUDE_DIR})
  else()
    find_path(FLUIDSYNTH_INCLUDE_DIR fluidsynth.h PATH_SUFFIXES fluidsynth)
    if(FLUIDSYNTH_INCLUDE_DIR)
      target_include_directories(DOOM64 PRIVATE ${FLUIDSYNTH_INCLUDE_DIR})
    endif()
  endif()
endif()

set(FLUIDSYNTH_RELEASE_LIB "")
set(FLUIDSYNTH_DEBUG_LIB "")
if(TARGET FluidSynth::FluidSynth)
  get_target_property(FLUIDSYNTH_RELEASE_LIB FluidSynth::FluidSynth IMPORTED_LOCATION_RELEASE)
  get_target_property(FLUIDSYNTH_DEBUG_LIB   FluidSynth::FluidSynth IMPORTED_LOCATION_DEBUG)
  if(NOT FLUIDSYNTH_RELEASE_LIB)
    get_target_property(FLUIDSYNTH_RELEASE_LIB FluidSynth::FluidSynth IMPORTED_LOCATION)
  endif()
  if(NOT FLUIDSYNTH_DEBUG_LIB)
    set(FLUIDSYNTH_DEBUG_LIB ${FLUIDSYNTH_RELEASE_LIB})
  endif()
else()
  if(DEFINED FLUIDSYNTH_LIBRARY)
    set(FLUIDSYNTH_RELEASE_LIB ${FLUIDSYNTH_LIBRARY})
    set(FLUIDSYNTH_DEBUG_LIB   ${FLUIDSYNTH_LIBRARY})
  elseif(DEFINED FLUIDSYNTH_LIBRARIES)
    list(GET FLUIDSYNTH_LIBRARIES 0 FLUIDSYNTH_RELEASE_LIB)
    set(FLUIDSYNTH_DEBUG_LIB ${FLUIDSYNTH_RELEASE_LIB})
  endif()
endif()

target_compile_definitions(DOOM64 PRIVATE HAVE_FLUIDSYNTH=1)

set(_GL_LIB OpenGL::GL)
if(NOT TARGET OpenGL::GL)
  if(DEFINED OPENGL_gl_LIBRARY)
    set(_GL_LIB ${OPENGL_gl_LIBRARY})
  else()
    set(_GL_LIB ${OPENGL_LIBRARIES})
  endif()
endif()

set(_GLU_LIB OpenGL::GLU)
if(NOT TARGET OpenGL::GLU)
  if(DEFINED OPENGL_glu_LIBRARY)
    set(_GLU_LIB ${OPENGL_glu_LIBRARY})
  elseif(DEFINED OPENGL_GLU_LIBRARY)
    set(_GLU_LIB ${OPENGL_GLU_LIBRARY})
  else()
    find_library(OPENGL_glu_LIBRARY NAMES GLU glu)
    if(OPENGL_glu_LIBRARY)
      set(_GLU_LIB ${OPENGL_glu_LIBRARY})
    else()
      set(_GLU_LIB "")
    endif()
  endif()
endif()

target_link_libraries(DOOM64 PRIVATE
  ${_PNG_LIB}
  ${_ZLIB_LIB}
  ${_SDL_LIB}
  ${_FLUID_LIB}
  ${_GL_LIB}
  ${_GLU_LIB}
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
