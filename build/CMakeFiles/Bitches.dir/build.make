# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/bjorn/Projects/MIC/Bitches

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bjorn/Projects/MIC/Bitches/build

# Include any dependencies generated for this target.
include CMakeFiles/Bitches.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Bitches.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Bitches.dir/flags.make

CMakeFiles/Bitches.dir/src/main.cpp.obj: CMakeFiles/Bitches.dir/flags.make
CMakeFiles/Bitches.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bjorn/Projects/MIC/Bitches/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Bitches.dir/src/main.cpp.obj"
	/usr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Bitches.dir/src/main.cpp.obj -c /home/bjorn/Projects/MIC/Bitches/src/main.cpp

CMakeFiles/Bitches.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bitches.dir/src/main.cpp.i"
	/usr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bjorn/Projects/MIC/Bitches/src/main.cpp > CMakeFiles/Bitches.dir/src/main.cpp.i

CMakeFiles/Bitches.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bitches.dir/src/main.cpp.s"
	/usr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bjorn/Projects/MIC/Bitches/src/main.cpp -o CMakeFiles/Bitches.dir/src/main.cpp.s

# Object files for target Bitches
Bitches_OBJECTS = \
"CMakeFiles/Bitches.dir/src/main.cpp.obj"

# External object files for target Bitches
Bitches_EXTERNAL_OBJECTS =

Bitches.elf: CMakeFiles/Bitches.dir/src/main.cpp.obj
Bitches.elf: CMakeFiles/Bitches.dir/build.make
Bitches.elf: CMakeFiles/Bitches.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bjorn/Projects/MIC/Bitches/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Bitches.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Bitches.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Bitches.dir/build: Bitches.elf

.PHONY : CMakeFiles/Bitches.dir/build

CMakeFiles/Bitches.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Bitches.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Bitches.dir/clean

CMakeFiles/Bitches.dir/depend:
	cd /home/bjorn/Projects/MIC/Bitches/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bjorn/Projects/MIC/Bitches /home/bjorn/Projects/MIC/Bitches /home/bjorn/Projects/MIC/Bitches/build /home/bjorn/Projects/MIC/Bitches/build /home/bjorn/Projects/MIC/Bitches/build/CMakeFiles/Bitches.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Bitches.dir/depend

