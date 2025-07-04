# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (UDPServer "CMakeProject1.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UDPServer PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(UDPServer PUBLIC boost)

# TODO: Add tests and install targets if needed.
