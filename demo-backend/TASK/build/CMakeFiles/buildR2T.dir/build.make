# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/src/cmake-3.24.3/bin/cmake

# The command to remove a file.
RM = /usr/local/src/cmake-3.24.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jin/demo/TASK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jin/demo/TASK/build

# Include any dependencies generated for this target.
include CMakeFiles/buildR2T.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/buildR2T.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/buildR2T.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/buildR2T.dir/flags.make

CMakeFiles/buildR2T.dir/buildR2T.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/buildR2T.cpp.o: /home/jin/demo/TASK/buildR2T.cpp
CMakeFiles/buildR2T.dir/buildR2T.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/buildR2T.dir/buildR2T.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/buildR2T.cpp.o -MF CMakeFiles/buildR2T.dir/buildR2T.cpp.o.d -o CMakeFiles/buildR2T.dir/buildR2T.cpp.o -c /home/jin/demo/TASK/buildR2T.cpp

CMakeFiles/buildR2T.dir/buildR2T.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/buildR2T.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/buildR2T.cpp > CMakeFiles/buildR2T.dir/buildR2T.cpp.i

CMakeFiles/buildR2T.dir/buildR2T.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/buildR2T.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/buildR2T.cpp -o CMakeFiles/buildR2T.dir/buildR2T.cpp.s

CMakeFiles/buildR2T.dir/util/trie.cc.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/trie.cc.o: /home/jin/demo/TASK/util/trie.cc
CMakeFiles/buildR2T.dir/util/trie.cc.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/buildR2T.dir/util/trie.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/trie.cc.o -MF CMakeFiles/buildR2T.dir/util/trie.cc.o.d -o CMakeFiles/buildR2T.dir/util/trie.cc.o -c /home/jin/demo/TASK/util/trie.cc

CMakeFiles/buildR2T.dir/util/trie.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/trie.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/trie.cc > CMakeFiles/buildR2T.dir/util/trie.cc.i

CMakeFiles/buildR2T.dir/util/trie.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/trie.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/trie.cc -o CMakeFiles/buildR2T.dir/util/trie.cc.s

CMakeFiles/buildR2T.dir/util/util.cc.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/util.cc.o: /home/jin/demo/TASK/util/util.cc
CMakeFiles/buildR2T.dir/util/util.cc.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/buildR2T.dir/util/util.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/util.cc.o -MF CMakeFiles/buildR2T.dir/util/util.cc.o.d -o CMakeFiles/buildR2T.dir/util/util.cc.o -c /home/jin/demo/TASK/util/util.cc

CMakeFiles/buildR2T.dir/util/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/util.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/util.cc > CMakeFiles/buildR2T.dir/util/util.cc.i

CMakeFiles/buildR2T.dir/util/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/util.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/util.cc -o CMakeFiles/buildR2T.dir/util/util.cc.s

CMakeFiles/buildR2T.dir/util/PB.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/PB.cpp.o: /home/jin/demo/TASK/util/PB.cpp
CMakeFiles/buildR2T.dir/util/PB.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/buildR2T.dir/util/PB.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/PB.cpp.o -MF CMakeFiles/buildR2T.dir/util/PB.cpp.o.d -o CMakeFiles/buildR2T.dir/util/PB.cpp.o -c /home/jin/demo/TASK/util/PB.cpp

CMakeFiles/buildR2T.dir/util/PB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/PB.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/PB.cpp > CMakeFiles/buildR2T.dir/util/PB.cpp.i

CMakeFiles/buildR2T.dir/util/PB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/PB.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/PB.cpp -o CMakeFiles/buildR2T.dir/util/PB.cpp.s

CMakeFiles/buildR2T.dir/util/ioUint.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/ioUint.cpp.o: /home/jin/demo/TASK/util/ioUint.cpp
CMakeFiles/buildR2T.dir/util/ioUint.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/buildR2T.dir/util/ioUint.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/ioUint.cpp.o -MF CMakeFiles/buildR2T.dir/util/ioUint.cpp.o.d -o CMakeFiles/buildR2T.dir/util/ioUint.cpp.o -c /home/jin/demo/TASK/util/ioUint.cpp

CMakeFiles/buildR2T.dir/util/ioUint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/ioUint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/ioUint.cpp > CMakeFiles/buildR2T.dir/util/ioUint.cpp.i

CMakeFiles/buildR2T.dir/util/ioUint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/ioUint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/ioUint.cpp -o CMakeFiles/buildR2T.dir/util/ioUint.cpp.s

CMakeFiles/buildR2T.dir/util/ioPB.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/ioPB.cpp.o: /home/jin/demo/TASK/util/ioPB.cpp
CMakeFiles/buildR2T.dir/util/ioPB.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/buildR2T.dir/util/ioPB.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/ioPB.cpp.o -MF CMakeFiles/buildR2T.dir/util/ioPB.cpp.o.d -o CMakeFiles/buildR2T.dir/util/ioPB.cpp.o -c /home/jin/demo/TASK/util/ioPB.cpp

CMakeFiles/buildR2T.dir/util/ioPB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/ioPB.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/ioPB.cpp > CMakeFiles/buildR2T.dir/util/ioPB.cpp.i

CMakeFiles/buildR2T.dir/util/ioPB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/ioPB.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/ioPB.cpp -o CMakeFiles/buildR2T.dir/util/ioPB.cpp.s

CMakeFiles/buildR2T.dir/util/mtrie.cc.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/mtrie.cc.o: /home/jin/demo/TASK/util/mtrie.cc
CMakeFiles/buildR2T.dir/util/mtrie.cc.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/buildR2T.dir/util/mtrie.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/mtrie.cc.o -MF CMakeFiles/buildR2T.dir/util/mtrie.cc.o.d -o CMakeFiles/buildR2T.dir/util/mtrie.cc.o -c /home/jin/demo/TASK/util/mtrie.cc

CMakeFiles/buildR2T.dir/util/mtrie.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/mtrie.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/mtrie.cc > CMakeFiles/buildR2T.dir/util/mtrie.cc.i

CMakeFiles/buildR2T.dir/util/mtrie.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/mtrie.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/mtrie.cc -o CMakeFiles/buildR2T.dir/util/mtrie.cc.s

CMakeFiles/buildR2T.dir/util/canLabel.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/canLabel.cpp.o: /home/jin/demo/TASK/util/canLabel.cpp
CMakeFiles/buildR2T.dir/util/canLabel.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/buildR2T.dir/util/canLabel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/canLabel.cpp.o -MF CMakeFiles/buildR2T.dir/util/canLabel.cpp.o.d -o CMakeFiles/buildR2T.dir/util/canLabel.cpp.o -c /home/jin/demo/TASK/util/canLabel.cpp

CMakeFiles/buildR2T.dir/util/canLabel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/canLabel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/canLabel.cpp > CMakeFiles/buildR2T.dir/util/canLabel.cpp.i

CMakeFiles/buildR2T.dir/util/canLabel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/canLabel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/canLabel.cpp -o CMakeFiles/buildR2T.dir/util/canLabel.cpp.s

CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o: CMakeFiles/buildR2T.dir/flags.make
CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o: /home/jin/demo/TASK/util/canLabelET.cpp
CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o: CMakeFiles/buildR2T.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o -MF CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o.d -o CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o -c /home/jin/demo/TASK/util/canLabelET.cpp

CMakeFiles/buildR2T.dir/util/canLabelET.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/buildR2T.dir/util/canLabelET.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jin/demo/TASK/util/canLabelET.cpp > CMakeFiles/buildR2T.dir/util/canLabelET.cpp.i

CMakeFiles/buildR2T.dir/util/canLabelET.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/buildR2T.dir/util/canLabelET.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jin/demo/TASK/util/canLabelET.cpp -o CMakeFiles/buildR2T.dir/util/canLabelET.cpp.s

# Object files for target buildR2T
buildR2T_OBJECTS = \
"CMakeFiles/buildR2T.dir/buildR2T.cpp.o" \
"CMakeFiles/buildR2T.dir/util/trie.cc.o" \
"CMakeFiles/buildR2T.dir/util/util.cc.o" \
"CMakeFiles/buildR2T.dir/util/PB.cpp.o" \
"CMakeFiles/buildR2T.dir/util/ioUint.cpp.o" \
"CMakeFiles/buildR2T.dir/util/ioPB.cpp.o" \
"CMakeFiles/buildR2T.dir/util/mtrie.cc.o" \
"CMakeFiles/buildR2T.dir/util/canLabel.cpp.o" \
"CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o"

# External object files for target buildR2T
buildR2T_EXTERNAL_OBJECTS =

buildR2T: CMakeFiles/buildR2T.dir/buildR2T.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/util/trie.cc.o
buildR2T: CMakeFiles/buildR2T.dir/util/util.cc.o
buildR2T: CMakeFiles/buildR2T.dir/util/PB.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/util/ioUint.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/util/ioPB.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/util/mtrie.cc.o
buildR2T: CMakeFiles/buildR2T.dir/util/canLabel.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/util/canLabelET.cpp.o
buildR2T: CMakeFiles/buildR2T.dir/build.make
buildR2T: CMakeFiles/buildR2T.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jin/demo/TASK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable buildR2T"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/buildR2T.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/buildR2T.dir/build: buildR2T
.PHONY : CMakeFiles/buildR2T.dir/build

CMakeFiles/buildR2T.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/buildR2T.dir/cmake_clean.cmake
.PHONY : CMakeFiles/buildR2T.dir/clean

CMakeFiles/buildR2T.dir/depend:
	cd /home/jin/demo/TASK/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jin/demo/TASK /home/jin/demo/TASK /home/jin/demo/TASK/build /home/jin/demo/TASK/build /home/jin/demo/TASK/build/CMakeFiles/buildR2T.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/buildR2T.dir/depend

