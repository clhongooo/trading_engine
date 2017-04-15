project(util)
include_directories(. ../include)
file(GLOB_RECURSE util_SOURCES "*.cpp")
add_library (${PROJECT_NAME} STATIC ${util_SOURCES})
