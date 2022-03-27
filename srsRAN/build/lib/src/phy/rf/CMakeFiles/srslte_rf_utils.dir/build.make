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
include lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/depend.make

# Include the progress variables for this target.
include lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/progress.make

# Include the compile flags for this target's objects.
include lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/flags.make

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/flags.make
lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o: ../lib/src/phy/rf/rf_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuo/srsLTE_20_10_1/srsRAN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o   -c /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/rf/rf_utils.c

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/srslte_rf_utils.dir/rf_utils.c.i"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/rf/rf_utils.c > CMakeFiles/srslte_rf_utils.dir/rf_utils.c.i

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/srslte_rf_utils.dir/rf_utils.c.s"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/rf/rf_utils.c -o CMakeFiles/srslte_rf_utils.dir/rf_utils.c.s

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.requires:

.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.requires

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.provides: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.requires
	$(MAKE) -f lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/build.make lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.provides.build
.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.provides

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.provides.build: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o


# Object files for target srslte_rf_utils
srslte_rf_utils_OBJECTS = \
"CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o"

# External object files for target srslte_rf_utils
srslte_rf_utils_EXTERNAL_OBJECTS =

lib/src/phy/rf/libsrslte_rf_utils.a: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o
lib/src/phy/rf/libsrslte_rf_utils.a: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/build.make
lib/src/phy/rf/libsrslte_rf_utils.a: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kuo/srsLTE_20_10_1/srsRAN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsrslte_rf_utils.a"
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && $(CMAKE_COMMAND) -P CMakeFiles/srslte_rf_utils.dir/cmake_clean_target.cmake
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/srslte_rf_utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/build: lib/src/phy/rf/libsrslte_rf_utils.a

.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/build

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/requires: lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/rf_utils.c.o.requires

.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/requires

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/clean:
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf && $(CMAKE_COMMAND) -P CMakeFiles/srslte_rf_utils.dir/cmake_clean.cmake
.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/clean

lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/depend:
	cd /home/kuo/srsLTE_20_10_1/srsRAN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuo/srsLTE_20_10_1/srsRAN /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/rf /home/kuo/srsLTE_20_10_1/srsRAN/build /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/src/phy/rf/CMakeFiles/srslte_rf_utils.dir/depend

