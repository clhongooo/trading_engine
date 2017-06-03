project(ib_md_ord)
file(GLOB_RECURSE ib_HEADERS "*.h")

set (ib_INC "")
foreach (_headerFile ${ib_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ib_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES ib_INC)

include_directories(../logger ${ib_INC} ../util)

file(GLOB_RECURSE ib_SOURCES "*.cpp")

link_directories(/usr/local/pantheios/lib)
add_executable(${PROJECT_NAME} ${ib_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${ZeroMQ_LIBRARY} logger util pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
