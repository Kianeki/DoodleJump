# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.2.4\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Kiani\CLionProjects\DoodleJump

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DoodleJump.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DoodleJump.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DoodleJump.dir/flags.make

CMakeFiles/DoodleJump.dir/main.cpp.obj: CMakeFiles/DoodleJump.dir/flags.make
CMakeFiles/DoodleJump.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DoodleJump.dir/main.cpp.obj"
	C:\mingw32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\DoodleJump.dir\main.cpp.obj -c C:\Users\Kiani\CLionProjects\DoodleJump\main.cpp

CMakeFiles/DoodleJump.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DoodleJump.dir/main.cpp.i"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Kiani\CLionProjects\DoodleJump\main.cpp > CMakeFiles\DoodleJump.dir\main.cpp.i

CMakeFiles/DoodleJump.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DoodleJump.dir/main.cpp.s"
	C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Kiani\CLionProjects\DoodleJump\main.cpp -o CMakeFiles\DoodleJump.dir\main.cpp.s

# Object files for target DoodleJump
DoodleJump_OBJECTS = \
"CMakeFiles/DoodleJump.dir/main.cpp.obj"

# External object files for target DoodleJump
DoodleJump_EXTERNAL_OBJECTS =

DoodleJump.exe: CMakeFiles/DoodleJump.dir/main.cpp.obj
DoodleJump.exe: CMakeFiles/DoodleJump.dir/build.make
DoodleJump.exe: CMakeFiles/DoodleJump.dir/linklibs.rsp
DoodleJump.exe: CMakeFiles/DoodleJump.dir/objects1.rsp
DoodleJump.exe: CMakeFiles/DoodleJump.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DoodleJump.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\DoodleJump.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DoodleJump.dir/build: DoodleJump.exe

.PHONY : CMakeFiles/DoodleJump.dir/build

CMakeFiles/DoodleJump.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\DoodleJump.dir\cmake_clean.cmake
.PHONY : CMakeFiles/DoodleJump.dir/clean

CMakeFiles/DoodleJump.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kiani\CLionProjects\DoodleJump C:\Users\Kiani\CLionProjects\DoodleJump C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug C:\Users\Kiani\CLionProjects\DoodleJump\cmake-build-debug\CMakeFiles\DoodleJump.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DoodleJump.dir/depend
