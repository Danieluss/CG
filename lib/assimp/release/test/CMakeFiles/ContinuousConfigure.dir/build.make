# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.11

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Burdel\Download\assimp-4.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Burdel\Download\assimp-4.1.0\release

# Utility rule file for ContinuousConfigure.

# Include the progress variables for this target.
include test/CMakeFiles/ContinuousConfigure.dir/progress.make

test/CMakeFiles/ContinuousConfigure:
	cd /d E:\Burdel\Download\assimp-4.1.0\release\test && "C:\Program Files\CMake\bin\ctest.exe" -D ContinuousConfigure

ContinuousConfigure: test/CMakeFiles/ContinuousConfigure
ContinuousConfigure: test/CMakeFiles/ContinuousConfigure.dir/build.make

.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
test/CMakeFiles/ContinuousConfigure.dir/build: ContinuousConfigure

.PHONY : test/CMakeFiles/ContinuousConfigure.dir/build

test/CMakeFiles/ContinuousConfigure.dir/clean:
	cd /d E:\Burdel\Download\assimp-4.1.0\release\test && $(CMAKE_COMMAND) -P CMakeFiles\ContinuousConfigure.dir\cmake_clean.cmake
.PHONY : test/CMakeFiles/ContinuousConfigure.dir/clean

test/CMakeFiles/ContinuousConfigure.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Burdel\Download\assimp-4.1.0 E:\Burdel\Download\assimp-4.1.0\test E:\Burdel\Download\assimp-4.1.0\release E:\Burdel\Download\assimp-4.1.0\release\test E:\Burdel\Download\assimp-4.1.0\release\test\CMakeFiles\ContinuousConfigure.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/ContinuousConfigure.dir/depend
