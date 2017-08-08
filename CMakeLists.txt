# Set compiler
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 11)

# Turn on warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -mcrc32 -Wno-literal-suffix")

set(TUBE_SOURCES
   EventHandlerTable.cpp 
   EventHandler.cpp      
   InetAddress.cpp  
   TcpConnection.cpp
)

add_library(tube STATIC ${TUBE_SOURCES})
target_include_directories(tube PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})