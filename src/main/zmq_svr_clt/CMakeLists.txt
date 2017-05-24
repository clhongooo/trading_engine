project(zmq_svr_clt)
 
file(GLOB_RECURSE proj_HEADERS "../util/*.h")

set (proj_INC "")
foreach (_headerFile ${proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND proj_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES proj_INC)
#
# include_directories(${PANTHEIOS_INC} ${proj_INC} ../omd_mdp_common ../atu_tcp_library ../logger ../util ../libjson)
# link_directories(/usr/local/pantheios/lib)
#
# add_executable(${PROJECT_NAME} Main.cpp)
# target_link_libraries(${PROJECT_NAME} logger util omd_mdp_common ${Boost_LIBRARIES} pthread pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)

include_directories(${proj_INC} ${ZeroMQ_INCLUDE_DIR} ../util)
add_executable(zmq_svr Svr.cpp)
add_executable(zmq_clt Clt.cpp)
target_link_libraries(zmq_svr util ${Boost_LIBRARIES} ${ZeroMQ_LIBRARY} pthread)
target_link_libraries(zmq_clt util ${Boost_LIBRARIES} ${ZeroMQ_LIBRARY} pthread)
