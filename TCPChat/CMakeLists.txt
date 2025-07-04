
add_executable(Server "Server.cpp")

add_library(TCPServer "TCPServer.h" "TCPServer.cpp")

#add_library(boost INTERFACE)

#if (WIN32) 
#	target_include_directories(boost INTERFACE "C:/Users/sidda/Desktop/books")
#elseif (UNIX) 
#	target_include_directories(boost INTERFACE "/home/kanshou/bb")
#endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UDPServer PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TCPServer PUBLIC boost)
target_link_libraries(Server PUBLIC TCPServer)