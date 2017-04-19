project(omd_cme)
file(GLOB_RECURSE proj_HEADERS "*.h")

set (proj_INC "")
foreach (_headerFile ${proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND proj_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES proj_INC)

include_directories(../include ${PANTHEIOS_INC} ${proj_INC})
link_directories(/usr/local/pantheios/lib)

file(GLOB_RECURSE proj_SOURCES "*.cpp")
add_executable(omd ${proj_SOURCES})
target_link_libraries(omd ${Boost_LIBRARIES} pthread pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
