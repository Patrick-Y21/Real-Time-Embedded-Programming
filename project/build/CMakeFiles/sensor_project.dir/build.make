# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/pi/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/project/build

# Include any dependencies generated for this target.
include CMakeFiles/sensor_project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sensor_project.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sensor_project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sensor_project.dir/flags.make

CMakeFiles/sensor_project.dir/src/main.cpp.o: CMakeFiles/sensor_project.dir/flags.make
CMakeFiles/sensor_project.dir/src/main.cpp.o: /home/pi/project/src/main.cpp
CMakeFiles/sensor_project.dir/src/main.cpp.o: CMakeFiles/sensor_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sensor_project.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sensor_project.dir/src/main.cpp.o -MF CMakeFiles/sensor_project.dir/src/main.cpp.o.d -o CMakeFiles/sensor_project.dir/src/main.cpp.o -c /home/pi/project/src/main.cpp

CMakeFiles/sensor_project.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sensor_project.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/project/src/main.cpp > CMakeFiles/sensor_project.dir/src/main.cpp.i

CMakeFiles/sensor_project.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sensor_project.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/project/src/main.cpp -o CMakeFiles/sensor_project.dir/src/main.cpp.s

CMakeFiles/sensor_project.dir/src/Delay.cpp.o: CMakeFiles/sensor_project.dir/flags.make
CMakeFiles/sensor_project.dir/src/Delay.cpp.o: /home/pi/project/src/Delay.cpp
CMakeFiles/sensor_project.dir/src/Delay.cpp.o: CMakeFiles/sensor_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sensor_project.dir/src/Delay.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sensor_project.dir/src/Delay.cpp.o -MF CMakeFiles/sensor_project.dir/src/Delay.cpp.o.d -o CMakeFiles/sensor_project.dir/src/Delay.cpp.o -c /home/pi/project/src/Delay.cpp

CMakeFiles/sensor_project.dir/src/Delay.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sensor_project.dir/src/Delay.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/project/src/Delay.cpp > CMakeFiles/sensor_project.dir/src/Delay.cpp.i

CMakeFiles/sensor_project.dir/src/Delay.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sensor_project.dir/src/Delay.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/project/src/Delay.cpp -o CMakeFiles/sensor_project.dir/src/Delay.cpp.s

CMakeFiles/sensor_project.dir/src/DHT11.cpp.o: CMakeFiles/sensor_project.dir/flags.make
CMakeFiles/sensor_project.dir/src/DHT11.cpp.o: /home/pi/project/src/DHT11.cpp
CMakeFiles/sensor_project.dir/src/DHT11.cpp.o: CMakeFiles/sensor_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sensor_project.dir/src/DHT11.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sensor_project.dir/src/DHT11.cpp.o -MF CMakeFiles/sensor_project.dir/src/DHT11.cpp.o.d -o CMakeFiles/sensor_project.dir/src/DHT11.cpp.o -c /home/pi/project/src/DHT11.cpp

CMakeFiles/sensor_project.dir/src/DHT11.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sensor_project.dir/src/DHT11.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/project/src/DHT11.cpp > CMakeFiles/sensor_project.dir/src/DHT11.cpp.i

CMakeFiles/sensor_project.dir/src/DHT11.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sensor_project.dir/src/DHT11.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/project/src/DHT11.cpp -o CMakeFiles/sensor_project.dir/src/DHT11.cpp.s

CMakeFiles/sensor_project.dir/src/Key.cpp.o: CMakeFiles/sensor_project.dir/flags.make
CMakeFiles/sensor_project.dir/src/Key.cpp.o: /home/pi/project/src/Key.cpp
CMakeFiles/sensor_project.dir/src/Key.cpp.o: CMakeFiles/sensor_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/sensor_project.dir/src/Key.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sensor_project.dir/src/Key.cpp.o -MF CMakeFiles/sensor_project.dir/src/Key.cpp.o.d -o CMakeFiles/sensor_project.dir/src/Key.cpp.o -c /home/pi/project/src/Key.cpp

CMakeFiles/sensor_project.dir/src/Key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sensor_project.dir/src/Key.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/project/src/Key.cpp > CMakeFiles/sensor_project.dir/src/Key.cpp.i

CMakeFiles/sensor_project.dir/src/Key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sensor_project.dir/src/Key.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/project/src/Key.cpp -o CMakeFiles/sensor_project.dir/src/Key.cpp.s

# Object files for target sensor_project
sensor_project_OBJECTS = \
"CMakeFiles/sensor_project.dir/src/main.cpp.o" \
"CMakeFiles/sensor_project.dir/src/Delay.cpp.o" \
"CMakeFiles/sensor_project.dir/src/DHT11.cpp.o" \
"CMakeFiles/sensor_project.dir/src/Key.cpp.o"

# External object files for target sensor_project
sensor_project_EXTERNAL_OBJECTS =

sensor_project: CMakeFiles/sensor_project.dir/src/main.cpp.o
sensor_project: CMakeFiles/sensor_project.dir/src/Delay.cpp.o
sensor_project: CMakeFiles/sensor_project.dir/src/DHT11.cpp.o
sensor_project: CMakeFiles/sensor_project.dir/src/Key.cpp.o
sensor_project: CMakeFiles/sensor_project.dir/build.make
sensor_project: /usr/lib/aarch64-linux-gnu/libgpiod.so
sensor_project: /usr/lib/aarch64-linux-gnu/libgpiodcxx.so
sensor_project: CMakeFiles/sensor_project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable sensor_project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sensor_project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sensor_project.dir/build: sensor_project
.PHONY : CMakeFiles/sensor_project.dir/build

CMakeFiles/sensor_project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sensor_project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sensor_project.dir/clean

CMakeFiles/sensor_project.dir/depend:
	cd /home/pi/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/project /home/pi/project /home/pi/project/build /home/pi/project/build /home/pi/project/build/CMakeFiles/sensor_project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sensor_project.dir/depend

