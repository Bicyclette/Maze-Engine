if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source build detected !")
endif()

cmake_minimum_required(VERSION 3.16)

project(Maze)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
cmake_policy(SET CMP0072 NEW)

set(SRCS
	src/stb_image.cpp
	src/main.cpp
	src/window.cpp
	src/shader_light.cpp
	src/camera.cpp
	src/mesh.cpp
	src/object.cpp
	src/grid_axis.cpp
	src/skybox.cpp
	src/scene.cpp
	src/framebuffer.cpp
	src/game.cpp
	src/graphics.cpp
	src/animatedObject.cpp
	src/joint.cpp
	src/worldPhysics.cpp
	src/character.cpp
	src/IBL.cpp
	src/particle.cpp
	src/audio.cpp
	src/helpers.cpp
	src/imgui.cpp
	src/imgui_draw.cpp
	src/imgui_tables.cpp
	src/imgui_widgets.cpp
	src/imgui_impl_sdl.cpp
	src/imgui_impl_opengl3.cpp)

set(HEADERS
	include/stb_image.h
	include/color.hpp
	include/window.hpp
	include/shader_light.hpp
	include/camera.hpp
	include/mesh.hpp
	include/object.hpp
	include/grid_axis.hpp
	include/skybox.hpp
	include/scene.hpp
	include/framebuffer.hpp
	include/game.hpp
	include/graphics.hpp
	include/animatedObject.hpp
	include/joint.hpp
	include/worldPhysics.hpp
	include/character.hpp
	include/IBL.hpp
	include/particle.hpp
	include/audio.hpp
	include/helpers.hpp
	include/imgui.h
	include/imconfig.h
	include/imgui_internal.h
	include/imstb_textedit.h
	include/imstb_rectpack.h
	include/imstb_truetype.h
	include/imgui_impl_sdl.h
	include/imgui_impl_opengl3.h)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

include_directories(include "external/BULLET/src/")

find_package(OPENMP REQUIRED)
if(OPENMP_FOUND)
	target_link_libraries(${PROJECT_NAME} ${OpenMP_LD_FLAGS})
else()
	message(FATAL_ERROR "OpenMP not found.")
endif()

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
else()
	message(FATAL_ERROR "glew not found")
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENAL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${OPENAL_LIBRARY})
else()
	message(FATAL_ERROR "OpenAL not found")
endif()

find_package(LIBSNDFILE REQUIRED)
if(LIBSNDFILE_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${LIBSNDFILE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${LIBSNDFILE_LIBRARIES})
else()
	message(FATAL_ERROR "libSndFile not found")
endif()

add_subdirectory(external)

target_link_libraries(${PROJECT_NAME} ${GLEW_LIB} SDL2 assimp BulletSoftBody BulletDynamics BulletCollision LinearMath)
