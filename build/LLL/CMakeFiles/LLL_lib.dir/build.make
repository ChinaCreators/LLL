# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/creatorlxd/Project/LLL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/creatorlxd/Project/LLL/build

# Include any dependencies generated for this target.
include LLL/CMakeFiles/LLL_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include LLL/CMakeFiles/LLL_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include LLL/CMakeFiles/LLL_lib.dir/progress.make

# Include the compile flags for this target's objects.
include LLL/CMakeFiles/LLL_lib.dir/flags.make

LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o: LLL/CMakeFiles/LLL_lib.dir/flags.make
LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o: ../LLL/LLL.cpp
LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o: LLL/CMakeFiles/LLL_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o"
	cd /Users/creatorlxd/Project/LLL/build/LLL && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o -MF CMakeFiles/LLL_lib.dir/LLL.cpp.o.d -o CMakeFiles/LLL_lib.dir/LLL.cpp.o -c /Users/creatorlxd/Project/LLL/LLL/LLL.cpp

LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LLL_lib.dir/LLL.cpp.i"
	cd /Users/creatorlxd/Project/LLL/build/LLL && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/creatorlxd/Project/LLL/LLL/LLL.cpp > CMakeFiles/LLL_lib.dir/LLL.cpp.i

LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LLL_lib.dir/LLL.cpp.s"
	cd /Users/creatorlxd/Project/LLL/build/LLL && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/creatorlxd/Project/LLL/LLL/LLL.cpp -o CMakeFiles/LLL_lib.dir/LLL.cpp.s

# Object files for target LLL_lib
LLL_lib_OBJECTS = \
"CMakeFiles/LLL_lib.dir/LLL.cpp.o"

# External object files for target LLL_lib
LLL_lib_EXTERNAL_OBJECTS =

LLL/libLLL_lib.a: LLL/CMakeFiles/LLL_lib.dir/LLL.cpp.o
LLL/libLLL_lib.a: LLL/CMakeFiles/LLL_lib.dir/build.make
LLL/libLLL_lib.a: LLL/CMakeFiles/LLL_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLLL_lib.a"
	cd /Users/creatorlxd/Project/LLL/build/LLL && $(CMAKE_COMMAND) -P CMakeFiles/LLL_lib.dir/cmake_clean_target.cmake
	cd /Users/creatorlxd/Project/LLL/build/LLL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LLL_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
LLL/CMakeFiles/LLL_lib.dir/build: LLL/libLLL_lib.a
.PHONY : LLL/CMakeFiles/LLL_lib.dir/build

LLL/CMakeFiles/LLL_lib.dir/clean:
	cd /Users/creatorlxd/Project/LLL/build/LLL && $(CMAKE_COMMAND) -P CMakeFiles/LLL_lib.dir/cmake_clean.cmake
.PHONY : LLL/CMakeFiles/LLL_lib.dir/clean

LLL/CMakeFiles/LLL_lib.dir/depend:
	cd /Users/creatorlxd/Project/LLL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/creatorlxd/Project/LLL /Users/creatorlxd/Project/LLL/LLL /Users/creatorlxd/Project/LLL/build /Users/creatorlxd/Project/LLL/build/LLL /Users/creatorlxd/Project/LLL/build/LLL/CMakeFiles/LLL_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : LLL/CMakeFiles/LLL_lib.dir/depend

