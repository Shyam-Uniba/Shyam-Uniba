# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /home/shyam/GSI/fairsoft_may18p1_root6/installation/bin/cmake

# The command to remove a file.
RM = /home/shyam/GSI/fairsoft_may18p1_root6/installation/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shyam/Desktop/MakeExample/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shyam/Desktop/MakeExample/cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/efficiency.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/efficiency.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/efficiency.dir/flags.make

CMakeFiles/efficiency.dir/MyClass.cc.o: CMakeFiles/efficiency.dir/flags.make
CMakeFiles/efficiency.dir/MyClass.cc.o: ../MyClass.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shyam/Desktop/MakeExample/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/efficiency.dir/MyClass.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/efficiency.dir/MyClass.cc.o -c /home/shyam/Desktop/MakeExample/cmake/MyClass.cc

CMakeFiles/efficiency.dir/MyClass.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/efficiency.dir/MyClass.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shyam/Desktop/MakeExample/cmake/MyClass.cc > CMakeFiles/efficiency.dir/MyClass.cc.i

CMakeFiles/efficiency.dir/MyClass.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/efficiency.dir/MyClass.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shyam/Desktop/MakeExample/cmake/MyClass.cc -o CMakeFiles/efficiency.dir/MyClass.cc.s

CMakeFiles/efficiency.dir/main.cc.o: CMakeFiles/efficiency.dir/flags.make
CMakeFiles/efficiency.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shyam/Desktop/MakeExample/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/efficiency.dir/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/efficiency.dir/main.cc.o -c /home/shyam/Desktop/MakeExample/cmake/main.cc

CMakeFiles/efficiency.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/efficiency.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shyam/Desktop/MakeExample/cmake/main.cc > CMakeFiles/efficiency.dir/main.cc.i

CMakeFiles/efficiency.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/efficiency.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shyam/Desktop/MakeExample/cmake/main.cc -o CMakeFiles/efficiency.dir/main.cc.s

# Object files for target efficiency
efficiency_OBJECTS = \
"CMakeFiles/efficiency.dir/MyClass.cc.o" \
"CMakeFiles/efficiency.dir/main.cc.o"

# External object files for target efficiency
efficiency_EXTERNAL_OBJECTS =

libefficiency.so: CMakeFiles/efficiency.dir/MyClass.cc.o
libefficiency.so: CMakeFiles/efficiency.dir/main.cc.o
libefficiency.so: CMakeFiles/efficiency.dir/build.make
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libCore.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libImt.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libRIO.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libNet.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libHist.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libGraf.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libGraf3d.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libGpad.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libTree.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libTreePlayer.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libRint.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libPostscript.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libMatrix.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libPhysics.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libMathCore.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libThread.so
libefficiency.so: /home/shyam/GSI/fairsoft_may18p1_root6/installation/lib/root/libMultiProc.so
libefficiency.so: CMakeFiles/efficiency.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shyam/Desktop/MakeExample/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libefficiency.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/efficiency.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/efficiency.dir/build: libefficiency.so

.PHONY : CMakeFiles/efficiency.dir/build

CMakeFiles/efficiency.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/efficiency.dir/cmake_clean.cmake
.PHONY : CMakeFiles/efficiency.dir/clean

CMakeFiles/efficiency.dir/depend:
	cd /home/shyam/Desktop/MakeExample/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shyam/Desktop/MakeExample/cmake /home/shyam/Desktop/MakeExample/cmake /home/shyam/Desktop/MakeExample/cmake/build /home/shyam/Desktop/MakeExample/cmake/build /home/shyam/Desktop/MakeExample/cmake/build/CMakeFiles/efficiency.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/efficiency.dir/depend

