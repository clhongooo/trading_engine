project(atu_tcp_library)
add_library (${PROJECT_NAME} STATIC ATU_Logger.cpp ATU_TCPClient.cpp ATU_Timer.cpp LogStream.cpp)
