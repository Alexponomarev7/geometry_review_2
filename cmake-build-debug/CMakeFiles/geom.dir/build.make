# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lexolordan/Geom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lexolordan/Geom/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/geom.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/geom.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/geom.dir/flags.make

CMakeFiles/geom.dir/main.cpp.o: CMakeFiles/geom.dir/flags.make
CMakeFiles/geom.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lexolordan/Geom/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/geom.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/geom.dir/main.cpp.o -c /Users/lexolordan/Geom/main.cpp

CMakeFiles/geom.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geom.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lexolordan/Geom/main.cpp > CMakeFiles/geom.dir/main.cpp.i

CMakeFiles/geom.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geom.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lexolordan/Geom/main.cpp -o CMakeFiles/geom.dir/main.cpp.s

CMakeFiles/geom.dir/library/geometry.cpp.o: CMakeFiles/geom.dir/flags.make
CMakeFiles/geom.dir/library/geometry.cpp.o: ../library/geometry.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lexolordan/Geom/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/geom.dir/library/geometry.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/geom.dir/library/geometry.cpp.o -c /Users/lexolordan/Geom/library/geometry.cpp

CMakeFiles/geom.dir/library/geometry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geom.dir/library/geometry.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lexolordan/Geom/library/geometry.cpp > CMakeFiles/geom.dir/library/geometry.cpp.i

CMakeFiles/geom.dir/library/geometry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geom.dir/library/geometry.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lexolordan/Geom/library/geometry.cpp -o CMakeFiles/geom.dir/library/geometry.cpp.s

# Object files for target geom
geom_OBJECTS = \
"CMakeFiles/geom.dir/main.cpp.o" \
"CMakeFiles/geom.dir/library/geometry.cpp.o"

# External object files for target geom
geom_EXTERNAL_OBJECTS =

geom: CMakeFiles/geom.dir/main.cpp.o
geom: CMakeFiles/geom.dir/library/geometry.cpp.o
geom: CMakeFiles/geom.dir/build.make
geom: CMakeFiles/geom.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lexolordan/Geom/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable geom"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/geom.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/geom.dir/build: geom

.PHONY : CMakeFiles/geom.dir/build

CMakeFiles/geom.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/geom.dir/cmake_clean.cmake
.PHONY : CMakeFiles/geom.dir/clean

CMakeFiles/geom.dir/depend:
	cd /Users/lexolordan/Geom/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lexolordan/Geom /Users/lexolordan/Geom /Users/lexolordan/Geom/cmake-build-debug /Users/lexolordan/Geom/cmake-build-debug /Users/lexolordan/Geom/cmake-build-debug/CMakeFiles/geom.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/geom.dir/depend

