project(strategy_engine_common)
 
file(GLOB_RECURSE strat_eng_HEADERS "*.h")

set (strat_eng_INC "")
foreach (_headerFile ${strat_eng_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND strat_eng_INC ${_dir})
endforeach()
list(REMOVE_DUPLICATES strat_eng_INC)

include_directories(../include ${PANTHEIOS_INC} ${strat_eng_INC} ../atu_tcp_library)

file(GLOB_RECURSE strat_eng_SOURCES "*.cpp")

add_library(${PROJECT_NAME} STATIC ${strat_eng_SOURCES})
