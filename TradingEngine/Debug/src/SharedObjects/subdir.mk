################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SharedObjects/CorrelMatrices.cpp \
../src/SharedObjects/MDI_Acknowledgement.cpp \
../src/SharedObjects/MarketData.cpp \
../src/SharedObjects/PortfoliosAndOrders.cpp \
../src/SharedObjects/ProbDistributions.cpp \
../src/SharedObjects/ShortTermAlpha.cpp \
../src/SharedObjects/SystemState.cpp \
../src/SharedObjects/TechIndicators.cpp \
../src/SharedObjects/VolSurfaces.cpp 

OBJS += \
./src/SharedObjects/CorrelMatrices.o \
./src/SharedObjects/MDI_Acknowledgement.o \
./src/SharedObjects/MarketData.o \
./src/SharedObjects/PortfoliosAndOrders.o \
./src/SharedObjects/ProbDistributions.o \
./src/SharedObjects/ShortTermAlpha.o \
./src/SharedObjects/SystemState.o \
./src/SharedObjects/TechIndicators.o \
./src/SharedObjects/VolSurfaces.o 

CPP_DEPS += \
./src/SharedObjects/CorrelMatrices.d \
./src/SharedObjects/MDI_Acknowledgement.d \
./src/SharedObjects/MarketData.d \
./src/SharedObjects/PortfoliosAndOrders.d \
./src/SharedObjects/ProbDistributions.d \
./src/SharedObjects/ShortTermAlpha.d \
./src/SharedObjects/SystemState.d \
./src/SharedObjects/TechIndicators.d \
./src/SharedObjects/VolSurfaces.d 


# Each subdirectory must supply rules for building sources it contributes
src/SharedObjects/%.o: ../src/SharedObjects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D '__SVN_REV__="$(shell svnversion -n /home/qy/Dropbox/nirvana/workspaceCN/TradingEngine)"' -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/include" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/DataAggregation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Logger" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/OrderExecutionAlgo" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/PortfolioGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/ProbDistributionGeneration" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/SharedObjects" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/TechIndUpdating" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/VolSurfaceCalculation" -I"/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/src/Facilities" -I"/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/include" -I/usr/local/pantheios/include -I/usr/local/stlsoft/include -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Math" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Model" -I"/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Util" -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


