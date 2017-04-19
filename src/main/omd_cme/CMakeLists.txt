project(omd_cme)
file(GLOB_RECURSE proj_SOURCES "*.cpp")
add_library (${PROJECT_NAME} STATIC ${proj_SOURCES})
