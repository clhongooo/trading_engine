project(mcast_recorder)
add_executable(${PROJECT_NAME} McastReceiver.h McastReceiver.cpp Main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
