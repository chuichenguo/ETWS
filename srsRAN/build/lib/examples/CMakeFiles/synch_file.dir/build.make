# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/kuo/srsLTE_20_10_1/srsRAN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kuo/srsLTE_20_10_1/srsRAN/build

# Include any dependencies generated for this target.
include lib/examples/CMakeFiles/synch_file.dir/depend.make

# Include the progress variables for this target.
include lib/examples/CMakeFiles/synch_file.dir/progress.make

# Include the compile flags for this target's objects.
include lib/examples/CMakeFiles/synch_file.dir/flags.make

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o: lib/examples/CMakeFiles/synch_file.dir/flags.make
lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o: ../lib/examples/synch_file.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuo/srsLTE_20_10_1/srsRAN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/synch_file.dir/synch_file.c.o   -c /home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/synch_file.c

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/synch_file.dir/synch_file.c.i"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/synch_file.c > CMakeFiles/synch_file.dir/synch_file.c.i

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/synch_file.dir/synch_file.c.s"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/synch_file.c -o CMakeFiles/synch_file.dir/synch_file.c.s

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.requires:

.PHONY : lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.requires

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.provides: lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.requires
	$(MAKE) -f lib/examples/CMakeFiles/synch_file.dir/build.make lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.provides.build
.PHONY : lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.provides

lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.provides.build: lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o


# Object files for target synch_file
synch_file_OBJECTS = \
"CMakeFiles/synch_file.dir/synch_file.c.o"

# External object files for target synch_file
synch_file_EXTERNAL_OBJECTS =

lib/examples/synch_file: lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o
lib/examples/synch_file: lib/examples/CMakeFiles/synch_file.dir/build.make
lib/examples/synch_file: lib/src/phy/libsrslte_phy.a
lib/examples/synch_file: /usr/lib/x86_64-linux-gnu/libfftw3f.so
lib/examples/synch_file: lib/examples/CMakeFiles/synch_file.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kuo/srsLTE_20_10_1/srsRAN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable synch_file"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/synch_file.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/examples/CMakeFiles/synch_file.dir/build: lib/examples/synch_file

.PHONY : lib/examples/CMakeFiles/synch_file.dir/build

lib/examples/CMakeFiles/synch_file.dir/requires: lib/examples/CMakeFiles/synch_file.dir/synch_file.c.o.requires

.PHONY : lib/examples/CMakeFiles/synch_file.dir/requires

lib/examples/CMakeFiles/synch_file.dir/clean:
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples && $(CMAKE_COMMAND) -P CMakeFiles/synch_file.dir/cmake_clean.cmake
.PHONY : lib/examples/CMakeFiles/synch_file.dir/clean

lib/examples/CMakeFiles/synch_file.dir/depend:
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuo/srsLTE_20_10_1/srsRAN /home/kuo/srsLTE_20_10_1/srsRAN/lib/examples /home/kuo/srsLTE_20_10_1/srsRAN/build /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples/CMakeFiles/synch_file.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/examples/CMakeFiles/synch_file.dir/depend

