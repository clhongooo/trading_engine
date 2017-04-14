project(cme_mdp)
include_directories(. sbe cme)
add_executable(${PROJECT_NAME} error.c CmeMdApi.cpp cme-md.cpp)
target_link_libraries(${PROJECT_NAME} pthread)
