# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bicyclette/Programmation/Games/Maze

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bicyclette/Programmation/Games/Maze/build

# Include any dependencies generated for this target.
include CMakeFiles/Maze.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Maze.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Maze.dir/flags.make

CMakeFiles/Maze.dir/src/stb_image.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/stb_image.cpp.o: ../src/stb_image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Maze.dir/src/stb_image.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/stb_image.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/stb_image.cpp

CMakeFiles/Maze.dir/src/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/stb_image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/stb_image.cpp > CMakeFiles/Maze.dir/src/stb_image.cpp.i

CMakeFiles/Maze.dir/src/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/stb_image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/stb_image.cpp -o CMakeFiles/Maze.dir/src/stb_image.cpp.s

CMakeFiles/Maze.dir/src/main.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Maze.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/main.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/main.cpp

CMakeFiles/Maze.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/main.cpp > CMakeFiles/Maze.dir/src/main.cpp.i

CMakeFiles/Maze.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/main.cpp -o CMakeFiles/Maze.dir/src/main.cpp.s

CMakeFiles/Maze.dir/src/window.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/window.cpp.o: ../src/window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Maze.dir/src/window.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/window.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/window.cpp

CMakeFiles/Maze.dir/src/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/window.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/window.cpp > CMakeFiles/Maze.dir/src/window.cpp.i

CMakeFiles/Maze.dir/src/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/window.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/window.cpp -o CMakeFiles/Maze.dir/src/window.cpp.s

CMakeFiles/Maze.dir/src/shader_light.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/shader_light.cpp.o: ../src/shader_light.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Maze.dir/src/shader_light.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/shader_light.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/shader_light.cpp

CMakeFiles/Maze.dir/src/shader_light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/shader_light.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/shader_light.cpp > CMakeFiles/Maze.dir/src/shader_light.cpp.i

CMakeFiles/Maze.dir/src/shader_light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/shader_light.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/shader_light.cpp -o CMakeFiles/Maze.dir/src/shader_light.cpp.s

CMakeFiles/Maze.dir/src/camera.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/camera.cpp.o: ../src/camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Maze.dir/src/camera.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/camera.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/camera.cpp

CMakeFiles/Maze.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/camera.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/camera.cpp > CMakeFiles/Maze.dir/src/camera.cpp.i

CMakeFiles/Maze.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/camera.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/camera.cpp -o CMakeFiles/Maze.dir/src/camera.cpp.s

CMakeFiles/Maze.dir/src/mesh.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/mesh.cpp.o: ../src/mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Maze.dir/src/mesh.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/mesh.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/mesh.cpp

CMakeFiles/Maze.dir/src/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/mesh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/mesh.cpp > CMakeFiles/Maze.dir/src/mesh.cpp.i

CMakeFiles/Maze.dir/src/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/mesh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/mesh.cpp -o CMakeFiles/Maze.dir/src/mesh.cpp.s

CMakeFiles/Maze.dir/src/object.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/object.cpp.o: ../src/object.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Maze.dir/src/object.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/object.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/object.cpp

CMakeFiles/Maze.dir/src/object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/object.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/object.cpp > CMakeFiles/Maze.dir/src/object.cpp.i

CMakeFiles/Maze.dir/src/object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/object.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/object.cpp -o CMakeFiles/Maze.dir/src/object.cpp.s

CMakeFiles/Maze.dir/src/grid_axis.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/grid_axis.cpp.o: ../src/grid_axis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Maze.dir/src/grid_axis.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/grid_axis.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/grid_axis.cpp

CMakeFiles/Maze.dir/src/grid_axis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/grid_axis.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/grid_axis.cpp > CMakeFiles/Maze.dir/src/grid_axis.cpp.i

CMakeFiles/Maze.dir/src/grid_axis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/grid_axis.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/grid_axis.cpp -o CMakeFiles/Maze.dir/src/grid_axis.cpp.s

CMakeFiles/Maze.dir/src/skybox.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/skybox.cpp.o: ../src/skybox.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Maze.dir/src/skybox.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/skybox.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/skybox.cpp

CMakeFiles/Maze.dir/src/skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/skybox.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/skybox.cpp > CMakeFiles/Maze.dir/src/skybox.cpp.i

CMakeFiles/Maze.dir/src/skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/skybox.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/skybox.cpp -o CMakeFiles/Maze.dir/src/skybox.cpp.s

CMakeFiles/Maze.dir/src/scene.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/scene.cpp.o: ../src/scene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Maze.dir/src/scene.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/scene.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/scene.cpp

CMakeFiles/Maze.dir/src/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/scene.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/scene.cpp > CMakeFiles/Maze.dir/src/scene.cpp.i

CMakeFiles/Maze.dir/src/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/scene.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/scene.cpp -o CMakeFiles/Maze.dir/src/scene.cpp.s

CMakeFiles/Maze.dir/src/framebuffer.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/framebuffer.cpp.o: ../src/framebuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Maze.dir/src/framebuffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/framebuffer.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/framebuffer.cpp

CMakeFiles/Maze.dir/src/framebuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/framebuffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/framebuffer.cpp > CMakeFiles/Maze.dir/src/framebuffer.cpp.i

CMakeFiles/Maze.dir/src/framebuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/framebuffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/framebuffer.cpp -o CMakeFiles/Maze.dir/src/framebuffer.cpp.s

CMakeFiles/Maze.dir/src/game.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/game.cpp.o: ../src/game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Maze.dir/src/game.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/game.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/game.cpp

CMakeFiles/Maze.dir/src/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/game.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/game.cpp > CMakeFiles/Maze.dir/src/game.cpp.i

CMakeFiles/Maze.dir/src/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/game.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/game.cpp -o CMakeFiles/Maze.dir/src/game.cpp.s

CMakeFiles/Maze.dir/src/graphics.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/graphics.cpp.o: ../src/graphics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Maze.dir/src/graphics.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/graphics.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/graphics.cpp

CMakeFiles/Maze.dir/src/graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/graphics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/graphics.cpp > CMakeFiles/Maze.dir/src/graphics.cpp.i

CMakeFiles/Maze.dir/src/graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/graphics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/graphics.cpp -o CMakeFiles/Maze.dir/src/graphics.cpp.s

CMakeFiles/Maze.dir/src/helpers.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/helpers.cpp.o: ../src/helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/Maze.dir/src/helpers.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/helpers.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/helpers.cpp

CMakeFiles/Maze.dir/src/helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/helpers.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/helpers.cpp > CMakeFiles/Maze.dir/src/helpers.cpp.i

CMakeFiles/Maze.dir/src/helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/helpers.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/helpers.cpp -o CMakeFiles/Maze.dir/src/helpers.cpp.s

CMakeFiles/Maze.dir/src/imgui.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui.cpp.o: ../src/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/Maze.dir/src/imgui.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui.cpp

CMakeFiles/Maze.dir/src/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui.cpp > CMakeFiles/Maze.dir/src/imgui.cpp.i

CMakeFiles/Maze.dir/src/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui.cpp -o CMakeFiles/Maze.dir/src/imgui.cpp.s

CMakeFiles/Maze.dir/src/imgui_draw.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui_draw.cpp.o: ../src/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/Maze.dir/src/imgui_draw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui_draw.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui_draw.cpp

CMakeFiles/Maze.dir/src/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui_draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui_draw.cpp > CMakeFiles/Maze.dir/src/imgui_draw.cpp.i

CMakeFiles/Maze.dir/src/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui_draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui_draw.cpp -o CMakeFiles/Maze.dir/src/imgui_draw.cpp.s

CMakeFiles/Maze.dir/src/imgui_tables.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui_tables.cpp.o: ../src/imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/Maze.dir/src/imgui_tables.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui_tables.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui_tables.cpp

CMakeFiles/Maze.dir/src/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui_tables.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui_tables.cpp > CMakeFiles/Maze.dir/src/imgui_tables.cpp.i

CMakeFiles/Maze.dir/src/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui_tables.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui_tables.cpp -o CMakeFiles/Maze.dir/src/imgui_tables.cpp.s

CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o: ../src/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui_widgets.cpp

CMakeFiles/Maze.dir/src/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui_widgets.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui_widgets.cpp > CMakeFiles/Maze.dir/src/imgui_widgets.cpp.i

CMakeFiles/Maze.dir/src/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui_widgets.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui_widgets.cpp -o CMakeFiles/Maze.dir/src/imgui_widgets.cpp.s

CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o: ../src/imgui_impl_sdl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_sdl.cpp

CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_sdl.cpp > CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.i

CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_sdl.cpp -o CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.s

CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o: CMakeFiles/Maze.dir/flags.make
CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o: ../src/imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building CXX object CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o -c /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_opengl3.cpp

CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_opengl3.cpp > CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.i

CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bicyclette/Programmation/Games/Maze/src/imgui_impl_opengl3.cpp -o CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.s

# Object files for target Maze
Maze_OBJECTS = \
"CMakeFiles/Maze.dir/src/stb_image.cpp.o" \
"CMakeFiles/Maze.dir/src/main.cpp.o" \
"CMakeFiles/Maze.dir/src/window.cpp.o" \
"CMakeFiles/Maze.dir/src/shader_light.cpp.o" \
"CMakeFiles/Maze.dir/src/camera.cpp.o" \
"CMakeFiles/Maze.dir/src/mesh.cpp.o" \
"CMakeFiles/Maze.dir/src/object.cpp.o" \
"CMakeFiles/Maze.dir/src/grid_axis.cpp.o" \
"CMakeFiles/Maze.dir/src/skybox.cpp.o" \
"CMakeFiles/Maze.dir/src/scene.cpp.o" \
"CMakeFiles/Maze.dir/src/framebuffer.cpp.o" \
"CMakeFiles/Maze.dir/src/game.cpp.o" \
"CMakeFiles/Maze.dir/src/graphics.cpp.o" \
"CMakeFiles/Maze.dir/src/helpers.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui_draw.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui_tables.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o" \
"CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o"

# External object files for target Maze
Maze_EXTERNAL_OBJECTS =

Maze: CMakeFiles/Maze.dir/src/stb_image.cpp.o
Maze: CMakeFiles/Maze.dir/src/main.cpp.o
Maze: CMakeFiles/Maze.dir/src/window.cpp.o
Maze: CMakeFiles/Maze.dir/src/shader_light.cpp.o
Maze: CMakeFiles/Maze.dir/src/camera.cpp.o
Maze: CMakeFiles/Maze.dir/src/mesh.cpp.o
Maze: CMakeFiles/Maze.dir/src/object.cpp.o
Maze: CMakeFiles/Maze.dir/src/grid_axis.cpp.o
Maze: CMakeFiles/Maze.dir/src/skybox.cpp.o
Maze: CMakeFiles/Maze.dir/src/scene.cpp.o
Maze: CMakeFiles/Maze.dir/src/framebuffer.cpp.o
Maze: CMakeFiles/Maze.dir/src/game.cpp.o
Maze: CMakeFiles/Maze.dir/src/graphics.cpp.o
Maze: CMakeFiles/Maze.dir/src/helpers.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui_draw.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui_tables.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui_widgets.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui_impl_sdl.cpp.o
Maze: CMakeFiles/Maze.dir/src/imgui_impl_opengl3.cpp.o
Maze: CMakeFiles/Maze.dir/build.make
Maze: /usr/lib64/libSDL2.so
Maze: /usr/lib64/libGLEW.so
Maze: /usr/lib64/libOpenGL.so
Maze: /usr/lib64/libGLX.so
Maze: /usr/lib64/libGLU.so
Maze: /usr/local/lib64/libassimp.so
Maze: /usr/lib64/libopenal.so
Maze: /usr/lib64/libsndfile.so
Maze: /usr/lib64/libBulletDynamics.so
Maze: /usr/lib64/libBulletCollision.so
Maze: /usr/lib64/libLinearMath.so
Maze: /usr/lib64/libBulletSoftBody.so
Maze: CMakeFiles/Maze.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bicyclette/Programmation/Games/Maze/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Linking CXX executable Maze"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Maze.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Maze.dir/build: Maze

.PHONY : CMakeFiles/Maze.dir/build

CMakeFiles/Maze.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Maze.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Maze.dir/clean

CMakeFiles/Maze.dir/depend:
	cd /home/bicyclette/Programmation/Games/Maze/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bicyclette/Programmation/Games/Maze /home/bicyclette/Programmation/Games/Maze /home/bicyclette/Programmation/Games/Maze/build /home/bicyclette/Programmation/Games/Maze/build /home/bicyclette/Programmation/Games/Maze/build/CMakeFiles/Maze.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Maze.dir/depend
