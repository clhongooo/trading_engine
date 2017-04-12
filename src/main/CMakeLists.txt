##################################################
#  ----------------
# < Trading Engine >
#  ----------------
#         \   ^__^
#          \  (oo)\_______
#             (__)\       )\/\
#                 ||----w |
#                 ||     ||
#
##################################################

cmake_minimum_required (VERSION 3.0)

project(TradingEngine)

## use this to globally use C++11 with in our project
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_INC 
                 atu_tcp_library
                 Common/ATU
                 Common/Logger
                 Common/Math
                 Common/Model
                 Common/PCH
                 Common/Util
                 Common/DataAggregation
                 Common/Facilities
                 Common/LowPriorityFacilities
                 Common/OrderExecutionAlgo
                 Common/ProbDistributionGeneration
                 Common/SharedObjects
                 Common/Strategies
                 Common/StrategyTests
                 Common/StrategyUtils
                 Common/TechIndUpdating
                 Common/VolSurfaceCalculation)
set(PANTHEIOS_INC /usr/local/pantheios/include
                  /usr/local/stlsoft/include)
set(PROJECT_SRC Main.cpp)    

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS
                              date_time
                              iostreams
                              system
                              thread
                              filesystem
                              REQUIRED)

find_package(GSL REQUIRED)


##################################################
# ZMQ
##################################################
# find_library(ZMQ_LIB libzmq)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

##################################################



add_subdirectory(atu_tcp_library)
add_subdirectory(Common)


if(Boost_FOUND AND GSL_FOUND)
    include_directories(${PROJECT_INC}
                        ${PANTHEIOS_INC}
                        ${Boost_INCLUDE_DIRS}
                        ${GSL_INCLUDE_DIRS}
                        ${ZeroMQ_INCLUDE_DIR}) 

    link_directories(/usr/local/pantheios/lib)
    add_executable(${PROJECT_NAME} ${PROJECT_SRC})

    target_link_libraries(${PROJECT_NAME} Common ${Boost_LIBRARIES} pthread ${GSL_LIBRARIES} ${ZeroMQ_LIBRARY} pantheios.1.core.gcc46 pantheios.1.be.file.gcc46 pantheios.1.bec.file.gcc46 pantheios.1.util.gcc46)
endif()

