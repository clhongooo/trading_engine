project(strategy_engine_common)
 
file(GLOB_RECURSE strat_eng_HEADERS "*.h")

set (strat_eng_INC "")
foreach (_headerFile ${strat_eng_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND strat_eng_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES strat_eng_INC)

include_directories(../include ../logger ${PANTHEIOS_INC} ${strat_eng_INC} ../atu_tcp_library ../util)

file(GLOB_RECURSE strat_eng_SOURCES "*.cpp")
# link_directories(/usr/local/pantheios/lib)

add_library(${PROJECT_NAME} STATIC ${strat_eng_SOURCES})
# target_link_libraries(${PROJECT_NAME} logger pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
target_link_libraries(${PROJECT_NAME} logger util)
