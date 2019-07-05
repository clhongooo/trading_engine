project(cme_ilink)
include_directories($ENV{QUICKFIXINC})
add_executable(${PROJECT_NAME} utils.cpp sequence_serialization.cpp audit_trail.cpp fix_trader.cpp fix_engine.cpp)
target_link_libraries(${PROJECT_NAME} quickfix)
