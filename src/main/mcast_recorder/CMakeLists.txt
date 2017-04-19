project(mcast_recorder)
include_directories(. ../util)
add_executable(${PROJECT_NAME} McastReceiver.cpp Main.cpp)
target_link_libraries(${PROJECT_NAME} util ${Boost_LIBRARIES})
