cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(TradingSystem)

## use this to globally use C++11 with in our project
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wfatal-errors -foptimize-sibling-calls")
# set_property(TARGET prog PROPERTY CXX_STANDARD 11)
# set_property(TARGET prog PROPERTY CXX_STANDARD_REQUIRED ON)

set(PANTHEIOS_INC /usr/local/pantheios/include
                  /usr/local/stlsoft/include)

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


##################################################
# cmake can't find GSL in some distros
##################################################
# find_package(GSL REQUIRED)
find_package(GSL)
if (NOT GSL_FOUND)
    set(GSL_ROOT_DIR /usr/local/)
    set(GSL_DIR /usr/local/)
    set(GSL_LIBRARIES dl gsl gslcblas m)
endif()


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
add_subdirectory(strategy_engine_common)
add_subdirectory(strategy_engine)
add_subdirectory(cme_ilink)
add_subdirectory(cme_mdp_kenny)
add_subdirectory(mcast_recorder)
add_subdirectory(ctp_md_ord)
add_subdirectory(logger)
add_subdirectory(util)
add_subdirectory(omd_mdp_common)
add_subdirectory(unit_tests_omd_mdp)
add_subdirectory(omd_mdp)
add_subdirectory(bin_mdi_reader)
add_subdirectory(unit_tests)
add_subdirectory(zmq_svr_clt)


include_directories(${PANTHEIOS_INC}
                    ${Boost_INCLUDE_DIRS}
                    ${GSL_INCLUDE_DIRS}
                    ${ZeroMQ_INCLUDE_DIR}
)
