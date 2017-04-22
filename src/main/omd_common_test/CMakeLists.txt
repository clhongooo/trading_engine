project(omd_common_test)
 
file(GLOB_RECURSE proj_SOURCES "*.cpp")

file(GLOB_RECURSE proj_HEADERS "../omd_mdp_common/*.h")

set (proj_INC "")
foreach (_headerFile ${proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND proj_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES proj_INC)


include_directories(${PANTHEIOS_INC} ${proj_INC} ../omd_mdp_common Testing ../logger ../util)
link_directories(/usr/local/pantheios/lib)

add_executable(${PROJECT_NAME} Main.cpp ${proj_SOURCES})
target_link_libraries(${PROJECT_NAME} logger omd_mdp_common util ${Boost_LIBRARIES} pthread ${GSL_LIBRARIES} ${ZeroMQ_LIBRARY} cppunit pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
