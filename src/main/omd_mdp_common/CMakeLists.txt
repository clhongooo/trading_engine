project(omd_mdp_common)
file(GLOB_RECURSE proj_HEADERS "*.h")

set (proj_INC "")
foreach (_headerFile ${proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND proj_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES proj_INC)

include_directories(. ${PANTHEIOS_INC} ${proj_INC} ../logger ../util ../libjson)
link_directories(/usr/local/pantheios/lib)

file(GLOB_RECURSE proj_SOURCES "*.cpp")

file(GLOB_RECURSE json_SOURCES "../libjson/Objects_static/*.o")

add_library (${PROJECT_NAME} STATIC ${proj_SOURCES} ${json_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} logger util pthread pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
