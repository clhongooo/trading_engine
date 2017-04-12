project(atu_tcp_library)
add_library (${PROJECT_NAME} STATIC ATU_Feed_Logger.h ATU_Logger.h ATU_TCPClient.h ATU_ThreadSafeQueue.h ATU_Timer.h Constants.h LogStream.h ATU_Feed_Logger.cpp ATU_Logger.cpp ATU_TCPClient.cpp ATU_Timer.cpp LogStream.cpp)
