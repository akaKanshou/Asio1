# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(boost INTERFACE)
if (WIN32) 
	target_include_directories(boost INTERFACE "C:/Users/sidda/Desktop/books")
elseif (UNIX) 
	target_include_directories(boost INTERFACE "/home/kanshou/bb")
endif()

project ("CMakeProject1")

# Include sub-projects.
add_subdirectory ("CMakeProject1")
add_subdirectory ("TCPChat")
