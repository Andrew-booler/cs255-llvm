# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cs255-llvm-pass

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cs255-llvm-pass/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/InstCounterPass.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/InstCounterPass.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/InstCounterPass.dir/flags.make

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o: lib/CMakeFiles/InstCounterPass.dir/flags.make
lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o: ../lib/InstCounter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cs255-llvm-pass/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o"
	cd /cs255-llvm-pass/build/lib && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o -c /cs255-llvm-pass/lib/InstCounter.cpp

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/InstCounterPass.dir/InstCounter.cpp.i"
	cd /cs255-llvm-pass/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cs255-llvm-pass/lib/InstCounter.cpp > CMakeFiles/InstCounterPass.dir/InstCounter.cpp.i

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/InstCounterPass.dir/InstCounter.cpp.s"
	cd /cs255-llvm-pass/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cs255-llvm-pass/lib/InstCounter.cpp -o CMakeFiles/InstCounterPass.dir/InstCounter.cpp.s

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.requires:

.PHONY : lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.requires

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.provides: lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.requires
	$(MAKE) -f lib/CMakeFiles/InstCounterPass.dir/build.make lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.provides.build
.PHONY : lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.provides

lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.provides.build: lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o


# Object files for target InstCounterPass
InstCounterPass_OBJECTS = \
"CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o"

# External object files for target InstCounterPass
InstCounterPass_EXTERNAL_OBJECTS =

lib/libInstCounterPass.so: lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o
lib/libInstCounterPass.so: lib/CMakeFiles/InstCounterPass.dir/build.make
lib/libInstCounterPass.so: lib/CMakeFiles/InstCounterPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cs255-llvm-pass/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module libInstCounterPass.so"
	cd /cs255-llvm-pass/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/InstCounterPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/InstCounterPass.dir/build: lib/libInstCounterPass.so

.PHONY : lib/CMakeFiles/InstCounterPass.dir/build

lib/CMakeFiles/InstCounterPass.dir/requires: lib/CMakeFiles/InstCounterPass.dir/InstCounter.cpp.o.requires

.PHONY : lib/CMakeFiles/InstCounterPass.dir/requires

lib/CMakeFiles/InstCounterPass.dir/clean:
	cd /cs255-llvm-pass/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/InstCounterPass.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/InstCounterPass.dir/clean

lib/CMakeFiles/InstCounterPass.dir/depend:
	cd /cs255-llvm-pass/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cs255-llvm-pass /cs255-llvm-pass/lib /cs255-llvm-pass/build /cs255-llvm-pass/build/lib /cs255-llvm-pass/build/lib/CMakeFiles/InstCounterPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/InstCounterPass.dir/depend

