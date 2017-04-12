################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PortfolioGeneration/PortfolioGenerator.cpp \
../src/PortfolioGeneration/StrategyB2.cpp \
../src/PortfolioGeneration/StrategyETFR.cpp \
../src/PortfolioGeneration/StrategyS11A.cpp 

OBJS += \
./src/PortfolioGeneration/PortfolioGenerator.o \
./src/PortfolioGeneration/StrategyB2.o \
./src/PortfolioGeneration/StrategyETFR.o \
./src/PortfolioGeneration/StrategyS11A.o 

CPP_DEPS += \
./src/PortfolioGeneration/PortfolioGenerator.d \
./src/PortfolioGeneration/StrategyB2.d \
./src/PortfolioGeneration/StrategyETFR.d \
./src/PortfolioGeneration/StrategyS11A.d 


# Each subdirectory must supply rules for building sources it contributes
src/PortfolioGeneration/%.o: ../src/PortfolioGeneration/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D '__SVN_REV__="$(shell svnversion -n /home/qy/Dropbox/nirvana/workspaceCN/TradingEngine)"' -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


