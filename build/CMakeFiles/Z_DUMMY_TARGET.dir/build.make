# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = C:/msys64/mingw64/bin/cmake.exe

# The command to remove a file.
RM = C:/msys64/mingw64/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/Jan/Projects/MIC/GIT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/Jan/Projects/MIC/GIT/build

# Include any dependencies generated for this target.
include CMakeFiles/Z_DUMMY_TARGET.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Z_DUMMY_TARGET.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Z_DUMMY_TARGET.dir/flags.make

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj: ../lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj: CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj -MF CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj.d -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj -c C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.i"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c > CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.s"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.s

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj: ../lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj -MF CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj.d -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj -c C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.i"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.s"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj: ../lib/nunchuk/examples/src/nunchuk.cpp
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj -MF CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj.d -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj -c C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/examples/src/nunchuk.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.i"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/examples/src/nunchuk.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.s"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/examples/src/nunchuk.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj: ../lib/nunchuk/src/Nunchuk.cpp
CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj -MF CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj.d -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj -c C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/src/Nunchuk.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.i"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/src/Nunchuk.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.s"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/src/Nunchuk.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj: ../src/main.cpp
CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj -MF CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj.d -o CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj -c C:/Users/Jan/Projects/MIC/GIT/src/main.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/Jan/Projects/MIC/GIT/src/main.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/Jan/Projects/MIC/GIT/src/main.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s

# Object files for target Z_DUMMY_TARGET
Z_DUMMY_TARGET_OBJECTS = \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj"

# External object files for target Z_DUMMY_TARGET
Z_DUMMY_TARGET_EXTERNAL_OBJECTS = \
"C:/Users/Jan/Projects/MIC/GIT/lib/nunchuk/build/CMakeFiles/Nunchuk.dir/src/Nunchuk.cpp.obj"

Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdC/CMakeCCompilerId.c.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/build/CMakeFiles/3.23.2/CompilerIdCXX/CMakeCXXCompilerId.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/examples/src/nunchuk.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/nunchuk/src/Nunchuk.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj
Z_DUMMY_TARGET: ../lib/nunchuk/build/CMakeFiles/Nunchuk.dir/src/Nunchuk.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/build.make
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Z_DUMMY_TARGET"
	C:/Users/Jan/.platformio/packages/toolchain-atmelavr/bin/avr-g++.exe -fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++11 -mmcu=atmega328p -Os -Wall -ffunction-sections -fdata-sections -flto -g -mmcu=atmega328p -flto -fuse-linker-plugin -Wl,--gc-sections $(Z_DUMMY_TARGET_OBJECTS) $(Z_DUMMY_TARGET_EXTERNAL_OBJECTS) -o Z_DUMMY_TARGET 

# Rule to build all files generated by this target.
CMakeFiles/Z_DUMMY_TARGET.dir/build: Z_DUMMY_TARGET
.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/build

CMakeFiles/Z_DUMMY_TARGET.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Z_DUMMY_TARGET.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/clean

CMakeFiles/Z_DUMMY_TARGET.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/Jan/Projects/MIC/GIT C:/Users/Jan/Projects/MIC/GIT C:/Users/Jan/Projects/MIC/GIT/build C:/Users/Jan/Projects/MIC/GIT/build C:/Users/Jan/Projects/MIC/GIT/build/CMakeFiles/Z_DUMMY_TARGET.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/depend

