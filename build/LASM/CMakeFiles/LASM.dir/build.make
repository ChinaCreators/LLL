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
include LASM/CMakeFiles/LASM.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include LASM/CMakeFiles/LASM.dir/compiler_depend.make

# Include the progress variables for this target.
include LASM/CMakeFiles/LASM.dir/progress.make

# Include the compile flags for this target's objects.
include LASM/CMakeFiles/LASM.dir/flags.make

LASM/CMakeFiles/LASM.dir/main.cpp.o: LASM/CMakeFiles/LASM.dir/flags.make
LASM/CMakeFiles/LASM.dir/main.cpp.o: ../LASM/main.cpp
LASM/CMakeFiles/LASM.dir/main.cpp.o: LASM/CMakeFiles/LASM.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object LASM/CMakeFiles/LASM.dir/main.cpp.o"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LASM/CMakeFiles/LASM.dir/main.cpp.o -MF CMakeFiles/LASM.dir/main.cpp.o.d -o CMakeFiles/LASM.dir/main.cpp.o -c /Users/creatorlxd/Project/LLL/LASM/main.cpp

LASM/CMakeFiles/LASM.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LASM.dir/main.cpp.i"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/creatorlxd/Project/LLL/LASM/main.cpp > CMakeFiles/LASM.dir/main.cpp.i

LASM/CMakeFiles/LASM.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LASM.dir/main.cpp.s"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/creatorlxd/Project/LLL/LASM/main.cpp -o CMakeFiles/LASM.dir/main.cpp.s

LASM/CMakeFiles/LASM.dir/LASM.cpp.o: LASM/CMakeFiles/LASM.dir/flags.make
LASM/CMakeFiles/LASM.dir/LASM.cpp.o: ../LASM/LASM.cpp
LASM/CMakeFiles/LASM.dir/LASM.cpp.o: LASM/CMakeFiles/LASM.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object LASM/CMakeFiles/LASM.dir/LASM.cpp.o"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LASM/CMakeFiles/LASM.dir/LASM.cpp.o -MF CMakeFiles/LASM.dir/LASM.cpp.o.d -o CMakeFiles/LASM.dir/LASM.cpp.o -c /Users/creatorlxd/Project/LLL/LASM/LASM.cpp

LASM/CMakeFiles/LASM.dir/LASM.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LASM.dir/LASM.cpp.i"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/creatorlxd/Project/LLL/LASM/LASM.cpp > CMakeFiles/LASM.dir/LASM.cpp.i

LASM/CMakeFiles/LASM.dir/LASM.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LASM.dir/LASM.cpp.s"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/creatorlxd/Project/LLL/LASM/LASM.cpp -o CMakeFiles/LASM.dir/LASM.cpp.s

LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o: LASM/CMakeFiles/LASM.dir/flags.make
LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o: ../LASM/LASMModule.cpp
LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o: LASM/CMakeFiles/LASM.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o -MF CMakeFiles/LASM.dir/LASMModule.cpp.o.d -o CMakeFiles/LASM.dir/LASMModule.cpp.o -c /Users/creatorlxd/Project/LLL/LASM/LASMModule.cpp

LASM/CMakeFiles/LASM.dir/LASMModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LASM.dir/LASMModule.cpp.i"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/creatorlxd/Project/LLL/LASM/LASMModule.cpp > CMakeFiles/LASM.dir/LASMModule.cpp.i

LASM/CMakeFiles/LASM.dir/LASMModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LASM.dir/LASMModule.cpp.s"
	cd /Users/creatorlxd/Project/LLL/build/LASM && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/creatorlxd/Project/LLL/LASM/LASMModule.cpp -o CMakeFiles/LASM.dir/LASMModule.cpp.s

# Object files for target LASM
LASM_OBJECTS = \
"CMakeFiles/LASM.dir/main.cpp.o" \
"CMakeFiles/LASM.dir/LASM.cpp.o" \
"CMakeFiles/LASM.dir/LASMModule.cpp.o"

# External object files for target LASM
LASM_EXTERNAL_OBJECTS =

LASM/LASM: LASM/CMakeFiles/LASM.dir/main.cpp.o
LASM/LASM: LASM/CMakeFiles/LASM.dir/LASM.cpp.o
LASM/LASM: LASM/CMakeFiles/LASM.dir/LASMModule.cpp.o
LASM/LASM: LASM/CMakeFiles/LASM.dir/build.make
LASM/LASM: LASM/libLASM_lib.a
LASM/LASM: LLL/libLLL_lib.a
LASM/LASM: LASM/CMakeFiles/LASM.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/creatorlxd/Project/LLL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable LASM"
	cd /Users/creatorlxd/Project/LLL/build/LASM && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LASM.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
LASM/CMakeFiles/LASM.dir/build: LASM/LASM
.PHONY : LASM/CMakeFiles/LASM.dir/build

LASM/CMakeFiles/LASM.dir/clean:
	cd /Users/creatorlxd/Project/LLL/build/LASM && $(CMAKE_COMMAND) -P CMakeFiles/LASM.dir/cmake_clean.cmake
.PHONY : LASM/CMakeFiles/LASM.dir/clean

LASM/CMakeFiles/LASM.dir/depend:
	cd /Users/creatorlxd/Project/LLL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/creatorlxd/Project/LLL /Users/creatorlxd/Project/LLL/LASM /Users/creatorlxd/Project/LLL/build /Users/creatorlxd/Project/LLL/build/LASM /Users/creatorlxd/Project/LLL/build/LASM/CMakeFiles/LASM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : LASM/CMakeFiles/LASM.dir/depend

