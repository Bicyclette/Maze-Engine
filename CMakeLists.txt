cmake_minimum_required(VERSION 3.16)

project(Maze)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
cmake_policy(SET CMP0072 NEW)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS} "-g")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS} "-O3")
set(CMAKE_VERBOSE_MAKEFILE ON)

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

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "sdl2 not found")
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
	message(FATAL_ERROR "OpenGL not found")
endif()

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
	target_link_libraries(${PROJECT_NAME} ${OpenMP_LD_FLAGS})
	target_link_libraries(${PROJECT_NAME} -fopenmp)
else()
	message(FATAL_ERROR "OpenMP not found")
endif()

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
else()
	message(FATAL_ERROR "Assimp not found")
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

find_package(BULLET REQUIRED)
if(BULLET_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "Bullet not found")
endif()
